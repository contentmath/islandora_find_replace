<?php

/**
 * @file
 * Form functions for the form to find objects to operate on.
 */

/**
 * The form function for the form to find objects to operate on.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 *
 * @return array
 *   The built form array.
 */
function islandora_find_replace_find_form($form, &$form_state) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $form = array();

  $form['find'] = array(
    '#type' => 'fieldset',
    '#title' => t('Find Objects'),
    '#description' => t('For faster results, make your search as specific as possible. Objects must match all criteria that you select.'),
  );

  $content_models = islandora_get_content_models();
  $cmodel_options = array();
  foreach ($content_models as $model) {
    $cmodel_options[$model['pid']] = check_plain($model['label']);
  }

  $form['find']['content_model'] = array(
    '#type' => 'select',
    '#title' => t('Content Model'),
    '#description' => t('Limit the search to objects with the selected content model.'),
    '#options' => $cmodel_options,
    '#required' => TRUE,
  );

  $collections = islandora_basic_collection_get_collections();
  $collection_options = array();
  $collection_options[] = t('- Select -');
  foreach ($collections as $collection) {
    $collection_options[$collection['pid']] = check_plain($collection['label']) . ' - (' . $collection['pid'] . ')';
  }
  $form['find']['collection'] = array(
    '#type' => 'select',
    '#title' => t('Collection'),
    '#description' => t('Limit the search to objects directly in the selected collection.'),
    '#options' => $collection_options,
  );

  $form['find']['date'] = array(
    '#type' => 'fieldset',
    '#title' => t('Date Condition'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['find']['date']['date_property'] = array(
    '#type' => 'select',
    '#title' => t('Date Condition'),
    '#options' => array(
      '' => t('- Select -'),
      'createdDate' => t('Date Created'),
      'lastModifiedDate' => t('Date Modified'),
    ),
    '#description' => t('To limit by date, select a date property.'),
  );

  $form['find']['date']['date_from'] = array(
    '#type' => 'date',
    '#title' => t('Date From'),
    '#description' => t('The start date for the date property selected above (inclusive).'),
  );
  $form['find']['date']['date_to'] = array(
    '#type' => 'date',
    '#title' => t('Date To'),
    '#description' => t('The end date for the date property selected above (inclusive).'),
  );

  $form['dsid'] = array(
    '#type' => 'textfield',
    '#title' => t('Datastream ID'),
    '#description' => t('A single datastream ID (DSID) to search for the string.'),
    '#required' => TRUE,
  );

  $form['search'] = array(
    '#type' => 'textarea',
    '#title' => t('Search text'),
    '#description' => t('The text to search for in the datastream.'),
    '#required' => TRUE,
    '#rows' => 1,
  );

  $form['replace'] = array(
    '#type' => 'textarea',
    '#title' => t('Replacement text'),
    '#description' => t('The text to replace the search with. Use (empty) to replace the search text with nothing.'),
    '#required' => TRUE,
    '#rows' => 1,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Find objects'),
  );

  return $form;
}

/**
 * Find form validation function.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 */
function islandora_find_replace_find_form_validate($form, &$form_state) {
  $val = $form_state['values'];
  $disallowed_dsids = array(
    'RELS-EXT',
    'RELS-INT',
    'POLICY',
    'COLLECTION_POLICY',
  );
  if (in_array(strtoupper(trim($val['dsid'])), $disallowed_dsids)) {
    form_set_error('dsid', t('Modifications are not permitted for the selected Datastream ID.'));
  }
  if ($val['date_property']) {
    $from = _islandora_find_replace_timestamp_from_form_date($val['date_from']);
    $to = _islandora_find_replace_timestamp_from_form_date($val['date_to'], FALSE);
    if ($from > $to) {
      form_set_error('date_from', t('The Date From cannot be after the Date To.'));
    }
  }
}

/**
 * Find form submit function.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 */
function islandora_find_replace_find_form_submit($form, &$form_state) {
  $candidates = islandora_find_replace_query($form_state['values']['content_model'],
    $form_state['values']['collection'],
    $form_state['values']['date_property'],
    $form_state['values']['date_from'],
    $form_state['values']['date_to']
  );

  $find_replace = array(
    'cmodel' => $form_state['values']['content_model'],
    'dsid' => $form_state['values']['dsid'],
    'find' => $form_state['values']['search'],
    'replacement' => $form_state['values']['replace'],
    'state' => 'submitted',
  );
  drupal_write_record('islandora_find_replace', $find_replace);

  $operations = array();
  foreach ($candidates as $candidate) {
    $operations[] = array(
      'islandora_find_replace_get_targets',
      array(
        $candidate,
        $form_state['values']['dsid'],
        $form_state['values']['search'],
        $find_replace['id'],
      ),
    );
  }

  $batch = array(
    'operations' => $operations,
    'title' => t('Finding Matching Objects'),
    'init_message' => t('Beginning search for matching objects.'),
    'finished' => 'islandora_find_replace_find_complete',
    'progress_message' => t('Processed @current out of @total.'),
  );

  batch_set($batch);
  $form_state['redirect'] = 'admin/islandora/tools/find-replace/replace/' . $find_replace['id'];
}
