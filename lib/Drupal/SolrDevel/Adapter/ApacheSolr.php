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
   * Implements Drupal_SolrDevel_Adapter::entityIndexed().
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
      $doc_returned = isset($response->response->docs[0]);
      return ($doc_returned) ? $response->response->docs[0] : FALSE;
    }
    catch (Exception $e) {
      $this->setError($e->getMessage());
      return FALSE;
    }
  }

  /**
   * Implements Drupal_SolrDevel_Adapter::analyzeQuery().
   */
  public function analyzeQuery($keys, $page_id, $entity_id, $entity_type) {
    $search_page = apachesolr_search_page_load($page_id);
    $conditions = apachesolr_search_conditions_default($search_page);
    $solr = apachesolr_get_solr($search_page->env_id);

    // Default parameters
    $params = array(
      'q' => $keys,
      'fq' => isset($conditions['fq']) ? $conditions['fq'] : array(),
      'rows' => 1,
    );

    $params['fq'][] = 'id:' . apachesolr_document_id($entity_id, $entity_type);
    $results = apachesolr_search_run('apachesolr', $params, '', '', 0, $solr);
    return isset($results[0]) ? $results[0] : array();
  }

  /**
   * Implements Drupal_SolrDevel_Adapter::getSearchPageOptions().
   */
  public function getSearchPageOptions($environment) {
    $options = array();
    $env_id = ltrim(strstr($environment['name'], ':'), ':');
    $sql = 'SELECT page_id, label FROM {apachesolr_search_page} WHERE env_id = :env_id';
    $result = db_query($sql, array(':env_id' => $env_id));
    foreach ($result as $record) {
      $options[$record->page_id] = check_plain($record->label);
    }
    return $options;
  }
}
