<?php
function cmsdata_add($cid){
  if(arg(3) == "submit"){
    return cmsdata_add_submit($cid);
  }
  $mfd = new MiniFieldData($cid);
  if(isset($mfd->FieldModel['weight']) && $cid == CMSDATA_CID_NAVIGATION){
    $mfd->FieldModel['weight'] = (int)MiniFieldData::LoadDataExpressionByCidField($cid, 'weight', 'MAX') + 1;
  }
  $Data = MiniFieldData::FormatData((object)($mfd->FieldModel), $mfd->arrFields);
  $variables = array(
    'Category' => MiniFieldCategory::LoadDataByCidId(null, $cid, true),
    'arrFields' => $mfd->arrFields,
    'Data' => $Data,
    'action' => base_path()."cmsdata/$cid/add/submit"
  );
  $filepath = CMSDATA_MODULE_PATH."/tpl/cmsdata-save.tpl.php";
  if(file_exists(CMSDATA_MODULE_PATH."/tpl/cmsdata-save-$cid.tpl.php")){
    $filepath = CMSDATA_MODULE_PATH."/tpl/cmsdata-save-$cid.tpl.php";
  }
  return MiniFieldCommon::Render($filepath, $variables);
}

function cmsdata_add_submit($cid){
  $mfd = new MiniFieldData($cid);
  $result = $mfd->SaveDataToDB();
  if(is_array($result)){
    $Data = MiniFieldData::FormatData((object)($mfd->FieldModel), $mfd->arrFields);
    $variables = array(
      'Category' => MiniFieldCategory::LoadDataByCidId(null, $cid, true),
      'arrFields' => $mfd->arrFields,
      'Data' => $Data,
      'action' => base_path()."cmsdata/$cid/add/submit",
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