<?php
function minifield_field_edit($cid, $id){
  drupal_add_js('misc/collapse.js');
  if(arg(6) == "submit"){
    return minifield_field_edit_submit($cid, $id);;
  }
  $mf = new MiniField($cid, $id);
  $arrFields = $mf->arrFields;
  $arrFields_field = array();
  $arrFields_field_list = array('field_type', 'field_size', 'field_notnull', 'field_default', 'field_length', 'field_unsigned', 'field_description');
  foreach($arrFields as $key=>$Field){
    if(in_array($Field->fieldname, $arrFields_field_list)){
      array_push($arrFields_field, $arrFields[$key]);
      unset($arrFields[$key]);
    }
  }
  $arrFields_editor = array();
  $arrFields_editor_list = array('editor_shown', 'editor_style', 'editor_help', 'editor_values_way', 'editor_values_separator', 'editor_values_group', 'editor_values', 'editor_validate', 'editor_filevalidate', 'editor_regexp', 'editor_regexp_error');
  foreach($arrFields as $key=>$Field){
    if(in_array($Field->fieldname, $arrFields_editor_list)){
      array_push($arrFields_editor, $arrFields[$key]);
      unset($arrFields[$key]);
    }
  }
  $variables = array(
    'arrFields' => $arrFields,
    'arrFields_field' => $arrFields_field,
    'arrFields_editor' => $arrFields_editor,
    'Data' => MiniFieldData::FormatData((object)($mf->FieldModel), $mf->arrFields),
    'action' => base_path().MINIFIELD_MENU_PATH."/field/$cid/$id/edit/submit"
  );
  $filepath = MINIFIELD_MODULE_PATH."/tpl/minifield-field-save.tpl.php";
  return MiniFieldCommon::Render($filepath, $variables);
}

function minifield_field_edit_submit($cid, $id){
  $mf = new MiniField($cid, $id);
  $result = $mf->SaveDataToDB();
  if(is_array($result)){
    foreach($result as $value){
      drupal_set_message($value, 'error');
    }
    $arrFields = $mf->arrFields;
    $arrFields_field = array();
    $arrFields_field_list = array('field_type', 'field_size', 'field_notnull', 'field_default', 'field_length', 'field_unsigned', 'field_description');
    foreach($arrFields as $key=>$Field){
      if(in_array($Field->fieldname, $arrFields_field_list)){
        array_push($arrFields_field, $arrFields[$key]);
        unset($arrFields[$key]);
      }
    }
    $arrFields_editor = array();
    $arrFields_editor_list = array('editor_shown', 'editor_style', 'editor_help', 'editor_values_way', 'editor_values_separator', 'editor_values_group', 'editor_values', 'editor_validate',  'editor_filevalidate', 'editor_regexp', 'editor_regexp_error');
    foreach($arrFields as $key=>$Field){
      if(in_array($Field->fieldname, $arrFields_editor_list)){
        array_push($arrFields_editor, $arrFields[$key]);
        unset($arrFields[$key]);
      }
    }
    $variables = array(
      'arrFields' => $arrFields,
      'arrFields_field' => $arrFields_field,
      'arrFields_editor' => $arrFields_editor,
      'Data' => MiniFieldData::FormatData((object)($mf->FieldModel), $mf->arrFields),
      'action' => base_path().MINIFIELD_MENU_PATH."/field/$cid/$id/edit/submit"
    );
    $filepath = MINIFIELD_MODULE_PATH."/tpl/minifield-field-save.tpl.php";
    return MiniFieldCommon::Render($filepath, $variables);
  }else{
    drupal_goto(MINIFIELD_MENU_PATH."/field/$cid/list");
  }
}