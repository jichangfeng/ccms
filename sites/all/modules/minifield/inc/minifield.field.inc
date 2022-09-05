<?php
/**
 *
 * @param type $handle handle style (list, add, edit, delete)
 * @param type $cid Category Identification number
 * @param type $id Field Identification number
 * @return type
 */
function minifield_field($handle, $cid, $id = 0){
  $arrHandles = array('list', 'add', 'edit', 'delete');
  if(!in_array($handle, $arrHandles)){
    return t($handle . ' 处理方式未定义！');
  }
  unset ($arrHandles);
  $filepath = "minifield.field.$handle.inc";
  if(!file_exists(MINIFIELD_MODULE_PATH . "/inc/" . $filepath)){
    return t(MINIFIELD_MODULE_PATH . "/inc/" . $filepath . ' 文件未找到！');
  }
  include $filepath;
  $function = "minifield_field_" . $handle;
  if(function_exists($function)){
    return $id == 0 ? $function($cid) : $function($cid, $id);
  }else{
    return t($handle . ' 处理函数未定义！');
  }
}

function minifield_field_structure(){
  $arrCategories = MiniFieldCategory::LoadDatasByCid(null);
  $output = '';
  $output .= '<dl class="admin-list">';
  $output .= '<dt>字段结构树（点击查看字段列表）</dt>';
  foreach($arrCategories as $Category){
    $output .= '<dd><a href="' .base_path().MINIFIELD_MENU_PATH.'/field/'.$Category->id.'/list">' .$Category->name. '</a></dd>';
  }
  $output .= '</dl>';
  $output .= '<style>.action-links{display: none;}</style>';
  return $output;
}