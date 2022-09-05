<?php
function cmsuser_user_list($rid){
  $Role = CMSRole::LoadRoleByRid($rid, true);
  if($Role->categoryid[0][0] <= 0){return '此角色未选择用户结构';}
  if(CMSRole::CanManageUser('view', $GLOBALS['user'], $Role) == 0){ drupal_access_denied(); }
  $arrFields = MiniField::LoadDatasByCid($Role->categoryid[0][0], null, false);
  foreach($arrFields as $key=>$Field){
    if($Field->enabled != 1 || $Field->spotlight != 1){
      unset($arrFields[$key]);
    }
  }
  $conditions = array();
  if(isset($_GET['status']) && is_numeric($_GET['status'])){ $conditions[] = array('status', $_GET['status']); }
  if(!isset($_GET['sortfield']) || empty($_GET['sortfield'])){
    $_GET['sortfield'] = 'uid';
    $_GET['sortdirection'] = 'DESC';
  }
  if(!isset($_GET['sortdirection']) || !in_array($_GET['sortdirection'], array('DESC', 'ASC'))){
    $_GET['sortdirection'] = 'DESC';
  }
  if(isset($_GET['sortfilter']) && strlen($_GET['sortfilter']) > 0){
    if($_GET['sortfield'] == 'uid'){
      $conditions[] = array('userid', $_GET['sortfilter']);
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
  $arrDatas = CMSUser::FilterUsersByRid($rid, $conditions, $orderbys, $_GET['limit'], true);
  $variables = array(
    'Role' => $Role,
    'arrFields' => $arrFields,
    'arrDatas' => $arrDatas
  );
  $filepath = CMSUSER_MODULE_PATH."/tpl/cmsuser-user-list.tpl.php";
  if(file_exists(CMSUSER_MODULE_PATH."/tpl/cmsuser-user-list-$rid.tpl.php")){
    $filepath = CMSUSER_MODULE_PATH."/tpl/cmsuser-user-list-$rid.tpl.php";
  }
  return MiniFieldCommon::Render($filepath, $variables);
  
}