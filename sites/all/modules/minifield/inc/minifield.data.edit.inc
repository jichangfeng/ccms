<?php
function minifield_data_edit($cid, $id){
  if(arg(6) == "submit"){
    return minifield_data_edit_submit($cid, $id);;
  }
  $mfd = new MiniFieldData($cid, $id);
  $variables = array(
    'cid' => $cid,
    'arrFields' => $mfd->arrFields,
    'Data' => MiniFieldData::FormatData((object)($mfd->FieldModel), $mfd->arrFields),
    'action' => base_path().MINIFIELD_MENU_PATH."/data/$cid/$id/edit/submit"
  );
  $filepath = MINIFIELD_MODULE_PATH."/tpl/minifield-data-save.tpl.php";
  return MiniFieldCommon::Render($filepath, $variables);
}

function minifield_data_edit_submit($cid, $id){
  $mfd = new MiniFieldData($cid, $id);
  $result = $mfd->SaveDataToDB();
  if(is_array($result)){
    foreach($result as $value){
      drupal_set_message($value, 'error');
    }
    $variables = array(
      'cid' => $cid,
      'arrFields' => $mfd->arrFields,
      'Data' => MiniFieldData::FormatData((object)($mfd->FieldModel), $mfd->arrFields),
      'action' => base_path().MINIFIELD_MENU_PATH."/data/$cid/$id/edit/submit"
    );
    $filepath = MINIFIELD_MODULE_PATH."/tpl/minifield-data-save.tpl.php";
    return MiniFieldCommon::Render($filepath, $variables);
  }else{
    drupal_goto(MINIFIELD_MENU_PATH."/data/$cid/list");
  }
}