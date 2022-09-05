<?php
function cmsuser_role_list(){
  $arrFields = MiniField::LoadDatasByCid(CMSUSER_CID_ROLE, null, false);
  foreach($arrFields as $key=>$Field){
    if($Field->enabled != 1 || $Field->spotlight != 1){
      unset($arrFields[$key]);
    }
  }
  $arrDatas = CMSRole::LoadRoles(null, true);
  $variables = array(
    'arrFields' => $arrFields,
    'arrDatas' => $arrDatas
  );
  $filepath = CMSUSER_MODULE_PATH."/tpl/cmsuser-role-list.tpl.php";
  return MiniFieldCommon::Render($filepath, $variables);
}