<?php
function cmsnode_edit($navid, $id){
  if(arg(4) == "submit"){
    return cmsnode_edit_submit($navid, $id);
  }
  $Navigation = CMSNavigation::LoadNavigationById($navid, true);
  if($Navigation->categoryid[0][0] <= 0){return '此栏目未选择内容结构';}
  $ManageContent = CMSNavigation::CanManageContent('edit', $GLOBALS['user'], $Navigation, $id);
  if($ManageContent == 0){ drupal_access_denied(); }
  $mfd = new MiniFieldData($Navigation->categoryid[0][0], $id, array('AfterConstruct' => 'CMSNodeField|AfterConstruct'));
  $mfd->Event_AfterProcessPostData = "CMSNodeField|AfterProcessPostData";
  $mfd->Event_AfterSaveDataToDB = "CMSNodeField|AfterSaveDataToDB";
  $variables = array(
    'Navigation' => $Navigation,
    'arrFields' => $mfd->arrFields,
    'Data' => MiniFieldData::FormatData((object)($mfd->FieldModel), $mfd->arrFields),
    'action' => base_path()."cmsnode/$navid/$id/edit/submit".(isset($_GET['page'])?'?page='.$_GET['page']:'')
  );
  $filepath = CMSNODE_MODULE_PATH."/tpl/cmsnode-save.tpl.php";
  if(file_exists(CMSNODE_MODULE_PATH."/tpl/cmsnode-save-$navid.tpl.php")){
    $filepath = CMSNODE_MODULE_PATH."/tpl/cmsnode-save-$navid.tpl.php";
  }
  return MiniFieldCommon::Render($filepath, $variables);
}

function cmsnode_edit_submit($navid, $id){
  $Navigation = CMSNavigation::LoadNavigationById($navid, true);
  if($Navigation->categoryid[0][0] <= 0){return '此栏目未选择内容结构';}
  $ManageContent = CMSNavigation::CanManageContent('edit', $GLOBALS['user'], $Navigation, $id);
  if($ManageContent == 0){ drupal_access_denied(); }
  $mfd = new MiniFieldData($Navigation->categoryid[0][0], $id, array('AfterConstruct' => 'CMSNodeField|AfterConstruct'));
  $mfd->Event_AfterProcessPostData = "CMSNodeField|AfterProcessPostData";
  $mfd->Event_AfterSaveDataToDB = "CMSNodeField|AfterSaveDataToDB";
  if(isset($_POST['navid'])){ unset($_POST['navid']); }
  if(isset($_POST['userid'])){ unset($_POST['userid']); }
  if(isset($_POST['created'])){ unset($_POST['created']); }
  $result = $mfd->SaveDataToDB();
  if(is_array($result)){
    $variables = array(
      'Navigation' => $Navigation,
      'arrFields' => $mfd->arrFields,
      'Data' => MiniFieldData::FormatData((object)($mfd->FieldModel), $mfd->arrFields),
      'action' => base_path()."cmsnode/$navid/$id/edit/submit".(isset($_GET['page'])?'?page='.$_GET['page']:''),
      'errors' => $result
    );
    $filepath = CMSNODE_MODULE_PATH."/tpl/cmsnode-save.tpl.php";
    if(file_exists(CMSNODE_MODULE_PATH."/tpl/cmsnode-save-$navid.tpl.php")){
      $filepath = CMSNODE_MODULE_PATH."/tpl/cmsnode-save-$navid.tpl.php";
    }
    return MiniFieldCommon::Render($filepath, $variables);
  }else{
    $options = array();
    if(isset($_GET['page'])){
      $options['query'] = array();
      $options['query']['page'] = $_GET['page'];
    }
    drupal_goto("cmsnode/$navid/list", $options);
  }
}