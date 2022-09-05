<?php
function minifield_field_delete($cid, $id){
  if(arg(6) && arg(6) == 'confirm'){
    MiniField::DeleteDataByCidId($cid, $id, true);
    drupal_goto(MINIFIELD_MENU_PATH."/field/$cid/list");
  }else{
    $output = '';
    $output .= '<form action="'.base_path().MINIFIELD_MENU_PATH.'/field/'.$cid.'/'.$id.'/delete/confirm" method="post">';
    $output .= '<div>Would you like to continue with deleting the field?</div>';
    $output .= '<div id="edit-actions" class="form-actions form-wrapper">';
    $output .= '<input id="edit-submit" class="form-submit" type="submit" value="Delete"/>';
    $output .= '<a id="edit-cancel" href="'.base_path().MINIFIELD_MENU_PATH.'/field/'.$cid.'/list">Cancel</a>';
    $output .= '</div>';
    $output .= '</form>';
    return $output;
  }
}