<?php
function minifield_data_delete($cid, $id){
  if(arg(6) && arg(6) == 'confirm'){
    MiniFieldData::DeleteDataByCidId($cid, $id, true);
    drupal_goto(MINIFIELD_MENU_PATH."/data/$cid/list");
  }else{
    $output = '';
    $output .= '<form action="'.base_path().MINIFIELD_MENU_PATH.'/data/'.$cid.'/'.$id.'/delete/confirm" method="post">';
    $output .= '<div>Would you like to continue with deleting the data?</div>';
    $output .= '<div id="edit-actions" class="form-actions form-wrapper">';
    $output .= '<input id="edit-submit" class="form-submit" type="submit" value="Delete"/>';
    $output .= '<a id="edit-cancel" href="'.base_path().MINIFIELD_MENU_PATH.'/data/'.$cid.'/list">Cancel</a>';
    $output .= '</div>';
    $output .= '</form>';
    return $output;
  }
}