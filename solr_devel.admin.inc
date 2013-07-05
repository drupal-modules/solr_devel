<?php

/**
 * @file
 * Administrative page callbacks for the Solr Devel module.
 */

/**
 * Displays the admin settings form.
 *
 * @ingroup forms
 */
function solr_devel_settings_form($form, &$form_state) {

  // Arguments for the description.
  $args = array(
    '@permissions-url' => url('admin/people/permissions', array('fragment' => 'module-solr_devel')),
  );

  if (module_exists('apachesolr')) {
    // These options works with apachesolr only.
    $form['solr_devel:apachesolr'] = array(
      '#type' => 'fieldset',
      '#title' => t('ApacheSolr'),
    );

    $form['solr_devel:apachesolr']['solr_devel:debug_queries'] = array(
      '#type' => 'checkbox',
      '#title' => t('Debug Solr queries'),
      '#default_value' => variable_get('solr_devel:debug_queries', 0),
      '#description' => t('Display debug information about Solr queries, for example a breakdown of how documents are ranked in search results and information on the Solr request. <strong>Debugging can decrease solr performance substantially</strong>, so make sure to grant the <a href="@permissions-url">view solr_devel information</a> permissions only to administrative users.', $args),
    );
  }

  if (module_exists('search_api_solr')) {
    // These options works with search_api_solr only.
    $form['solr_devel:search_api_solr'] = array(
      '#type' => 'fieldset',
      '#title' => t('Search Api Solr'),
      '#description' => t('Debug messages will be displayed in status area of the current view.'),
    );

    $form['solr_devel:search_api_solr']['solr_devel:debugging'] = array(
      '#type' => 'fieldset',
      '#title' => t('Debugging'),
    );

    $form['solr_devel:search_api_solr']['solr_devel:debugging']['solr_devel:debugging_queries_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display Solr queries'),
      '#default_value' => variable_get('solr_devel:debugging_queries_enabled', FALSE),
    );

    $form['solr_devel:search_api_solr']['solr_devel:debugging']['solr_devel:debugging_results_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display Solr results'),
      '#default_value' => variable_get('solr_devel:debugging_results_enabled', FALSE),
    );

    $form['solr_devel:search_api_solr']['solr_devel:debugging']['solr_devel:debugging_results_process_time'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display Solr result processing time'),
      '#default_value' => variable_get('solr_devel:debugging_results_process_time', FALSE),
    );

    $form['solr_devel:search_api_solr']['solr_devel:debugging']['solr_devel:debugging_result_boosts_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display Solr result boosts'),
      '#default_value' => variable_get('solr_devel:debugging_result_boosts_enabled', FALSE),
    );

    $form['solr_devel:search_api_solr']['solr_devel:debugging']['solr_devel:debugging_result_boosts_human_readable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display Solr result boosts in human readable form'),
      '#default_value' => variable_get('solr_devel:debugging_result_boosts_human_readable', FALSE),
    );

    $form['solr_devel:search_api_solr']['solr_devel:debugging']['solr_devel:debugging_result_boosts_expand'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display Solr result boosts expanded'),
      '#default_value' => variable_get('solr_devel:debugging_result_boosts_expand', FALSE),
    );

    $form['solr_devel:search_api_solr']['solr_devel:debugging']['solr_devel:debugging_result_spellcheck_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display Solr result spellchecker data'),
      '#default_value' => variable_get('solr_devel:debugging_result_spellcheck_enabled', FALSE),
    );

    $form['solr_devel:search_api_solr']['solr_devel:debugging']['solr_devel:debugging_result_full_response_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display Solr result full response'),
      '#default_value' => variable_get('solr_devel:debugging_result_full_response_enabled', FALSE),
    );

    $form['solr_devel:search_api_solr']['solr_devel:logging'] = array(
      '#type' => 'fieldset',
      '#title' => t('Logging'),
      '#description' => t('Watchdog logging.'),
    );

    $form['solr_devel:search_api_solr']['solr_devel:logging']['solr_devel:logging_queries_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Log Solr queries'),
      '#default_value' => variable_get('solr_devel:logging_queries_enabled', FALSE),
    );

    $form['solr_devel:search_api_solr']['solr_devel:logging']['solr_devel:logging_responses_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Log Solr responses'),
      '#default_value' => variable_get('solr_devel:logging_responses_enabled', FALSE),
    );
  }

  if (!module_exists('apachesolr')) {
    // User didn't enable search_api_solr module.
    $form['solr_devel:apachesolr'] = array(
      '#markup' => t('<b>Solr Devel</b> supports !module module debugging, please enable it to see its settings.', array(
        '!module' => l(t('apachesolr'), 'https://drupal.org/project/apachesolr'),
      )),
      '#prefix' => '<p>',
      '#suffix' => '</p>',
    );
  }

  if (!module_exists('search_api_solr')) {
    // User didn't enable search_api_solr module.
    $form['solr_devel:search_api_solr'] = array(
      '#markup' => t('<b>Solr Devel</b> supports !module module debugging, please enable it to see its settings..', array(
        '!module' => l(t('search_api_solr'), 'https://drupal.org/project/search_api_solr'),
      )),
      '#prefix' => '<p>',
      '#suffix' => '</p>',
    );
  }

  $form = system_settings_form($form);

  if (!module_exists('apachesolr') && !module_exists('search_api_solr')) {
    // None of the supported modules has been enabled, we don't want the
    // "Save configuration" button as it's useless.
    unset($form['actions']['submit']);
  }

  return $form;
}
