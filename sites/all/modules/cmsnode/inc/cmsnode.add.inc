<?php
function cmsnode_add($navid){
  if(arg(3) == "submit"){
    return cmsnode_add_submit($navid);
  }
  $Navigation = CMSNavigation::LoadNavigationById($navid, true);
  if($Navigation->categoryid[0][0] <= 0){return '此栏目未选择内容结构';}
  $ManageContent = CMSNavigation::CanManageContent('add', $GLOBALS['user'], $Navigation);
  if($ManageContent == 0){ drupal_access_denied(); }
  $mfd = new MiniFieldData($Navigation->categoryid[0][0], 0, array('AfterConstruct' => 'CMSNodeField|AfterConstruct'));
  $mfd->Event_AfterProcessPostData = "CMSNodeField|AfterProcessPostData";
  $mfd->Event_AfterSaveDataToDB = "CMSNodeField|AfterSaveDataToDB";
  $variables = array(
    'Navigation' => $Navigation,
    'arrFields' => $mfd->arrFields,
    'Data' => MiniFieldData::FormatData((object)($mfd->FieldModel), $mfd->arrFields),
    'action' => base_path()."cmsnode/$navid/add/submit"
  );
  $filepath = CMSNODE_MODULE_PATH."/tpl/cmsnode-save.tpl.php";
  if(file_exists(CMSNODE_MODULE_PATH."/tpl/cmsnode-save-$navid.tpl.php")){
    $filepath = CMSNODE_MODULE_PATH."/tpl/cmsnode-save-$navid.tpl.php";
  }
  return MiniFieldCommon::Render($filepath, $variables);
}

function cmsnode_add_submit($navid){
  $Navigation = CMSNavigation::LoadNavigationById($navid, true);
  if($Navigation->categoryid[0][0] <= 0){return '此栏目未选择内容结构';}
  $ManageContent = CMSNavigation::CanManageContent('add', $GLOBALS['user'], $Navigation);
  if($ManageContent == 0){ drupal_access_denied(); }
  $mfd = new MiniFieldData($Navigation->categoryid[0][0], 0, array('AfterConstruct' => 'CMSNodeField|AfterConstruct'));
  $mfd->Event_AfterProcessPostData = "CMSNodeField|AfterProcessPostData";
  $mfd->Event_AfterSaveDataToDB = "CMSNodeField|AfterSaveDataToDB";
  $_POST['navid'] = $navid;
  $_POST['userid'] = $GLOBALS['user']->uid;
  $time = time();
  $_POST['created'] = @date("Y-m-d", $time);
  $_POST['createdhour'] = @date("H", $time);
  $_POST['createdminute'] = @date("i", $time);
  $_POST['createdsecond'] = @date("s", $time);
  $result = $mfd->SaveDataToDB();
  if(is_array($result)){
    $variables = array(
      'Navigation' => $Navigation,
      'arrFields' => $mfd->arrFields,
      'Data' => MiniFieldData::FormatData((object)($mfd->FieldModel), $mfd->arrFields),
      'action' => base_path()."cmsnode/$navid/add/submit",
      'errors' => $result
    );
    $filepath = CMSNODE_MODULE_PATH."/tpl/cmsnode-save.tpl.php";
    if(file_exists(CMSNODE_MODULE_PATH."/tpl/cmsnode-save-$navid.tpl.php")){
      $filepath = CMSNODE_MODULE_PATH."/tpl/cmsnode-save-$navid.tpl.php";
    }
    return MiniFieldCommon::Render($filepath, $variables);
  }else{
    drupal_goto("cmsnode/$navid/list");
  }
}