<?php

/**
 * @file
 * Contains Drupal_SolrDevel_ApacheSolr_Adapter.
 */

/**
 * Apache Solr Search Integration's implementation of the Solr Devel adapter.
 */
class Drupal_SolrDevel_ApacheSolr_Adapter extends Drupal_SolrDevel_Adapter {

  /**
   * Implements Drupal_SolrDevel_Adapter::searchByEntity().
   *
   * @todo Catch exceptions?
   */
  public function entityIndexed($entity_id, $entity_type = 'node') {
    $solr = apachesolr_get_solr($this->getOption('env_id'));
    $id = apachesolr_document_id($entity_id, $entity_type);
    $params = array('fq' => 'id:' . $id);
    $response = $solr->search('', $params);
    return (bool) $response->response->numFound;
  }
}
