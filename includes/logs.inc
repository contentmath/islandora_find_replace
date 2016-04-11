<?php

/**
 * @file
 * A list of Find & Replace form submissions and states.
 */

/**
 * Render a table of find & replace operations.
 *
 * @return array
 *   The render array of output.
 */
function islandora_find_replace_operation_logs() {
  $operations = islandora_find_replace_get_submissions();

  $header = array(
    t('ID'),
    t('Content Model'),
    t('DSID'),
    t('Find'),
    t('Replace'),
  );
  $rows = array();
  foreach ($operations as $operation) {
    $rows[] = array(
      l($operation->id, 'admin/islandora/tools/find-replace/log/' . $operation->id),
      check_plain($operation->cmodel),
      check_plain($operation->dsid),
      check_plain($operation->find),
      check_plain($operation->replacement),
    );
  }

  return array(
    '#type' => 'markup',
    '#markup' => theme('table', array('header' => $header, 'rows' => $rows)),
  );
}

/**
 * Return all find & replace submissions.
 *
 * @return mixed
 *   Database rows.
 */
function islandora_find_replace_get_submissions() {
  return db_query("SELECT * FROM {islandora_find_replace} WHERE state = 'complete'")->fetchAll();
}
