<?php
function cmsnode_structure(){
  $arrFields = MiniField::LoadDatasByCid(CMSDATA_CID_NAVIGATION, null, false);
  foreach($arrFields as $key=>$Field){
    if($Field->enabled != 1 || $Field->spotlight != 1){
      unset($arrFields[$key]);
    }
  }
  $arrDatas = CMSNavigation::LoadNavigations(0, 1, true);
  $variables = array(
    'Category' => MiniFieldCategory::LoadDataByCidId(null, CMSDATA_CID_NAVIGATION, true),
    'arrFields' => $arrFields,
    'arrDatas' => $arrDatas
  );
  $filepath = CMSNODE_MODULE_PATH."/tpl/cmsnode-structure.tpl.php";
  return MiniFieldCommon::Render($filepath, $variables);
}

/**
 *
 * @param type $handle handle style (list, add, edit, delete)
 * @param type $navid Navigation Identification number
 * @param type $id Data Identification number
 * @return type
 */
function cmsnode($handle, $navid, $id = 0){
  $arrHandles = array('list', 'add', 'edit', 'delete');
  if(!in_array($handle, $arrHandles)){
    return $handle . ' 处理方式未定义！';
  }
  unset ($arrHandles);
  $filepath = "cmsnode.$handle.inc";
  if(!file_exists(CMSNODE_MODULE_PATH . "/inc/" . $filepath)){
    return CMSNODE_MODULE_PATH . "/inc/" . $filepath . ' 文件未找到！';
  }
  include $filepath;
  $function = "cmsnode_" . $handle;
  if(function_exists($function)){
    return $id == 0 ? $function($navid) : $function($navid, $id);
  }else{
    return $handle . ' 处理函数未定义！';
  }
}
/**
 * @param type $navtag The navigation id or urlalias.
 */
