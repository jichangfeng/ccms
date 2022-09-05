<?php
function cmsdata_delete($cid, $id){
  if(arg(4) && arg(4) == 'confirm'){
    if($cid == CMSDATA_CID_NAVIGATION){
      CMSNavigation::DeleteNavigationById($id);
    }else{
      MiniFieldData::DeleteDataByCidId($cid, $id, true);
    }
    drupal_goto("cmsdata/$cid/list");
  }else{
    $output = '';
    $output .= '<form action="'.base_path().'cmsdata/'.$cid.'/'.$id.'/delete/confirm" method="post">';
    $output .= '<div>Would you like to continue with deleting the data?</div>';
    $output .= '<div id="edit-actions" class="form-actions form-wrapper">';
    $output .= '<input id="edit-submit" class="form-submit" type="submit" value="Delete"/>';
    $output .= '<a id="edit-cancel" href="'.base_path().'cmsdata/'.$cid.'/list">Cancel</a>';
    $output .= '</div>';
    $output .= '</form>';
    return $output;
  }
}