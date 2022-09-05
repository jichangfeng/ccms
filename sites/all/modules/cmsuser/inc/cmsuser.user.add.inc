<?php
function cmsuser_user_add($rid){
  if(arg(4) == "submit"){
    return cmsuser_user_add_submit($rid);
  }
  $Role = CMSRole::LoadRoleByRid($rid, true);
  if($Role->categoryid[0][0] <= 0){return '此角色未选择用户结构';}
  if(CMSRole::CanManageUser('add', $GLOBALS['user'], $Role) == 0){ drupal_access_denied(); }
  $account = user_load(0);
  $mfd = new MiniFieldData($Role->categoryid[0][0], 0, array('AfterConstruct' => 'CMSUserField|AfterConstruct'));
  $mfd->Event_AfterProcessPostData = "CMSUserField|AfterProcessPostData";
  $mfd->Event_AfterSaveDataToDB = "CMSUserField|AfterSaveDataToDB";
  $Data = MiniFieldData::FormatData((object)($mfd->FieldModel), $mfd->arrFields);
  $Data = (object)array_merge((array)$Data, (array)$account);
  $variables = array(
    'Role' => $Role,
    'arrFields' => $mfd->arrFields,
    'Data' => $Data,
    'action' => base_path()."cmsuser/user/$rid/add/submit"
  );
  $filepath = CMSUSER_MODULE_PATH."/tpl/cmsuser-user-save.tpl.php";
  if(file_exists(CMSUSER_MODULE_PATH."/tpl/cmsuser-user-save-$rid.tpl.php")){
    $filepath = CMSUSER_MODULE_PATH."/tpl/cmsuser-user-save-$rid.tpl.php";
  }
  return MiniFieldCommon::Render($filepath, $variables);
}

function cmsuser_user_add_submit($rid){
  $Role = CMSRole::LoadRoleByRid($rid, true);
  if($Role->categoryid[0][0] <= 0){return '此角色未选择用户结构';}
  if(CMSRole::CanManageUser('add', $GLOBALS['user'], $Role) == 0){ drupal_access_denied(); }
  $account = user_load(0);
  $mfd = new MiniFieldData($Role->categoryid[0][0], 0, array('AfterConstruct' => 'CMSUserField|AfterConstruct'));
  $mfd->Event_AfterProcessPostData = "CMSUserField|AfterProcessPostData";
  $mfd->Event_AfterSaveDataToDB = "CMSUserField|AfterSaveDataToDB";
  $_POST['roles'] = $rid;
  $result = CMSUser::SaveUser(0, $mfd);
  if(is_array($result)){
    $Data = MiniFieldData::FormatData((object)($mfd->FieldModel), $mfd->arrFields);
    $Data = (object)array_merge((array)$Data, (array)$account);
    $Data->name = isset($_POST['name']) ? $_POST['name'] : $Data->name;
    $Data->mail = isset($_POST['mail']) ? $_POST['mail'] : $Data->mail;
    $Data->mobile = isset($_POST['mobile']) ? $_POST['mobile'] : $Data->mobile;
    $Data->status = isset($_POST['status']) ? $_POST['status'] : $Data->status;
    $variables = array(
      'Role' => $Role,
      'arrFields' => $mfd->arrFields,
      'Data' => $Data,
      'action' => base_path()."cmsuser/user/$rid/add/submit",
      'errors' => $result
    );
    $filepath = CMSUSER_MODULE_PATH."/tpl/cmsuser-user-save.tpl.php";
    if(file_exists(CMSUSER_MODULE_PATH."/tpl/cmsuser-user-save-$rid.tpl.php")){
      $filepath = CMSUSER_MODULE_PATH."/tpl/cmsuser-user-save-$rid.tpl.php";
    }
    return MiniFieldCommon::Render($filepath, $variables);
  }else{
    drupal_goto("cmsuser/user/$rid/list");
  }
}