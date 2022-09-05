<?php
function cmsfield_field_add($cid){
  if(arg(4) == "submit"){
    return cmsfield_field_add_submit($cid);
  }else if(arg(4) == "userbasic"){
    return cmsfield_field_add_userbasic($cid);
  }else if(arg(4) == "contentbasic"){
    return cmsfield_field_add_contentbasic($cid);
  }
  $mf = new MiniField($cid);
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
    'Category' => MiniFieldCategory::LoadDataByCidId(null, $cid, true),
    'arrFields' => $arrFields,
    'arrFields_field' => $arrFields_field,
    'arrFields_editor' => $arrFields_editor,
    'Data' => MiniFieldData::FormatData((object)($mf->FieldModel), $mf->arrFields),
    'action' => base_path()."cmsfield/field/$cid/add/submit"
  );
  $filepath = CMSFIELD_MODULE_PATH."/tpl/cmsfield-field-save.tpl.php";
  return MiniFieldCommon::Render($filepath, $variables);
}

function cmsfield_field_add_submit($cid){
  $mf = new MiniField($cid);
  $result = $mf->SaveDataToDB();
  if(is_array($result)){
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
      'Category' => MiniFieldCategory::LoadDataByCidId(null, $cid, true),
      'arrFields' => $arrFields,
      'arrFields_field' => $arrFields_field,
      'arrFields_editor' => $arrFields_editor,
      'Data' => MiniFieldData::FormatData((object)($mf->FieldModel), $mf->arrFields),
      'action' => base_path()."cmsfield/field/$cid/add/submit",
      'errors' => $result
    );
    $filepath = CMSFIELD_MODULE_PATH."/tpl/cmsfield-field-save.tpl.php";
    return MiniFieldCommon::Render($filepath, $variables);
  }else{
    drupal_goto("cmsfield/field/$cid/list");
  }
}

function cmsfield_field_add_userbasic($cid){
  $Category = MiniFieldCategory::LoadDataByCidId(null, $cid, false);
  if($Category->grouptag != 'user'){ drupal_goto("cmsfield/field/$cid/list"); }
  $_POST = array(
    'id' => 0, 'cid' => $cid, 'name' => '用户标识', 'fieldname' => 'userid',
    'switchofeditor' => '', 'editor' => 'hidden',
    'switchofenabled' => '', 'enabled' => 1,
    'switchofrequired' => '', 'required' => 1,
    'switchofspotlight' => '', 'spotlight' => 0,
    'switchofweight' => '', 'weight' => 1,
    'switchoffield_type' => '', 'field_type' => 'int',
    'switchoffield_size' => '', 'field_size' => 'normal',
    'switchoffield_notnull' => '', 'field_notnull' => 0,
    'field_default' => 0, 'field_length' => 50,
    'switchoffield_unsigned' => '', 'field_unsigned' => 0,
    'field_description' => '',
    'switchofeditor_shown' => '', 'editor_shown' => 0,
    'editor_style' => '', 'editor_help' => '',
    'switchofeditor_values_way' => '', 'editor_values_way' => 'enumerate',
    'editor_values_separator' => '|', 'editor_values_group' => '', 'editor_values' => '',
    'editor_validate' => '', 'editor_filevalidate' => '',
    'switchofeditor_regexp' => '', 'editor_regexp' => '/^[0-9]*[1-9][0-9]*$/',
    'editor_regexp_error' => '用户标识值错误'
  );
  $mf = new MiniField($cid);
  $mf->SaveDataToDB();
  drupal_goto("cmsfield/field/$cid/list");
}

