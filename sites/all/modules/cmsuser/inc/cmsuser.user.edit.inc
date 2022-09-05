<?php
function cmsuser_user_edit($rid, $uid){
  if(arg(5) == "submit"){
    return cmsuser_user_edit_submit($rid, $uid);
  }
  $Role = CMSRole::LoadRoleByRid($rid, true);
  if($Role->categoryid[0][0] <= 0){return '此角色未选择用户结构';}
  if(CMSRole::CanManageUser('edit', $GLOBALS['user'], $Role) == 0){ drupal_access_denied(); }
  $account = user_load($uid);
  if(!isset($account->uid) || $account->uid <= 0 || !isset($account->roles[$rid])){ return ''; }
  $arrDatas = MiniFieldData::FilterDatasByCid($Role->categoryid[0][0], array(), array(), array(array('userid', $account->uid)), array(), 1, false);
  $id = isset($arrDatas[0]) ? $arrDatas[0]->id : 0;
  $mfd = new MiniFieldData($Role->categoryid[0][0], $id, array('AfterConstruct' => 'CMSUserField|AfterConstruct'));
  $mfd->Event_AfterProcessPostData = "CMSUserField|AfterProcessPostData";
  $mfd->Event_AfterSaveDataToDB = "CMSUserField|AfterSaveDataToDB";
  $Data = MiniFieldData::FormatData((object)($mfd->FieldModel), $mfd->arrFields);
  $Data = (object)array_merge((array)$Data, (array)$account);
  $variables = array(
    'Role' => $Role,
    'arrFields' => $mfd->arrFields,
    'Data' => $Data,
    'action' => base_path()."cmsuser/user/$rid/$uid/edit/submit"
  );
  $filepath = CMSUSER_MODULE_PATH."/tpl/cmsuser-user-save.tpl.php";
  if(file_exists(CMSUSER_MODULE_PATH."/tpl/cmsuser-user-save-$rid.tpl.php")){
    $filepath = CMSUSER_MODULE_PATH."/tpl/cmsuser-user-save-$rid.tpl.php";
  }
  return MiniFieldCommon::Render($filepath, $variables);
}

function cmsuser_user_edit_submit($rid, $uid){
  $Role = CMSRole::LoadRoleByRid($rid, true);
  if($Role->categoryid[0][0] <= 0){return '此角色未选择用户结构';}
  if(CMSRole::CanManageUser('edit', $GLOBALS['user'], $Role) == 0){ drupal_access_denied(); }
  $account = user_load($uid);
  if(!isset($account->uid) || $account->uid <= 0 || !isset($account->roles[$rid])){ return ''; }
  $arrDatas = MiniFieldData::FilterDatasByCid($Role->categoryid[0][0], array(), array(), array(array('userid', $account->uid)), array(), 1, false);
  $id = isset($arrDatas[0]) ? $arrDatas[0]->id : 0;
  $mfd = new MiniFieldData($Role->categoryid[0][0], $id, array('AfterConstruct' => 'CMSUserField|AfterConstruct'));
  $mfd->Event_AfterProcessPostData = "CMSUserField|AfterProcessPostData";
  $mfd->Event_AfterSaveDataToDB = "CMSUserField|AfterSaveDataToDB";
  $_POST['roles'] = $rid;
  $result = CMSUser::SaveUser($uid, $mfd);
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
      'action' => base_path()."cmsuser/user/$rid/$uid/edit/submit",
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