<?php

/**
 * @file
 * Contains Drupal_SolrDevel_ApacheSolr_Adapter.
 */

/**
 * Apache Solr Search Integration's implementation of the Solr Devel adapter.
 */
class Drupal_SolrDevel_Adapter_ApacheSolr extends Drupal_SolrDevel_Adapter {

  /**
   * Implements Drupal_SolrDevel_Adapter::getQueue().
   */
  public function getQueue($entity_id, $bundle, $entity_type) {
    return new Drupal_SolrDevel_Queue_Apachesolr($this, $entity_id, $bundle, $entity_type);
  }

  /**
   * Implements Drupal_SolrDevel_Adapter::searchByEntity().
   */
  public function entityIndexed($entity_id, $entity_type) {
    try {
      $solr = apachesolr_get_solr($this->getOption('env_id'));
      $id = apachesolr_document_id($entity_id, $entity_type);
      $params = array('fq' => 'id:' . $id);
      $response = $solr->search('', $params);
      return (bool) $response->response->numFound;
    }
    catch (Exception $e) {
      $this->setError($e->getMessage());
      return FALSE;
    }
  }
}
