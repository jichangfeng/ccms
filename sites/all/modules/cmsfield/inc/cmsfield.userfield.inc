<?php
function cmsfield_userfield_structure(){
  $CategoryGroups = array();
  $arrValueArrays = MiniFieldRepository::GroupTags();
  foreach($arrValueArrays as $ValueArray){
    if($ValueArray[0] == 'user'){
      $CategoryGroups[] = array(
        'ValueArray' => $ValueArray,
        'arrCategories' => MiniFieldCategory::FilterDatasByCid(null, array(), array(), array(array('grouptag', $ValueArray[0])), array(array('weight', 'ASC')), null, true)
      );
      break;
    }
  }
  $variables = array('CategoryGroups' => $CategoryGroups);
  $filepath = CMSFIELD_MODULE_PATH."/tpl/cmsfield-userfield-structure.tpl.php";
  return MiniFieldCommon::Render($filepath, $variables);
}

function cmsfield_userfield_list($cid){
  $Category = MiniFieldCategory::LoadDataByCidId(null, $cid, false);
  if($Category->grouptag != 'user'){ return ''; }
  $arrFields = MiniFieldStructure::arrFields();
  foreach($arrFields as $key=>$Field){
    if($Field->enabled != 1 || $Field->spotlight != 1){
      unset($arrFields[$key]);
    }
  }
  $arrDatas = MiniField::LoadDatasByCid($cid, 20, true);
  $variables = array(
    'Category' => MiniFieldCategory::LoadDataByCidId(null, $cid, true),
    'arrFields' => $arrFields,
    'arrDatas' => $arrDatas
  );
  $filepath = CMSFIELD_MODULE_PATH."/tpl/cmsfield-userfield-list.tpl.php";
  return MiniFieldCommon::Render($filepath, $variables);
}

function cmsfield_userfield_edit($cid, $id){
  $Category = MiniFieldCategory::LoadDataByCidId(null, $cid, false);
  if($Category->grouptag != 'user'){ return ''; }
  $Field = MiniField::LoadDataByCidId($cid, $id, false);
  if($Field->fieldname == 'userid'){ return ''; }
  if(arg(5) == "submit"){
    return cmsfield_userfield_edit_submit($cid, $id);
  }
  $mf = new MiniField($cid, $id);
  $arrFields = $mf->arrFields;
  $denyFieldnames = array();
  $denyFieldnames = array_merge($denyFieldnames, array('cid', 'fieldname', 'editor', 'enabled', 'required', 'spotlight'));
  $denyFieldnames = array_merge($denyFieldnames, array('field_type', 'field_size', 'field_notnull', 'field_default', 'field_length', 'field_unsigned', 'field_description'));
  $denyFieldnames = array_merge($denyFieldnames, array('editor_shown', 'editor_validate', 'editor_filevalidate', 'editor_regexp', 'editor_regexp_error'));
  foreach($arrFields as $key=>$Field){
    if(in_array($Field->fieldname, $denyFieldnames)){
      unset($arrFields[$key]);
    }
  }
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
    'Category' => MiniFieldCategory::LoadDataByCidId(null, $cid, true),
    'arrFields' => $arrFields,
    'arrFields_field' => $arrFields_field,
    'arrFields_editor' => $arrFields_editor,
    'Data' => MiniFieldData::FormatData((object)($mf->FieldModel), $mf->arrFields),
    'action' => base_path()."cmsfield/userfield/$cid/$id/edit/submit"
  );
  $filepath = CMSFIELD_MODULE_PATH."/tpl/cmsfield-userfield-save.tpl.php";
  return MiniFieldCommon::Render($filepath, $variables);
}

function cmsfield_userfield_edit_submit($cid, $id){
  $mf = new MiniField($cid, $id);
  $arrFields = $mf->arrFields;
  $denyFieldnames = array();
  $denyFieldnames = array_merge($denyFieldnames, array('cid', 'fieldname', 'editor', 'enabled', 'required', 'spotlight'));
  $denyFieldnames = array_merge($denyFieldnames, array('field_type', 'field_size', 'field_notnull', 'field_default', 'field_length', 'field_unsigned', 'field_description'));
  $denyFieldnames = array_merge($denyFieldnames, array('editor_shown', 'editor_validate', 'editor_filevalidate', 'editor_regexp', 'editor_regexp_error'));
  foreach($arrFields as $key=>$Field){
    if(in_array($Field->fieldname, $denyFieldnames)){
      unset($arrFields[$key]);
      if(isset($_POST[$Field->fieldname])){ unset($_POST[$Field->fieldname]); }
    }
  }
  $result = $mf->SaveDataToDB();
  if(is_array($result)){
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
      'Category' => MiniFieldCategory::LoadDataByCidId(null, $cid, true),
      'arrFields' => $arrFields,
      'arrFields_field' => $arrFields_field,
      'arrFields_editor' => $arrFields_editor,
      'Data' => MiniFieldData::FormatData((object)($mf->FieldModel), $mf->arrFields),
      'action' => base_path()."cmsfield/userfield/$cid/$id/edit/submit",
      'errors' => $result
    );
    $filepath = CMSFIELD_MODULE_PATH."/tpl/cmsfield-userfield-save.tpl.php";
    return MiniFieldCommon::Render($filepath, $variables);
  }else{
    drupal_goto("cmsfield/userfield/$cid/list");
  }
}