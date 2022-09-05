<?php
function cmsnode_delete($navid, $id){
  if(arg(4) && arg(4) == 'confirm'){
    $Navigation = CMSNavigation::LoadNavigationById($navid, true);
    if($Navigation->categoryid[0][0] <= 0){return '此栏目未选择内容结构';}
    $ManageContent = CMSNavigation::CanManageContent('delete', $GLOBALS['user'], $Navigation, $id);
    if($ManageContent == 0){ drupal_access_denied(); }
    CMSNode::DeleteNodeByNavidId($navid, $id);
    $options = array();
    if(isset($_GET['page'])){
      $options['query'] = array();
      $options['query']['page'] = $_GET['page'];
    }
    drupal_goto("cmsnode/$navid/list", $options);
  }else{
    $output = '';
    $output .= '<form action="'.base_path().'cmsnode/'.$navid.'/'.$id.'/delete/confirm" method="post">';
    $output .= '<div>Would you like to continue with deleting the data?</div>';
    $output .= '<div id="edit-actions" class="form-actions form-wrapper">';
    $output .= '<input id="edit-submit" class="form-submit" type="submit" value="Delete"/>';
    $output .= '<a id="edit-cancel" href="'.base_path().'cmsnode/'.$navid.'/list">Cancel</a>';
    $output .= '</div>';
    $output .= '</form>';
    return $output;
  }
}