function cmsnode_handle($navtag){
  if(!is_string($navtag) && !is_numeric($navtag)){ drupal_not_found();exit; }
  $arg = arg(2);
  if(is_string($arg) && $arg == 'delete'){
    if(isset($_POST['nodeides']) && is_array($_POST['nodeides'])){
      $Navigation = CMSNavigation::LoadNavigationByTag($navtag, true);
      if(!isset($Navigation->id)){ echo 'error';exit; }
      $ManageContent = CMSNavigation::CanManageContent('delete', $GLOBALS['user'], $Navigation);
      if($ManageContent == 0){ echo 'error';exit; }
      $conditions = array();
      $conditions[] = array('id', $_POST['nodeides'], 'IN');
      $conditions[] = array('navid', $Navigation->id);
      if($ManageContent == 1){ $conditions[] = array('userid', $GLOBALS['user']->uid); }
      MiniFieldData::DeleteDatasByCid($Navigation->categoryid[0][0], $conditions, true);
    }
    echo 'ok';exit;
  }else if(is_string($arg) && $arg == 'mark'){
    if(isset($_POST['nodeides']) && is_array($_POST['nodeides']) && isset($_POST['status']) && is_numeric($_POST['status'])){
      $Navigation = CMSNavigation::LoadNavigationByTag($navtag, true);
      if(!isset($Navigation->id)){ echo 'error';exit; }
      $ManageContent = CMSNavigation::CanManageContent('edit', $GLOBALS['user'], $Navigation);
      if($ManageContent == 0){ echo 'error';exit; }
      $tablename = minifield_tablename_load($Navigation->categoryid[0][0]);
      if(empty($tablename)){ echo 'error';exit; }
      $condition = db_update($tablename)->fields(array('status' => $_POST['status']))
        ->condition('id', $_POST['nodeides'], 'IN')->condition('navid', $Navigation->id);
      if($ManageContent == 1){ $condition->condition('userid', $GLOBALS['user']->uid); }
      $condition->execute();
    }
    echo 'ok';exit;
  }else if(is_string($arg) && $arg == 'move'){
    if(isset($_POST['nodeides']) && is_array($_POST['nodeides']) && isset($_POST['tonavid']) && is_numeric($_POST['tonavid'])){
      $FromNavigation = CMSNavigation::LoadNavigationByTag($navtag, true);
      if(!isset($FromNavigation->id)){ echo 'error';exit; }
      $ManageContent_From = CMSNavigation::CanManageContent('edit', $GLOBALS['user'], $FromNavigation);
      if($ManageContent_From == 0){ echo 'error';exit; }
      $ToNavigation = CMSNavigation::LoadNavigationByTag($_POST['tonavid'], true);
      if(!isset($ToNavigation->id)){ echo 'error';exit; }
      $ManageContent_To = CMSNavigation::CanManageContent('add', $GLOBALS['user'], $ToNavigation);
      if($ManageContent_To == 0){ echo 'error';exit; }
      if($FromNavigation->categoryid[0][0] != $ToNavigation->categoryid[0][0]){ echo 'error';exit; }
      if($FromNavigation->id == $ToNavigation->id){ echo 'error';exit; }
      $tablename = minifield_tablename_load($Navigation->categoryid[0][0]);
      if(empty($tablename)){ echo 'error';exit; }
      $condition = db_update($tablename)->fields(array('navid' => $ToNavigation->id))
        ->condition('id', $_POST['nodeides'], 'IN')->condition('navid', $FromNavigation->id);
      if($ManageContent_From == 1){ $condition->condition('userid', $GLOBALS['user']->uid); }
      $condition->execute();
    }
    echo 'ok';exit;
  }else if(is_string($arg) && $arg == 'deliveradd'){
    $Navigation = CMSNavigation::LoadNavigationByTag($navtag, true);
    if($Navigation->categoryid[0][0] <= 0){ echo '此栏目未选择内容组织形式';exit; }
    $mfd = new MiniFieldData($Navigation->categoryid[0][0], 0, array('AfterConstruct' => 'CMSNodeField|AfterConstruct'));
    $mfd->Event_AfterProcessPostData = "CMSNodeField|AfterProcessPostData";
    $mfd->Event_AfterSaveDataToDB = "CMSNodeField|AfterSaveDataToDB";
    if($mfd->FieldModel['id'] == 0){
      $_POST['navid'] = $Navigation->id;
      $_POST['userid'] = $GLOBALS['user']->uid;
      $time = time();
      $_POST['created'] = @date("Y-m-d", $time);
      $_POST['createdhour'] = @date("H", $time);
      $_POST['createdminute'] = @date("i", $time);
      $_POST['createdsecond'] = @date("s", $time);
      $result = $mfd->SaveDataToDB();
      echo json_encode($result);exit;
    }
    echo 'error';exit;
  }else if(is_string($arg) && $arg == 'deliveredit'){
    $Navigation = CMSNavigation::LoadNavigationByTag($navtag, true);
    if($Navigation->categoryid[0][0] <= 0){echo '此栏目未选择内容组织形式';exit;}
    $mfd = new MiniFieldData($Navigation->categoryid[0][0], arg(3), array('AfterConstruct' => 'CMSNodeField|AfterConstruct'));
    $mfd->Event_AfterProcessPostData = "CMSNodeField|AfterProcessPostData";
    $mfd->Event_AfterSaveDataToDB = "CMSNodeField|AfterSaveDataToDB";
    if($mfd->FieldModel['id'] > 0){
      if(isset($_POST['navid'])){ unset($_POST['navid']); }
      if(isset($_POST['userid'])){ unset($_POST['userid']); }
      if(isset($_POST['created'])){ unset($_POST['created']); }
      $result = $mfd->SaveDataToDB();
      echo json_encode($result);exit;
    }
    echo 'error';exit;
  }else if(is_string($arg) && $arg == 'deliverdel'){
    $id = arg(3);
    if(!is_numeric($id) || $id <= 0){ echo 'error';exit; }
    $Navigation = CMSNavigation::LoadNavigationByTag($navtag, true);
    MiniFieldData::DeleteDataByCidId($Navigation->categoryid[0][0], $id, true);
    echo 'ok';exit;
  }
}