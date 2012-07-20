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
   * Helper function to searhc by unique identifier.
   *
   * @param int $entity_id
   *   The unique identifier of the entity.
   * @param string $entity_type
   *   The machine name of the entity.
   *
   * @return
   */
  public function searchByIdentifier($entity_id, $entity_type) {
    $solr = apachesolr_get_solr($this->getOption('env_id'));
    $id = apachesolr_document_id($entity_id, $entity_type);
    $params = array('fq' => 'id:' . $id);
    return $solr->search('', $params);
  }

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
      $response = $this->searchByIdentifier($entity_id, $entity_type);
      return (bool) $response->response->numFound;
    }
    catch (Exception $e) {
      $this->setError($e->getMessage());
      return FALSE;
    }
  }

  /**
   * Implements Drupal_SolrDevel_Adapter::getDocument().
   */
  public function getDocument($entity_id, $entity_type) {
    try {
      $response = $this->searchByIdentifier($entity_id, $entity_type);
      if (isset($response->response->docs[0])) {
        return $response->response->docs[0];
      }
      else {
        return FALSE;
      }
    }
    catch (Exception $e) {
      $this->setError($e->getMessage());
      return FALSE;
    }
  }
}
