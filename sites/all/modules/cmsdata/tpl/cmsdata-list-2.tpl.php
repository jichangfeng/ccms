<?php
if(isset($arrDatas[0])){
  drupal_goto('cmsdata/'.$Category->id.'/'.$arrDatas[0]->id.'/edit');
}else{
  drupal_goto('cmsdata/'.$Category->id.'/add');
}
?>