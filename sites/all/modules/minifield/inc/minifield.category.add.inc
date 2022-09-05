<?php
function minifield_category_add(){
  drupal_add_js('misc/collapse.js');
  if(arg(4) == "submit"){
    return minifield_category_add_submit();
  }
  $mfc = new MiniFieldCategory(null);
  $arrFields = $mfc->arrFields;
  $arrFields_table = array();
  $arrFields_table_list = array('table_type', 'table_collation', 'table_description');
  foreach($arrFields as $key=>$Field){
    if(in_array($Field->fieldname, $arrFields_table_list)){
      array_push($arrFields_table, $arrFields[$key]);
      unset($arrFields[$key]);
    }
  }
  $variables = array(
    'arrFields' => $arrFields,
    'arrFields_table' => $arrFields_table,
    'Data' => MiniFieldData::FormatData((object)($mfc->FieldModel), $mfc->arrFields),
    'action' => base_path().MINIFIELD_MENU_PATH."/category/add/submit"
  );
  $filepath = MINIFIELD_MODULE_PATH."/tpl/minifield-category-save.tpl.php";
  return MiniFieldCommon::Render($filepath, $variables);
}

function minifield_category_add_submit(){
  $mfc = new MiniFieldCategory(null);
  $result = $mfc->SaveDataToDB();
  if(is_array($result)){
    foreach($result as $value){
      drupal_set_message($value, 'error');
    }
    $arrFields = $mfc->arrFields;
    $arrFields_table = array();
    $arrFields_table_list = array('table_type', 'table_collation', 'table_description');
    foreach($arrFields as $key=>$Field){
      if(in_array($Field->fieldname, $arrFields_table_list)){
        array_push($arrFields_table, $arrFields[$key]);
        unset($arrFields[$key]);
      }
    }
    $variables = array(
      'arrFields' => $arrFields,
      'arrFields_table' => $arrFields_table,
      'Data' => MiniFieldData::FormatData((object)($mfc->FieldModel), $mfc->arrFields),
      'action' => base_path().MINIFIELD_MENU_PATH."/category/add/submit"
    );
    $filepath = MINIFIELD_MODULE_PATH."/tpl/minifield-category-save.tpl.php";
    return MiniFieldCommon::Render($filepath, $variables);
  }else{
    drupal_goto(MINIFIELD_MENU_PATH.'/category/list');
  }
}