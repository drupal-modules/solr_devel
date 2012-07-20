<?php

/**
 * @file
 * Page callbacks for the Solr Devel module.
 */

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

    $row = array(
      'environment' => check_plain($adapter->getLabel()),
      'indexed' => $adapter->entityIndexed($entity_id, $entity_type) ? t('Yes') : t('No'),
      'queued' => $adapter->getQueue($entity_id, $bundle, $entity_type)->getStatus() ? t('Yes') : t('No'),
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
  $adapter = solr_devel_adapter_load($environment);
  dpm($adapter->getQueue($node->nid, $node->type, 'node')->getDebug());
  return array();
}
