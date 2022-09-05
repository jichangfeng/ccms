<?php
/**
 *
 * @param type $handle handle style (list, add, edit, del)
 * @param type $rid role identification number
 * @return type
 */
function cmsuser_role($handle, $rid = 0){
  $arrHandles = array('list', 'add', 'edit', 'del');
  if(!in_array($handle, $arrHandles)){
    return $handle . ' 处理方式未定义！';
  }
  unset ($arrHandles);
  $filepath = "cmsuser.role.$handle.inc";
  if(!file_exists(CMSUSER_MODULE_PATH . "/inc/" . $filepath)){
    return CMSUSER_MODULE_PATH . "/inc/" . $filepath . ' 文件未找到！';
  }
  include $filepath;
  $function = "cmsuser_role_" . $handle;
  if(function_exists($function)){
    return $rid == 0 ? $function() : $function($rid);
  }else{
    return $handle . ' 处理函数未定义！';
  }
}

function cmsuser_role_handle($arg){
  if(is_string($arg) && $arg == 'clear'){
    CMSRole::ClearRoles();
    drupal_goto("cmsuser/role/list");return;
  }else if(is_string($arg) && $arg == 'delete'){
    if(isset($_POST['rides']) && is_array($_POST['rides'])){
      CMSRole::DeleteRolesByRides($_POST['rides']);
    }
    echo 'ok';exit;
  }else if(is_string($arg) && $arg == 'permissions'){
    require_once drupal_get_path('module', 'user') . '/user.admin.inc';
    $page_callback_result = drupal_get_form('user_admin_permissions');
    $output = drupal_render_page($page_callback_result);
    $html = new DOMDocument();
    $html->loadHTML($output);
    $newhtml = new DOMDocument();
    /* $nodelist = $html->getElementsByTagName('head');
    $nodelist = $nodelist->item(0)->childNodes;
    for($i = 0;$i < $nodelist->length;$i++){
      if(in_array($nodelist->item($i)->nodeName, array('title', 'link', 'meta', 'style', 'script'))){ continue; }
      $newnode = $newhtml->importNode($nodelist->item($i), true);
      $newhtml->appendChild($newnode);
    } */
    $element = $html->getElementById('user-admin-permissions');
    $newnode = $newhtml->importNode($element, true);
    $newhtml->appendChild($newnode);
    $content = $newhtml->saveHTML();
    $variables = array(
      'content' => $content
    );
    $filepath = CMSUSER_MODULE_PATH."/tpl/cmsuser-role-permissions.tpl.php";
    return MiniFieldCommon::Render($filepath, $variables);
  }
  exit;
}