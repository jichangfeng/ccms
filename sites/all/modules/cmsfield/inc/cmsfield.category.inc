<?php
/**
 *
 * @param type $handle handle style (list, add, edit, delete)
 * @param type $id Category Identification number
 * @return type 
 */
function cmsfield_category($handle, $id = 0){
  $arrHandles = array('list', 'add', 'edit', 'delete');
  if(!in_array($handle, $arrHandles)){
    return $handle . ' 处理方式未定义！';
  }
  unset ($arrHandles);
  $filepath = "cmsfield.category.$handle.inc";
  if(!file_exists(CMSFIELD_MODULE_PATH . "/inc/" . $filepath)){
    return CMSFIELD_MODULE_PATH . "/inc/" . $filepath . ' 文件未找到！';
  }
  include $filepath;
  $function = "cmsfield_category_" . $handle;
  if(function_exists($function)){
    return $id == 0 ? $function() : $function($id);
  }else{
    return $handle . ' 处理函数未定义！';
  }
}