function cmsfield_field_add_contentbasic($cid){
  $Category = MiniFieldCategory::LoadDataByCidId(null, $cid, false);
  if($Category->grouptag != 'content'){ drupal_goto("cmsfield/field/$cid/list"); }
  $_POST = array(
    'id' => 0, 'cid' => $cid, 'name' => '栏目', 'fieldname' => 'navid',
    'switchofeditor' => '', 'editor' => 'hidden',
    'switchofenabled' => '', 'enabled' => 1,
    'switchofrequired' => '', 'required' => 1,
    'switchofspotlight' => '', 'spotlight' => 0,
    'switchofweight' => '', 'weight' => 1,
    'switchoffield_type' => '', 'field_type' => 'int',
    'switchoffield_size' => '', 'field_size' => 'normal',
    'switchoffield_notnull' => '', 'field_notnull' => 0,
    'field_default' => 0, 'field_length' => 50,
    'switchoffield_unsigned' => '', 'field_unsigned' => 0,
    'field_description' => '',
    'switchofeditor_shown' => '', 'editor_shown' => 0,
    'editor_style' => '', 'editor_help' => '',
    'switchofeditor_values_way' => '', 'editor_values_way' => 'enumerate',
    'editor_values_separator' => '|', 'editor_values_group' => '', 'editor_values' => '',
    'editor_validate' => '', 'editor_filevalidate' => '',
    'switchofeditor_regexp' => '', 'editor_regexp' => '/^[0-9]*[1-9][0-9]*$/',
    'editor_regexp_error' => '栏目输入错误'
  );
  $mf = new MiniField($cid);
  $mf->SaveDataToDB();
  $_POST = array(
    'id' => 0, 'cid' => $cid, 'name' => '用户', 'fieldname' => 'userid',
    'switchofeditor' => '', 'editor' => 'hidden',
    'switchofenabled' => '', 'enabled' => 1,
    'switchofrequired' => '', 'required' => 1,
    'switchofspotlight' => '', 'spotlight' => 1,
    'switchofweight' => '', 'weight' => 2,
    'switchoffield_type' => '', 'field_type' => 'int',
    'switchoffield_size' => '', 'field_size' => 'normal',
    'switchoffield_notnull' => '', 'field_notnull' => 0,
    'field_default' => 0, 'field_length' => 50,
    'switchoffield_unsigned' => '', 'field_unsigned' => 0,
    'field_description' => '',
    'switchofeditor_shown' => '', 'editor_shown' => 0,
    'editor_style' => '', 'editor_help' => '',
    'switchofeditor_values_way' => '', 'editor_values_way' => 'enumerate',
    'editor_values_separator' => '|', 'editor_values_group' => '', 'editor_values' => '',
    'editor_validate' => '', 'editor_filevalidate' => '',
    'switchofeditor_regexp' => '', 'editor_regexp' => '/^\d+$/',
    'editor_regexp_error' => '用户输入错误'
  );
  $mf = new MiniField($cid);
  $mf->SaveDataToDB();
  $_POST = array(
    'id' => 0, 'cid' => $cid, 'name' => '创建时间', 'fieldname' => 'created',
    'switchofeditor' => '', 'editor' => 'datepicker',
    'switchofenabled' => '', 'enabled' => 1,
    'switchofrequired' => '', 'required' => 1,
    'switchofspotlight' => '', 'spotlight' => 1,
    'switchofweight' => '', 'weight' => 3,
    'switchoffield_type' => '', 'field_type' => 'int',
    'switchoffield_size' => '', 'field_size' => 'normal',
    'switchoffield_notnull' => '', 'field_notnull' => 0,
    'field_default' => '0', 'field_length' => 50,
    'switchoffield_unsigned' => '', 'field_unsigned' => 0,
    'field_description' => '',
    'switchofeditor_shown' => '', 'editor_shown' => 0,
    'editor_style' => '', 'editor_help' => '',
    'switchofeditor_values_way' => '', 'editor_values_way' => 'enumerate',
    'editor_values_separator' => '|', 'editor_values_group' => '', 'editor_values' => '',
    'editor_validate' => '', 'editor_filevalidate' => '',
    'switchofeditor_regexp' => '', 'editor_regexp' => '/[^\s]+/',
    'editor_regexp_error' => '创建时间不能为空'
  );
  $mf = new MiniField($cid);
  $mf->SaveDataToDB();
  $_POST = array(
    'id' => 0, 'cid' => $cid, 'name' => '状态', 'fieldname' => 'status',
    'switchofeditor' => '', 'editor' => 'radio',
    'switchofenabled' => '', 'enabled' => 1,
    'switchofrequired' => '', 'required' => 1,
    'switchofspotlight' => '', 'spotlight' => 1,
    'switchofweight' => '', 'weight' => 4,
    'switchoffield_type' => '', 'field_type' => 'varchar',
    'switchoffield_size' => '', 'field_size' => 'normal',
    'switchoffield_notnull' => '', 'field_notnull' => 0,
    'field_default' => '', 'field_length' => 50,
    'switchoffield_unsigned' => '', 'field_unsigned' => 0,
    'field_description' => '',
    'switchofeditor_shown' => '', 'editor_shown' => 1,
    'editor_style' => '', 'editor_help' => '',
    'switchofeditor_values_way' => '', 'editor_values_way' => 'dynamic',
    'editor_values_separator' => '|', 'editor_values_group' => '', 'editor_values' => 'CMSNodeField|GetValueArraysOfStatus',
    'editor_validate' => '', 'editor_filevalidate' => '',
    'switchofeditor_regexp' => '', 'editor_regexp' => '/[^\s]+/',
    'editor_regexp_error' => '状态必须选择'
  );
  $mf = new MiniField($cid);
  $mf->SaveDataToDB();
  drupal_goto("cmsfield/field/$cid/list");
}