<?php

/**
 * @file
 * Contains Drupal_SolrDevel_ApacheSolr_Queue.
 */

/**
 * Checks the queue status of an entity.
 *
 * Breaks up the queue process into parts so we can analyze it. Stores debug
 * data so developers can determined why an entity is in the queue or not.
 */
class Drupal_SolrDevel_ApacheSolr_Queue {

  /**
   * The machine name of the environment.
   *
   * @var string
   */
  protected $_envId;

  /**
   * The unique identifier of the entity.
   *
   * @var int
   */
  protected $_entityId;

  /**
   * The entity's bundle.
   *
   * @var string
   */
  protected $_bundle;

  /**
   * The machine name of the entity.
   *
   * @var string
   */
  protected $_entityType;

  /**
   * Stores debug information.
   *
   * @var array
   */
  protected $_debug;

  /**
   * Constructs a Drupal_SolrDevel_ApacheSolr_Queue object.
   *
   * @param string $env_id
   *   The machine name of the environment.
   * @param int $entity_id
   *   The unique identifier of the entity.
   * @param string $bundle
   *   The entity's bundle.
   * @param string $entity_type
   *   The machine name of the entity.
   */
  public function __construct($env_id, $entity_id, $bundle, $entity_type) {
    $this->_envId = $env_id;
    $this->_entityId = $entity_id;
    $this->_bundle = $bundle;
    $this->_entityType = $entity_type;
  }

  /**
   * Runs the queue
   *
   * @return boolean
   *   Whether the content is queued for indexing.
   */
  public function run() {
    $queued = TRUE;

    // Initialize the debug array.
    $this->_debug = array(
      'read_only' => FALSE,
      'bundle_excluded' => FALSE,
      'in_table' => TRUE,
      'processed' => FALSE,
      'status_callbacks' => array(),
      'status_callbacks_skipped' => array(),
      'exclude_hooks' => array(),
    );

    // Return FALSE if index is read only.
    if (variable_get('apachesolr_read_only', 0)) {
      $this->_debug['read_only'] = TRUE;
      $queued = FALSE;
    }

    // Get bundles that are allowed to be indexed.
    $bundles = drupal_map_assoc(
      apachesolr_get_index_bundles($this->_envId, $this->_entityType)
    );

    // Checks whether the bundle is excluded.
    if (!isset($bundles[$this->_bundle])) {
      $this->_debug['bundle_excluded'] = TRUE;
      $queued = FALSE;
    }

    // Get $last_entity_id and $last_changed.
    extract(apachesolr_get_last_index_position($this->_envId, $this->_entityType));
    $table = apachesolr_get_indexer_table($this->_entityType);

    // Build the queue query.
    $query = db_select($table, 'aie')
      ->fields('aie')
      ->condition('aie.bundle', $bundles)
      ->condition('entity_id', $this->_entityId)
      ->condition(db_or()
        ->condition('aie.changed', $last_changed, '>')
        ->condition(db_and()
          ->condition('aie.changed', $last_changed, '<=')
          ->condition('aie.entity_id', $last_entity_id, '>')
         )
       );

    // Entity-specific tables don't need this condition.
    if ($table == 'apachesolr_index_entities') {
      $query->condition('aie.entity_type', $this->_entityType);
    }

    // If no records are returned, the item has been processed.
    if (!$record = $query->execute()->fetch()) {
      $this->_debug['processed'] = TRUE;
      $queued = FALSE;
    }

    // Loads index include, which is where the default status callbacks live.
    module_load_include('inc', 'apachesolr', 'apachesolr.index');

    // Ensure entry is in table. If not, we have a problem.
    $query = db_select($table, 'aie')
      ->fields('aie', array('status'))
      ->condition('aie.entity_type', $this->_entityType)
      ->condition('aie.entity_id', $this->_entityId);

    // Invokes status callback to check whether entity should be excluded. For
    // example, the apachesolr_index_node_status_callback() tests if the node
    // status is 0, meaning it is unpublished.
    if ($record = $query->execute()->fetch()) {
      $status_callbacks = apachesolr_entity_get_callback($this->_entityType, 'status callback');
      if (is_array($status_callbacks)) {
        foreach ($status_callbacks as $status_callback) {
          if (is_callable($status_callback)) {
            $callback_value = $status_callback($this->_entityId, $this->_entityType);
            $record->status = $record->status && $callback_value;
            $this->_debug['status_callbacks'][$status_callback] = (!$callback_value); // FALSE
          }
          else {
            $this->_debug['status_callbacks_skipped'][$status_callback] = TRUE;
          }
        }
      }
    }
    else {
      // There is a problem with the queue if the data is not here.
      $this->_debug['in_table'] = FALSE;
      $queued = FALSE;
    }

    // Invoke hook_apachesolr_exclude().
    foreach (module_implements('apachesolr_exclude') as $module) {
      $function = $module . '_apachesolr_exclude';
      $exclude = module_invoke($module, 'apachesolr_exclude', $record->entity_id, $this->_entityType, $record, $this->_envId);
      if (!empty($exclude)) {
        $this->_debug['exclude_hooks'][$function] = TRUE;
        $queued = FALSE;
      }
      else {
        $this->_debug['exclude_hooks'][$function] = FALSE;
      }
    }

    // Invoke hook_apachesolr_ENTITY_TYPE_exclude().
    foreach (module_implements('apachesolr_' . $this->_entityType . '_exclude') as $module) {
      $function = $module . '_apachesolr_' . $this->_entityType . '_exclude';
      $exclude = module_invoke($module, 'apachesolr_' . $this->_entityType . '_exclude', $record->entity_id, $record, $this->_envId);
      if (!empty($exclude)) {
        $this->_debug['exclude_hooks'][$function] = TRUE;
        $queued = FALSE;
      }
      else {
        $this->_debug['exclude_hooks'][$function] = FALSE;
      }
    }

    return $queued;
  }

  /**
   * Gets the debug information.
   *
   * @return array
   *
   */
  public function getDebug() {
    return $this->_debug;
  }
}
