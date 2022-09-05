<?php
function cmsuser_usermaterial(){
  global $user;
  $rid = 0;
  $roleides = array_keys($user->roles);
  foreach($roleides as $roleid){
    if($roleid <= 2){ continue; }
    $rid = $roleid;
    break;
  }
  if($rid == 0){ return ''; }
  $uid = $user->uid;
  if(arg(2) == "modify"){
    return cmsuser_usermaterial_modify($rid, $uid);
  }
  $Role = CMSRole::LoadRoleByRid($rid, false);
  if(!isset($Role->categoryid) || !is_numeric($Role->categoryid)){ return ''; }
  $tablename = minifield_tablename_load($Role->categoryid);
  if(empty($tablename)){ return ''; }
  $account = user_load($uid);
  if(!isset($account->uid) || $account->uid <= 0 || !isset($account->roles[$rid])){ return ''; }
  $arrDatas = MiniFieldData::FilterDatasByCid($Role->categoryid, array(), array(), array(array('userid', $account->uid)), array(), 1, false);
  $id = isset($arrDatas[0]) ? $arrDatas[0]->id : 0;
  $mfd = new MiniFieldData($Role->categoryid, $id);
  $Data = MiniFieldData::FormatData((object)($mfd->FieldModel), $mfd->arrFields);
  $Data = (object)array_merge((array)$Data, (array)$account);
  $variables = array(
    'Role' => $Role,
    'arrFields' => $mfd->arrFields,
    'Data' => $Data,
    'action' => base_path()."cmsuser/usermaterial/modify"
  );
  $filepath = CMSUSER_MODULE_PATH."/tpl/cmsuser-user-material.tpl.php";
  return MiniFieldCommon::Render($filepath, $variables);
}
function cmsuser_usermaterial_modify($rid, $uid){
  $Role = CMSRole::LoadRoleByRid($rid, false);
  if(!isset($Role->categoryid) || !is_numeric($Role->categoryid)){ return ''; }
  $tablename = minifield_tablename_load($Role->categoryid);
  if(empty($tablename)){ return ''; }
  $account = user_load($uid);
  if(!isset($account->uid) || $account->uid <= 0 || !isset($account->roles[$rid])){ return ''; }
  $arrDatas = MiniFieldData::FilterDatasByCid($Role->categoryid, array(), array(), array(array('userid', $account->uid)), array(), 1, false);
  $id = isset($arrDatas[0]) ? $arrDatas[0]->id : 0;
  $mfd = new MiniFieldData($Role->categoryid, $id);
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
      'action' => base_path()."cmsuser/usermaterial/modify",
      'errors' => $result
    );
    $filepath = CMSUSER_MODULE_PATH."/tpl/cmsuser-user-material.tpl.php";
    return MiniFieldCommon::Render($filepath, $variables);
  }else{
    drupal_goto("cmsuser/usermaterial");
  }
}

function cmsuser_user_structure(){
  $arrRoles = CMSRole::LoadRoles(null, true);
  $variables = array('arrRoles' => $arrRoles);
  $filepath = CMSUSER_MODULE_PATH."/tpl/cmsuser-user-structure.tpl.php";
  return MiniFieldCommon::Render($filepath, $variables);
}

/**
 *
 * @param type $handle handle style (list, add, edit, del)
 * @param type $rid role identification number
 * @param type $uid user identification number
 * @return type
 */
function cmsuser_user($handle, $rid, $uid = 0){
  $arrHandles = array('list', 'add', 'edit', 'del');
  if(!in_array($handle, $arrHandles)){
    return $handle . ' 处理方式未定义！';
  }
  unset ($arrHandles);
  $filepath = "cmsuser.user.$handle.inc";
  if(!file_exists(CMSUSER_MODULE_PATH . "/inc/" . $filepath)){
    return CMSUSER_MODULE_PATH . "/inc/" . $filepath . ' 文件未找到！';
  }
  include $filepath;
  $function = "cmsuser_user_" . $handle;
  if(function_exists($function)){
    return $uid == 0 ? $function($rid) : $function($rid, $uid);
  }else{
    return $handle . ' 处理函数未定义！';
  }
}

