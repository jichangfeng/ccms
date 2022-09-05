<?php
function minifield_category_delete($id){
  if(arg(5) && arg(5) == 'confirm'){
    MiniFieldCategory::DeleteDataByCidId(null, $id, true);
    drupal_goto(MINIFIELD_MENU_PATH.'/category/list');
  }else{
    $output = '';
    $output .= '<form action="'.base_path().MINIFIELD_MENU_PATH.'/category/'.$id.'/delete/confirm" method="post">';
    $output .= '<div>Would you like to continue with deleting the category?</div>';
    $output .= '<div id="edit-actions" class="form-actions form-wrapper">';
    $output .= '<input id="edit-submit" class="form-submit" type="submit" value="Delete"/>';
    $output .= '<a id="edit-cancel" href="'.base_path().MINIFIELD_MENU_PATH.'/category/list">Cancel</a>';
    $output .= '</div>';
    $output .= '</form>';
    return $output;
  }
}