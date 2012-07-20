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
function solr_devel_node_page($node) {
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
  foreach ($environments as $environment) {
    $adapter = solr_devel_adapter_load($environment);

    $row = array(
      'environment' => check_plain($adapter->getLabel()),
      'indexed' => $adapter->entityIndexed($entity_id, $entity_type) ? t('Yes') : t('No'),
      'queued' => $adapter->entityQueued($entity_id, $bundle, $entity_type) ? t('Yes') : t('No'),
      'operations' => '',
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
