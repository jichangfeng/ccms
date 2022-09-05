<?php
function cmsfield_category_delete($id){
  if(arg(4) && arg(4) == 'confirm'){
    MiniFieldCategory::DeleteDataByCidId(null, $id, true);
    drupal_goto('cmsfield/category/list');
  }else{
    $output = '';
    $output .= '<form action="'.base_path().'cmsfield/category/'.$id.'/delete/confirm" method="post">';
    $output .= '<div>Would you like to continue with deleting the category?</div>';
    $output .= '<div id="edit-actions" class="form-actions form-wrapper">';
    $output .= '<input id="edit-submit" class="form-submit" type="submit" value="Delete"/>';
    $output .= '<a id="edit-cancel" href="'.base_path().'cmsfield/category/list">Cancel</a>';
    $output .= '</div>';
    $output .= '</form>';
    return $output;
  }
}