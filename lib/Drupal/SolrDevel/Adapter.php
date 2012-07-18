<?php

/**
 * @file
 * Contains Drupal_SolrDevel_Adapter.
 */

/**
 * Base adapter class to abstract various debug functionality.
 */
abstract class Drupal_SolrDevel_Adapter {

  /**
   * An array of options, usually containing contextual information about the
   * index or server this adapter is associated with.
   *
   * @var array
   */
  protected $_options = array();

  /**
   * Constructs a Drupal_SolrDevel_Adapter object.
   *
   * @param array $options
   *   An array of options, usually containing contextual information about the
   *   index or server this adapter is associated with.
   */
  public function __construct(array $options = array()) {
    $this->_options = $options;
  }

  /**
   * Returns an option or a default value if the option is not set.
   *
   * @param string $name
   *   The name of the option, which is the array key of the class property
   *   Drupal_SolrDevel_Adapter::_options.
   * @param mixed $default
   *   (optional) The default if the option doesn't exist. Defaults to NULL.
   *
   * @return mixed
   *   The option being requested.
   */
  public function getOption($name, $default = NULL) {
    return (isset($this->_options[$name])) ? $this->_options[$name] : $default;
  }

  /**
   * Sets an option for this instance.
   *
   * @param string $name
   *   The name of the option, which is the array key of the class property
   *   Drupal_SolrDevel_Adapter::_options.
   * @param mixed $value
   *   The value being stored as an option.
   *
   * @return Drupal_SolrDevel_Adapter
   *   An instance of this class.
   */
  public function setOption($name, $value) {
    $this->_options[$name] = $value;
    return $this;
  }

  /**
   * Tests whether an entity is indexed.
   *
   * @param int $entity_id
   *   The unique identifier of the entity.
   * @param string $entity_type
   *   The machine name of the entity, defaults to "node".
   *
   * @return boolean
   *   TRUE if the entity is indexed, FALSE otherwise.
   */
  abstract public function entityIndexed($entity_id, $entity_type = 'node');
}
