<?php
function cmsuser_user_del($rid, $uid){
  $Role = CMSRole::LoadRoleByRid($rid, true);
  if($Role->categoryid[0][0] <= 0){return '此角色未选择用户结构';}
  if(CMSRole::CanManageUser('delete', $GLOBALS['user'], $Role) == 0){ drupal_access_denied(); }
  if(arg(5) && arg(5) == 'confirm'){
    CMSUser::DeleteUserByRidUid($rid, $uid);
    drupal_goto("cmsuser/user/$rid/list");
  }else{
    $output = '';
    $output .= '<form action="'.base_path().'cmsuser/user/'.$rid.'/'.$uid.'/del/confirm" method="post">';
    $output .= '<div>Would you like to continue with deleting the data?</div>';
    $output .= '<div id="edit-actions" class="form-actions form-wrapper">';
    $output .= '<input id="edit-submit" class="form-submit" type="submit" value="Delete"/>';
    $output .= '<a id="edit-cancel" href="'.base_path().'cmsuser/user/'.$rid.'/list">Cancel</a>';
    $output .= '</div>';
    $output .= '</form>';
    return $output;
  }
}