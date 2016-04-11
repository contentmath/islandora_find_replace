<?php

/**
 * @file
 * Form functions for the form to select which objects to update.
 */

/**
 * Form function to generate the table select for objects to operate on.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 * @param array $find_replace
 *   The find & replace database row.
 *
 * @return array
 *   The populated form array.
 */
function islandora_find_replace_replace_form($form, &$form_state, $find_replace) {
  if ($find_replace['state'] != 'search') {
    drupal_set_message(t('There was a problem loading the submission.'), 'error');
    drupal_goto('admin/islandora/tools/find-replace/find');
  }

  $form_state['storage']['find_replace'] = $find_replace;
  $diff_exists = module_exists('islandora_pretty_text_diff');
  $options = array();
  $dsid = $find_replace['dsid'];

  foreach (unserialize($find_replace['find_results']) as $option) {
    $options[$option['object']] = array(
      'label' => l($option['label'], 'islandora/object/' . $option['object'],
        array('attributes' => array('target' => '_blank'))),
      'datastream' => l($dsid,
        'islandora/object/' . $option['object'] . '/datastream/' . $dsid . '/view',
        array('attributes' => array('target' => '_blank'))
      ),
      'count' => $option['count'],
    );
    if ($diff_exists) {
      $options[$option['object']]['preview'] = l(t('Preview'),
        'admin/islandora/tools/find-replace/replace/' . $find_replace['id'] . '/' . $option['object'] . '/preview',
        array('attributes' => array('target' => '_blank')));
    }
  }

  $header = array(
    'label' => t('Label'),
    'datastream' => t('Current Datastream content'),
    'count' => t('Number of occurrences'),
  );
  if ($diff_exists) {
    $header['preview'] = t('Preview Diff');
  }
  $form['back'] = array(
    '#type' => 'markup',
    '#markup' => l(t('Back (New Search)'), 'admin/islandora/tools/find-replace/find'),
  );
  $form['preview'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => l(t('No matching objects were found. Search again.'),
      'admin/islandora/tools/find-replace/find'),
  );

  if (count($options)) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Replace String'),
    );
  }

  return $form;
}

/**
 * Form submit function for the replace form.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 */
function islandora_find_replace_replace_form_submit($form, &$form_state) {
  $selected = array_keys(array_filter($form_state['values']['preview']));

  $operations = array();
  foreach ($selected as $select) {
    $operations[] = array('islandora_find_replace_update_objects', array(
      $select,
      $form_state['storage']['find_replace']['dsid'],
      $form_state['storage']['find_replace']['find'],
      $form_state['storage']['find_replace']['replacement'],
      $form_state['storage']['find_replace']['id']),
    );
  }
  $batch = array(
    'operations' => $operations,
    'title' => t('Replacing Text'),
    'init_message' => t('Beginning text replacement.'),
    'finished' => 'islandora_find_replace_update_complete',
    'progress_message' => t('Processed @current out of @total.'),
  );
  batch_set($batch);
  $form_state['redirect'] = 'admin/islandora/tools/find-replace/log/' . $form_state['storage']['find_replace']['id'];
}