function cmsuser_user_handle($rid, $arg){
  if(is_string($arg) && $arg == 'clear'){
    if(is_numeric($rid)){ CMSUser::ClearUsersByRid($rid); }
    drupal_goto("cmsuser/user/$rid/list");return;
  }else if(is_string($arg) && $arg == 'delete'){
    $Role = CMSRole::LoadRoleByRid($rid, true);
    if($Role->categoryid[0][0] <= 0){ echo 'error';exit; }
    if(CMSRole::CanManageUser('delete', $GLOBALS['user'], $Role) == 0){ echo 'error';exit; }
    if(isset($_POST['uides']) && is_array($_POST['uides'])){
      CMSUser::DeleteUsersByRidUides($rid, $_POST['uides']);
    }
    echo 'ok';exit;
  }else if(is_string($arg) && $arg == 'getinfo'){
    $infos = array('uid' => 0, 'name' => '', 'mail' => '', 'mobile' => '', 'status' => 0, 'exist' => false, 'isloggedin' => false, 'isonline' => false);
    $key = isset($_POST['key']) ? $_POST['key'] : '';
    if(empty($key)){
      echo json_encode($infos);exit;
    }
    $uid = 0;
    empty($uid) && (bool)filter_var($key, FILTER_VALIDATE_EMAIL) ? $uid = CMSUser::LoadUserUidByMail($key) : null;
    empty($uid) && preg_match('/^1\d{10}$/', $key) ? $uid = CMSUser::LoadUserUidByMobile($key) : null;
    empty($uid) && is_string($key) ? $uid = CMSUser::LoadUserUidByName($key) : null;
    empty($uid) && is_numeric($key) ? $uid = $key : null;
    $account = user_load($uid);
    if(isset($account->uid) && $account->uid > 0){
      $infos['uid'] = $account->uid;
      $infos['name'] = $account->name;
      $infos['mail'] = preg_replace('/(?<=.).(?=.*@)/u', '*', $account->mail);
      $infos['mobile'] = preg_replace("/(\d{2})\d{7}(\d{2})/", "\$1******\$2", $account->mobile);
      $infos['status'] = $account->status;
      $infos['exist'] = true;
      $infos['isloggedin'] = CMSUser::IsLoggedIn($account->uid);
      $infos['isonline'] = CMSUser::IsOnline($account->uid);
    }
    echo json_encode($infos);exit;
  }else if(is_string($arg) && $arg == 'logout'){
    global $user;
    if(!isset($user->uid) || !$user->uid){
      echo '0';exit;
    }
    watchdog('user', 'Session closed for %name.', array('%name' => $user->name));
    module_invoke_all('user_logout', $user);
    // Destroy the current session, and reset $user to the anonymous user.
    session_destroy();
    echo '1';exit;
  }else if(is_string($arg) && $arg == 'login'){
    $name = isset($_POST['name']) ? $_POST['name'] : '';
    $pass = isset($_POST['pass']) ? $_POST['pass'] : '';
    $result = CMSUser::LoginUser($name, $pass);
    echo json_encode($result);exit;
  }else if(is_string($arg) && $arg == 'register'){
    $autologin = isset($_POST['autologin']) && $_POST['autologin'] ? true: false;
    $result = CMSUser::RegisterUser($autologin);
    echo json_encode($result);exit;
  }else if(is_string($arg) && $arg == 'activate'){
    if(!isset($_POST['uid']) || !is_numeric($_POST['uid'])){
      echo json_encode(array('status' => 'error', 'text' => '缺少参数'));exit;
    }else if(!isset($_POST['action']) || !in_array($_POST['action'], array('apply', 'validate'))){
      echo json_encode(array('status' => 'error', 'text' => '缺少参数'));exit;
    }else if($_POST['action'] == 'apply' && isset($_POST['officialname'])){
      $result = CMSUser::ActivateUserApply($_POST['uid'], $_POST['officialname']);
      echo json_encode($result);exit;
    }else if($_POST['action'] == 'validate' && isset($_POST['activatecode'])){
      $result = CMSUser::ActivateUserValidate($_POST['uid'], $_POST['activatecode']);
      echo json_encode($result);exit;
    }else{
      echo json_encode(array('status' => 'error', 'text' => '缺少参数'));exit;
    }
  }else if(is_string($arg) && $arg == 'resetpass'){
    if(!isset($_POST['uid']) || !is_numeric($_POST['uid'])){
      echo json_encode(array('status' => 'error', 'text' => '缺少参数'));exit;
    }else if(!isset($_POST['action']) || !in_array($_POST['action'], array('apply', 'validate'))){
      echo json_encode(array('status' => 'error', 'text' => '缺少参数'));exit;
    }else if($_POST['action'] == 'apply' && isset($_POST['officialname'])){
      $result = CMSUser::ResetPassApply($_POST['uid'], $_POST['officialname']);
      echo json_encode($result);exit;
    }else if($_POST['action'] == 'validate' && isset($_POST['resetpasscode']) && isset($_POST['pass'])){
      $result = CMSUser::ResetPassValidate($_POST['uid'], $_POST['resetpasscode'], $_POST['pass']);
      echo json_encode($result);exit;
    }else{
      echo json_encode(array('status' => 'error', 'text' => '缺少参数'));exit;
    }
  }
  exit;
}