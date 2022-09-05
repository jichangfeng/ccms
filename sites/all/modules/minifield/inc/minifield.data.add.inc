<?php
function minifield_data_add($cid){
  if(arg(5) == "submit"){
    return minifield_data_add_submit($cid);
  }
  $mfd = new MiniFieldData($cid);
  $variables = array(
    'cid' => $cid,
    'arrFields' => $mfd->arrFields,
    'Data' => MiniFieldData::FormatData((object)($mfd->FieldModel), $mfd->arrFields),
    'action' => base_path().MINIFIELD_MENU_PATH."/data/$cid/add/submit"
  );
  $filepath = MINIFIELD_MODULE_PATH."/tpl/minifield-data-save.tpl.php";
  return MiniFieldCommon::Render($filepath, $variables);
}

function minifield_data_add_submit($cid){
  $mfd = new MiniFieldData($cid);
  $result = $mfd->SaveDataToDB();
  if(is_array($result)){
    foreach($result as $value){
      drupal_set_message($value, 'error');
    }
    $variables = array(
      'cid' => $cid,
      'arrFields' => $mfd->arrFields,
      'Data' => MiniFieldData::FormatData((object)($mfd->FieldModel), $mfd->arrFields),
      'action' => base_path().MINIFIELD_MENU_PATH."/data/$cid/add/submit"
    );
    $filepath = MINIFIELD_MODULE_PATH."/tpl/minifield-data-save.tpl.php";
    return MiniFieldCommon::Render($filepath, $variables);
  }else{
    drupal_goto(MINIFIELD_MENU_PATH."/data/$cid/list");
  }
}