<?php
function cmsfield_field_delete($cid, $id){
  if(arg(5) && arg(5) == 'confirm'){
    MiniField::DeleteDataByCidId($cid, $id, true);
    drupal_goto("cmsfield/field/$cid/list");
  }else{
    $output = '';
    $output .= '<form action="'.base_path().'cmsfield/field/'.$cid.'/'.$id.'/delete/confirm" method="post">';
    $output .= '<div>Would you like to continue with deleting the field?</div>';
    $output .= '<div id="edit-actions" class="form-actions form-wrapper">';
    $output .= '<input id="edit-submit" class="form-submit" type="submit" value="Delete"/>';
    $output .= '<a id="edit-cancel" href="'.base_path().'cmsfield/field/'.$cid.'/list">Cancel</a>';
    $output .= '</div>';
    $output .= '</form>';
    return $output;
  }
}