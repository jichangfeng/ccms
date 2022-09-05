<?php
/**
 * @author Changfeng Ji <jichf@qq.com>
 */
function cmsframe($page_callback_result){
  if (is_int($page_callback_result)) {
    switch ($page_callback_result) {
      case MENU_NOT_FOUND:
        drupal_deliver_html_page($page_callback_result);
        break;
      case MENU_ACCESS_DENIED:
        drupal_deliver_html_page($page_callback_result);
        break;
      case MENU_SITE_OFFLINE:
        drupal_deliver_html_page($page_callback_result);
        break;
    }
  }
  echo MiniFieldCommon::Render(CMS_MODULE_PATH . '/tpl/cms-frame.tpl.php', array('page_callback_result' => $page_callback_result));
  exit;
}

/**
 * 
 * @param string $path
 * @return bool
 * @author Changfeng Ji <jichf@qq.com>
 */
function cmsMenuAccessCheck($path = null){
  return MiniFieldCommon::MenuAccessCheck($path);
}

/**
 * 
 * @param $data
 * @param $options
 * @return array
 * @see drupal_add_css()
 * @author Changfeng Ji <jichf@qq.com>
 */
function cmsAddCss($data = NULL, $options = NULL){
  $StaticName = __FUNCTION__;
  $css = MiniFieldStatic::GetStatic($StaticName, false);
  if(!isset($css)){
    $css = array('modules/system/system.messages.css');
  }
  if(is_null($data)){
    return $css;
  }
  drupal_add_css($data, $options);
  if(is_string($options) && !in_array($options, array('file', 'external'))){
    return $css;
  }else if(is_array($options) && isset($options['type']) && !in_array($options['type'], array('file', 'external'))){
    return $css;
  }
  $css[] = $data;
  MiniFieldStatic::SetStatic($StaticName, $css, false);
  return $css;
}

/**
 * 
 * @param $data
 * @param $options
 * @return array
 * @see drupal_add_js()
 * @author Changfeng Ji <jichf@qq.com>
 */
function cmsAddJs($data = NULL, $options = NULL){
  $StaticName = __FUNCTION__;
  $js = MiniFieldStatic::GetStatic($StaticName, false);
  if(!isset($js)){
    $js = array('misc/jquery.js', 'misc/jquery.once.js', 'misc/drupal.js', 'settings', 'modules/user/user.permissions.js');
  }
  if(is_null($data)){
    return $js;
  }
  drupal_add_js($data, $options);
  if(is_string($options) && !in_array($options, array('file', 'external'))){
    return $js;
  }else if(is_array($options) && isset($options['type']) && !in_array($options['type'], array('file', 'external'))){
    return $js;
  }
  $js[] = $data;
  MiniFieldStatic::SetStatic($StaticName, $js, false);
}