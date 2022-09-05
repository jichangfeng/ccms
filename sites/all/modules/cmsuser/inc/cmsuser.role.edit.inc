<?php
function cmsuser_role_edit($rid){
  if(arg(4) == "submit"){
    return cmsuser_role_edit_submit($rid);
  }
  $role = user_role_load($rid);
  if(!isset($role->rid) || !is_numeric($role->rid)){ return ''; }
  $arrDatas = MiniFieldData::FilterDatasByCid(CMSUSER_CID_ROLE, array(), array(), array(array('roleid', $role->rid)), array(), 1, false);
  $id = isset($arrDatas[0]) ? $arrDatas[0]->id : 0;
  $mfd = new MiniFieldData(CMSUSER_CID_ROLE, $id);
  $Data = MiniFieldData::FormatData((object)($mfd->FieldModel), $mfd->arrFields);
  $Data = (object)array_merge((array)$Data, (array)$role);
  $variables = array(
    'arrFields' => $mfd->arrFields,
    'Data' => $Data,
    'action' => base_path()."cmsuser/role/".$rid."/edit/submit"
  );
  $filepath = CMSUSER_MODULE_PATH."/tpl/cmsuser-role-save.tpl.php";
  return MiniFieldCommon::Render($filepath, $variables);
}

function cmsuser_role_edit_submit($rid){
  $role = user_role_load($rid);
  if(!isset($role->rid) || !is_numeric($role->rid)){ return ''; }
  $arrDatas = MiniFieldData::FilterDatasByCid(CMSUSER_CID_ROLE, array(), array(), array(array('roleid', $role->rid)), array(), 1, false);
  $id = isset($arrDatas[0]) ? $arrDatas[0]->id : 0;
  $mfd = new MiniFieldData(CMSUSER_CID_ROLE, $id);
  $result = CMSRole::SaveRole($rid, $mfd);
  if(is_array($result)){
    $Data = MiniFieldData::FormatData((object)($mfd->FieldModel), $mfd->arrFields);
    $Data = (object)array_merge((array)$Data, (array)$role);
    $variables = array(
      'arrFields' => $mfd->arrFields,
      'Data' => $Data,
      'action' => base_path()."cmsuser/role/".$rid."/edit/submit",
      'errors' => $result
    );
    $filepath = CMSUSER_MODULE_PATH."/tpl/cmsuser-role-save.tpl.php";
    return MiniFieldCommon::Render($filepath, $variables);
  }else{
    drupal_goto("cmsuser/role/list");
  }
}