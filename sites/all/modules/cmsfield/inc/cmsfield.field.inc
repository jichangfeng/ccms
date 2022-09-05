<?php
function cmsfield_field_structure(){
  $CategoryGroups = array();
  $arrValueArrays = MiniFieldRepository::GroupTags();
  foreach($arrValueArrays as $ValueArray){
    $CategoryGroups[] = array(
      'ValueArray' => $ValueArray,
      'arrCategories' => MiniFieldCategory::FilterDatasByCid(null, array(), array(), array(array('grouptag', $ValueArray[0])), array(array('weight', 'ASC')), null, true)
    );
  }
  $variables = array('CategoryGroups' => $CategoryGroups);
  $filepath = CMSFIELD_MODULE_PATH."/tpl/cmsfield-field-structure.tpl.php";
  return MiniFieldCommon::Render($filepath, $variables);
}

/**
 *
 * @param type $handle handle style (list, add, edit, delete)
 * @param type $cid Category Identification number
 * @param type $id Field Identification number
 * @return type
 */
function cmsfield_field($handle, $cid, $id = 0){
  $arrHandles = array('list', 'add', 'edit', 'delete');
  if(!in_array($handle, $arrHandles)){
    return $handle . ' 处理方式未定义！';
  }
  unset ($arrHandles);
  $filepath = "cmsfield.field.$handle.inc";
  if(!file_exists(CMSFIELD_MODULE_PATH . "/inc/" . $filepath)){
    return CMSFIELD_MODULE_PATH . "/inc/" . $filepath . ' 文件未找到！';
  }
  include $filepath;
  $function = "cmsfield_field_" . $handle;
  if(function_exists($function)){
    return $id == 0 ? $function($cid) : $function($cid, $id);
  }else{
    return $handle . ' 处理函数未定义！';
  }
}