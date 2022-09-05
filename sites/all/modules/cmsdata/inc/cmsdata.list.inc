<?php
function cmsdata_list($cid){
  $arrFields = MiniField::LoadDatasByCid($cid, null, false);
  foreach($arrFields as $key=>$Field){
    if($Field->enabled != 1 || $Field->spotlight != 1){
      unset($arrFields[$key]);
    }
  }
  $arrDatas = array();
  if($cid == CMSDATA_CID_NAVIGATION){
    $arrDatas = CMSNavigation::LoadNavigations(0, 1, true);
  }else if($cid == CMSDATA_CID_TRANSLATION){
    $conditions = array();
    if(isset($_POST['zhs']) && !empty($_POST['zhs'])){ $conditions[] = array('zhs', '%'.db_like($_POST['zhs']).'%', 'LIKE'); }
    if(isset($_POST['zht']) && !empty($_POST['zht'])){ $conditions[] = array('zht', '%'.db_like($_POST['zht']).'%', 'LIKE'); }
    if(isset($_POST['enus']) && !empty($_POST['enus'])){ $conditions[] = array('enus', '%'.db_like($_POST['enus']).'%', 'LIKE'); }
    $limit = empty($conditions) ? 20 : null;
    if(isset($_GET['groupby']) && !empty($_GET['groupby'])){ $conditions[] = array('groupby', $_GET['groupby']); }
    $arrDatas = MiniFieldData::FilterDatasByCid($cid, array(), array(), $conditions, array(), $limit, true);
  }else{
    $arrDatas = MiniFieldData::LoadDatasByCid($cid, 20, true);
  }
  $variables = array(
    'Category' => MiniFieldCategory::LoadDataByCidId(null, $cid, true),
    'arrFields' => $arrFields,
    'arrDatas' => $arrDatas
  );
  $filepath = CMSDATA_MODULE_PATH."/tpl/cmsdata-list.tpl.php";
  if(file_exists(CMSDATA_MODULE_PATH."/tpl/cmsdata-list-$cid.tpl.php")){
    $filepath = CMSDATA_MODULE_PATH."/tpl/cmsdata-list-$cid.tpl.php";
  }
  return MiniFieldCommon::Render($filepath, $variables);
}