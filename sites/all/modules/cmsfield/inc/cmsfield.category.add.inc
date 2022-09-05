<?php
function cmsfield_category_add(){
  if(arg(3) == "submit"){
    return cmsfield_category_add_submit();
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
    'Category' => MiniFieldCategoryStructure::Category(),
    'arrFields' => $arrFields,
    'arrFields_table' => $arrFields_table,
    'Data' => MiniFieldData::FormatData((object)($mfc->FieldModel), $mfc->arrFields),
    'action' => base_path()."cmsfield/category/add/submit"
  );
  $filepath = CMSFIELD_MODULE_PATH."/tpl/cmsfield-category-save.tpl.php";
  return MiniFieldCommon::Render($filepath, $variables);
}

function cmsfield_category_add_submit(){
  $mfc = new MiniFieldCategory(null);
  $result = $mfc->SaveDataToDB();
  if(is_array($result)){
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
      'Category' => MiniFieldCategoryStructure::Category(),
      'arrFields' => $arrFields,
      'arrFields_table' => $arrFields_table,
      'Data' => MiniFieldData::FormatData((object)($mfc->FieldModel), $mfc->arrFields),
      'action' => base_path()."cmsfield/category/add/submit",
      'errors' => $result
    );
    $filepath = CMSFIELD_MODULE_PATH."/tpl/cmsfield-category-save.tpl.php";
    return MiniFieldCommon::Render($filepath, $variables);
  }else{
    drupal_goto('cmsfield/category/list');
  }
}