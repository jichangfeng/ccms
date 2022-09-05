<?php
function cmsnode_list($navid){
  $Navigation = CMSNavigation::LoadNavigationById($navid, true);
  if($Navigation->categoryid[0][0] <= 0){return '此栏目未选择内容结构';}
  $ManageContent = CMSNavigation::CanManageContent('view', $GLOBALS['user'], $Navigation);
  if($ManageContent == 0){ drupal_access_denied(); }
  $arrFields = MiniField::LoadDatasByCid($Navigation->categoryid[0][0], null, false);
  $Field_status = null;
  foreach($arrFields as $key => $Field){
    if($Field->fieldname == 'status'){
      $Field_status = $Field;
    }
    if($Field->enabled != 1 || $Field->spotlight != 1){
      unset($arrFields[$key]);
    }
  }
  $conditions = array();
  if($ManageContent == 1){ $conditions[] = array('userid', $GLOBALS['user']->uid); }
  if(isset($_GET['status']) && is_numeric($_GET['status'])){ $conditions[] = array('status', $_GET['status']); }
  if(!isset($_GET['sortfield']) || empty($_GET['sortfield'])){
    $_GET['sortfield'] = 'id';
    $_GET['sortdirection'] = 'DESC';
  }
  if(!isset($_GET['sortdirection']) || !in_array($_GET['sortdirection'], array('DESC', 'ASC'))){
    $_GET['sortdirection'] = 'DESC';
  }
  if(isset($_GET['sortfilter']) && strlen($_GET['sortfilter']) > 0){
    if($_GET['sortfield'] == 'id' && is_numeric($_GET['sortfilter'])){
      $conditions[] = array($_GET['sortfield'], $_GET['sortfilter']);
    }else if($_GET['sortfield'] == 'userid' && is_numeric($_GET['sortfilter'])){
      $conditions[] = array($_GET['sortfield'], $_GET['sortfilter']);
    }else if($_GET['sortfield'] == 'userid' && is_string($_GET['sortfilter'])){
      $uides = MiniFieldSql::LoadDataColByTableField('users', 'uid', array(array('name', '%' . db_like($_GET['sortfilter']) . '%', 'LIKE')));
      $conditions[] = array($_GET['sortfield'], $uides, 'IN');
    }else if(isset($_GET['sortmatch']) && $_GET['sortmatch'] == '1'){
      $conditions[] = array($_GET['sortfield'], $_GET['sortfilter']);
    }else{
      $conditions[] = array($_GET['sortfield'], '%' . db_like($_GET['sortfilter']) . '%', 'LIKE');
    }
  }
  $orderbys = array();
  $orderbys[] = array($_GET['sortfield'], $_GET['sortdirection']);
  if(!isset($_GET['limit']) || !in_array($_GET['limit'], array(5, 10, 20, 50, 100))){
    $_GET['limit'] = 20;
  }
  $arrDatas = CMSNode::FilterNodesByNavid($navid, 0, array(), array(), $conditions, $orderbys, $_GET['limit'], true);
  $variables = array(
    'Navigation' => $Navigation,
    'arrFields' => $arrFields,
    'Field_status' => $Field_status,
    'arrDatas' => $arrDatas
  );
  $filepath = CMSNODE_MODULE_PATH."/tpl/cmsnode-list.tpl.php";
  if(file_exists(CMSNODE_MODULE_PATH."/tpl/cmsnode-list-$navid.tpl.php")){
    $filepath = CMSNODE_MODULE_PATH."/tpl/cmsnode-list-$navid.tpl.php";
  }
  return MiniFieldCommon::Render($filepath, $variables);
}