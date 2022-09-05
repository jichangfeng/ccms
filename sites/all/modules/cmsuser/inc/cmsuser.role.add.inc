<?php
function cmsuser_role_add(){
  if(arg(3) == "submit"){
    return cmsuser_role_add_submit();
  }
  $role = new stdClass();
  $role->name = '';
  $role->weight = (int)MiniFieldSql::LoadDataExpressionByTableField('role', 'weight', 'MAX') + 1;
  $mfd = new MiniFieldData(CMSUSER_CID_ROLE);
  $Data = MiniFieldData::FormatData((object)($mfd->FieldModel), $mfd->arrFields);
  $Data = (object)array_merge((array)$Data, (array)$role);
  $variables = array(
    'arrFields' => $mfd->arrFields,
    'Data' => $Data,
    'action' => base_path()."cmsuser/role/add/submit"
  );
  $filepath = CMSUSER_MODULE_PATH."/tpl/cmsuser-role-save.tpl.php";
  return MiniFieldCommon::Render($filepath, $variables);
}

function cmsuser_role_add_submit(){
  $role = new stdClass();
  $role->name = '';
  $role->weight = 0;
  $mfd = new MiniFieldData(CMSUSER_CID_ROLE);
  $result = CMSRole::SaveRole(0, $mfd);
  if(is_array($result)){
    $Data = MiniFieldData::FormatData((object)($mfd->FieldModel), $mfd->arrFields);
    $Data = (object)array_merge((array)$Data, (array)$role);
    $variables = array(
      'arrFields' => $mfd->arrFields,
      'Data' => $Data,
      'action' => base_path()."cmsuser/role/add/submit",
      'errors' => $result
    );
    $filepath = CMSUSER_MODULE_PATH."/tpl/cmsuser-role-save.tpl.php";
    return MiniFieldCommon::Render($filepath, $variables);
  }else{
    drupal_goto("cmsuser/role/list");
  }
}