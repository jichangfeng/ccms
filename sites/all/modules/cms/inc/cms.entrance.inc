<?php
function cms_entrance(){
  global $user;
  if($user->uid > 0 && cmsMenuAccessCheck('cms/front')){
    drupal_goto('cms/front');
  }
  $variables = array();
  $variables['name'] = '';
  $variables['pass'] = '';
  $variables['errors'] = array('name' => '', 'pass' => '', 'securitycode' => '');
  if(isset($_POST['name']) && isset($_POST['pass'])){
    $variables['name'] = $_POST['name'];
    $variables['pass'] = $_POST['pass'];
    if(!isset($_POST['securitycode']) || empty($_POST['securitycode']) || !MiniFieldFile::generateSecurimage('check', $_POST['securitycode'])){
      $variables['errors']['securitycode'] = '验证码错误';
    }else{
      $result = CMSUser::LoginUser($_POST['name'], $_POST['pass']);
      if(is_array($result)){
        $variables['errors'] = array_merge($variables['errors'], $result);
      }else if(is_object($result) && isset($result->uid)){
        drupal_goto("cms/front");
      }
    }
  }
  echo MiniFieldCommon::Render(CMS_MODULE_PATH . '/tpl/cms-entrance.tpl.php', $variables);exit;
}