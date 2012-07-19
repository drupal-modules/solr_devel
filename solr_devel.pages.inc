<?php

/**
 * @file
 * Page callbacks for the Solr Devel module.
 */

function solr_devel_node_page($node) {
  $build = array();

  $rows = array();
  $environments = solr_devel_get_environment_info();
  foreach ($environments as $environment) {

    $adapter = solr_devel_adapter_load($environment);
    $indexed = $adapter->entityIndexed($node->nid);

    $row = array(
      'environment' => check_plain($adapter->getLabel()),
      'indexed' => $indexed ? t('Yes') : t('No'),
      'queue' => t('Maybe?'),
      'operations' => '',
    );

    $rows[] = $row;
  }

  $header = array(
    'environment' => t('Environment'),
    'indexed' => t('Indexed'),
    'queue' => t('Queued'),
    'operations' => t('Operations'),
  );

  $build['index_stats'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#header' => $header,
  );

  return $build;
}
