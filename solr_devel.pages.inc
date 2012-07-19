<?php

/**
 * @file
 * Page callbacks for the Solr Devel module.
 */

function solr_devel_node_page($node) {
  $environments = solr_devel_get_environment_info();
  foreach ($environments as $environment) {
    $adapter = solr_devel_adapter_load($environment);
    dpm($adapter->entityIndexed($node->nid));
  }

  return array();
}