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
    t('Object ID'),
    t('DSID'),
    t('Find'),
    t('Replace'),
    t('Was regex'),
  );
  $rows = array();
  foreach ($operations as $operation) {
    $objectIDs = '';
    $revisions = unserialize($operation->revisions);
    if (is_array($revisions) || is_object($revisions)){
      foreach($revisions['success'] as $key => $value){  
        $objectIDs = $objectIDs .' <p> ' . $key . '</p>';
      }
    }
    $rows[] = array(
      l($operation->id, 'admin/islandora/tools/find-replace/log/' . $operation->id),
      check_plain($operation->cmodel),
      $objectIDs,
      check_plain($operation->dsid),
      check_plain($operation->find),
      check_plain($operation->replacement),
      check_plain($operation->isregex) == 1 ? 'YES' : 'NO',
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

function generate_csv(){
  drupal_add_http_header('Content-Type', 'text/csv;charset=utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment; filename=find_replace_success_log.csv');
  drupal_add_http_header('Content-Transfer-Encoding', 'binary');
  drupal_add_http_header('Pragma', 'no-cache');
  drupal_add_http_header('Expires', '0');
  // Instead of writing to a file, we write to the output stream.
  $fh = fopen('php://output', 'w');
  $operations = islandora_find_replace_get_submissions();
  // Add a header row
  fputs($fh, $bom =( chr(0xEF) . chr(0xBB) . chr(0xBF) ));
  fputcsv($fh, array('ID','Content Model','Object ID','DSID','Find','Replace'));
 
  foreach($operations as $operation) {
    $objectID = '';
    $revisions = unserialize($operation->revisions);
    if (is_array($revisions) || is_object($revisions)){
     
      $numOfItem = count($revisions['success']);
      //$counter = 0;
      foreach($revisions['success'] as $key => $value){ 
        /*$objectIDs = $objectIDs . $key;
        if($counter < $numOfItem - 1){
          $objectIDs = $objectIDs."\r\n";
          $counter++;
        } */
        $objectID = $key;
        fputcsv($fh, array(
          check_plain($operation->id),
          check_plain($operation->cmodel),
          $objectID,
          check_plain($operation->dsid),
          $operation->find,
          $operation->replacement
        ));
      }
    }
    
  }
  // Close the output stream.
  fclose($fh);
}