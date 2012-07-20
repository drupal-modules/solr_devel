<?php

/**
 * @file
 * Page callbacks for the Solr Devel module.
 */

/**
 * Initializes the render array for analysis pages.
 *
 * @param string $title
 *   The title of the page.
 *
 * @return array
 *   The initialized render array.
 */
function solr_devel_init_build($title = '') {
  $build = array();

  // Adds link to return to overview page.
  if (isset($_GET['destination']) && !url_is_external($_GET['destination'])) {
    $build['link'] = array(
      '#type' => 'link',
      '#title' => t('Go back to overview'),
      '#href' => $_GET['destination'],
    );
  }

  return $build;
}

/**
 * Node debugging page callback.
 *
 * @param $node
 *   The node being rendered.
 *
 * @return array
 *   The render array.
 */
function solr_devel_node_overview_page($node) {
  return solr_devel_get_entity_table($node->nid, $node->type, 'node');
}

/**
 * Gets Solr devel information for an entity.
 *
 * @param int $entity_id
 *   The unique identifier of the entity.
 * @param string $bundle
 *   The entity's bundle.
 * @param string $entity_type
 *   The machine name of the entity.
 */
function solr_devel_get_entity_table($entity_id, $bundle, $entity_type) {
  $build = array();

  $rows = array();
  $environments = solr_devel_get_environment_info();
  foreach ($environments as $id => $environment) {
    $adapter = solr_devel_adapter_load($environment);

    $indexed = $adapter->entityIndexed($entity_id, $entity_type);
    $queued = $adapter->getQueue($entity_id, $bundle, $entity_type)->getStatus();

    $row = array(
      'environment' => check_plain($adapter->getLabel()),
      'indexed' => $indexed ? t('Yes') : t('No'),
      'queued' => $queued ? t('Yes') : t('No'),
      'operations' => '',
    );

    // Initialize operations, get the
    $operations = array();
    $destination = drupal_get_destination();

    $operations['queue'] = array(
      'title' => t('analyze queue'),
      'href' => $entity_type . '/' . $entity_id . '/devel/solr/' . $id . '/queue',
      'query' => $destination,
    );

    if ($indexed) {
      $operations['document'] = array(
        'title' => t('analyze document'),
        'href' => $entity_type . '/' . $entity_id . '/devel/solr/' . $id . '/document',
        'query' => $destination,
      );
    }

    $row['operations'] = array(
      'data' => array(
        '#theme' => 'links__node_operations',
        '#links' => $operations,
        '#attributes' => array('class' => array('links', 'inline')),
      ),
    );

    $rows[] = $row;
  }

  $header = array(
    'environment' => t('Environment'),
    'indexed' => t('Indexed'),
    'queued' => t('Queued for indexing'),
    'operations' => t('Operations'),
  );

  $build['index_stats'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#header' => $header,
  );

  return $build;
}

/**
 * Prints debug information about the queue.
 *
 * @param stdClass $node
 *   The node being rendered.
 * @param array $environment
 *   The environment definition.
 *
 * @return array
 *   The render array.
 */
function solr_devel_node_queue_analysis_page($node, $environment) {
  $build = solr_devel_init_build();

  $adapter = solr_devel_adapter_load($environment);
  $debug = $adapter->getQueue($node->nid, $node->type, 'node')->getDebug();
  $build['debug'] = array('#markup' => kprint_r($debug, TRUE));

  return $build;
}

/**
 * Prints debug information about the document in the Solr index related to the
 * node.
 *
 * @param stdClass $node
 *   The node being rendered.
 * @param array $environment
 *   The environment definition.
 *
 * @return array
 *   The render array.
 */
function solr_devel_node_document_analysis_page($node, $environment) {
  $build = solr_devel_init_build();

  $adapter = solr_devel_adapter_load($environment);
  $document = $adapter->getDocument($node->nid, 'node');
  $build['document'] = array('#markup' => kprint_r($document, TRUE));

  return $build;
}
