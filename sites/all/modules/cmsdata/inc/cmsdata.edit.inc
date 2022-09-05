<?php
function cmsdata_edit($cid, $id){
  if(arg(4) == "submit"){
    return cmsdata_edit_submit($cid, $id);
  }
  $mfd = new MiniFieldData($cid, $id);
  $Data = MiniFieldData::FormatData((object)($mfd->FieldModel), $mfd->arrFields);
  $variables = array(
    'Category' => MiniFieldCategory::LoadDataByCidId(null, $cid, true),
    'arrFields' => $mfd->arrFields,
    'Data' => $Data,
    'action' => base_path()."cmsdata/$cid/$id/edit/submit"
  );
  $filepath = CMSDATA_MODULE_PATH."/tpl/cmsdata-save.tpl.php";
  if(file_exists(CMSDATA_MODULE_PATH."/tpl/cmsdata-save-$cid.tpl.php")){
    $filepath = CMSDATA_MODULE_PATH."/tpl/cmsdata-save-$cid.tpl.php";
  }
  return MiniFieldCommon::Render($filepath, $variables);
}

function cmsdata_edit_submit($cid, $id){
  $mfd = new MiniFieldData($cid, $id);
  $result = $mfd->SaveDataToDB();
  if(is_array($result)){
    $Data = MiniFieldData::FormatData((object)($mfd->FieldModel), $mfd->arrFields);
    $variables = array(
      'Category' => MiniFieldCategory::LoadDataByCidId(null, $cid, true),
      'arrFields' => $mfd->arrFields,
      'Data' => $Data,
      'action' => base_path()."cmsdata/$cid/$id/edit/submit",
      'errors' => $result
    );
    $filepath = CMSDATA_MODULE_PATH."/tpl/cmsdata-save.tpl.php";
    if(file_exists(CMSDATA_MODULE_PATH."/tpl/cmsdata-save-$cid.tpl.php")){
      $filepath = CMSDATA_MODULE_PATH."/tpl/cmsdata-save-$cid.tpl.php";
    }
    return MiniFieldCommon::Render($filepath, $variables);
  }else{
    drupal_goto("cmsdata/$cid/list");
  }
}