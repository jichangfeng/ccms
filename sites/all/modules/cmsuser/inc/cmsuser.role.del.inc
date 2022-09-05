<?php
function cmsuser_role_del($rid){
  if(arg(4) && arg(4) == 'confirm'){
    CMSRole::DeleteRoleByRid($rid);
    drupal_goto("cmsuser/role/list");
  }else{
    $output = '';
    $output .= '<form action="'.base_path().'cmsuser/role/'.$rid.'/del/confirm" method="post">';
    $output .= '<div>Would you like to continue with deleting the data?</div>';
    $output .= '<div id="edit-actions" class="form-actions form-wrapper">';
    $output .= '<input id="edit-submit" class="form-submit" type="submit" value="Delete"/>';
    $output .= '<a id="edit-cancel" href="'.base_path().'cmsuser/role/list">Cancel</a>';
    $output .= '</div>';
    $output .= '</form>';
    return $output;
  }
}