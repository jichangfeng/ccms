<?php
function cmssite_siteroot(){
  $page_callback_result = '';
  $cached = cache_get('cmscache_siteindex' . (isset($_GET['page'])?'_'.$_GET['page']:''));
  if($cached){
    $page_callback_result = $cached->data;
  }else{
    $filepath = CMSSiteInfo::GetAttr('themepath') . '/tpl/index.tpl.php';
    $page_callback_result = MiniFieldCommon::Render($filepath, array());
    $expire = 24 * 60 * 60;// one day
    //cache_set('cmscache_siteindex' . (isset($_GET['page'])?'_'.$_GET['page']:''), $page_callback_result, 'cache', time() + $expire);
  }
  echo $page_callback_result;exit;
}
function cmssite_sts(){
  $page_callback_result = '';
  $navtag = arg(1);// navigation id or urlalias
  $nodeid = arg(2);
  if(is_null($navtag) || empty($navtag) || !CMSNavigation::ExistNavigationByTag($navtag)){
    $page_callback_result = '页面不存在，请检查链接是否正确';
  }else{
    $Navigation = CMSNavigation::LoadNavigationByTag($navtag, true);
    if(is_null($nodeid) || !is_numeric($nodeid) || empty($Navigation->tplarticle)){
      $cached = cache_get('cmscache_sitenav_' . $navtag . (isset($_GET['page'])?'_'.$_GET['page']:''));
      if($cached){
        $page_callback_result = $cached->data;
      }else{
        if(isset($Navigation->seotitle) && !empty($Navigation->seotitle)){
          CMSSiteInfo::SetAttr('title', $Navigation->seotitle);
        }else{
          CMSSiteInfo::SetAttr('title', $Navigation->name . '_' . CMSSiteInfo::GetAttr('title', false));
        }
        if(isset($Navigation->seokeywords) && !empty($Navigation->seokeywords)){
          CMSSiteInfo::SetAttr('keywords', $Navigation->seokeywords);
        }
        if(isset($Navigation->seodescription) && !empty($Navigation->seodescription)){
          CMSSiteInfo::SetAttr('description', $Navigation->seodescription);
        }
        $page_callback_result = CMSNavigation::GetNavigationTplindexHtml($navtag);
        $expire = 24 * 60 * 60;// one day
        $becached = false;
        foreach($Navigation->tplcache as $value){
          if(isset($value[0]) && $value[0] == 'tplindex'){
            $becached = true;
            break;
          }
        }
        if($becached == true){
          cache_set('cmscache_sitenav_' . $navtag . (isset($_GET['page'])?'_'.$_GET['page']:''), $page_callback_result, 'cache', time() + $expire);
        }
      }
    }else{
      $cached = cache_get('cmscache_sitenode_' . $navtag . '_' . $nodeid . (isset($_GET['page'])?'_'.$_GET['page']:''));
      if($cached){
        $page_callback_result = $cached->data;
      }else{
        if(isset($Navigation->seotitle) && !empty($Navigation->seotitle)){
          CMSSiteInfo::SetAttr('title', $Navigation->seotitle);
        }else{
          CMSSiteInfo::SetAttr('title', $Navigation->name . '_' . CMSSiteInfo::GetAttr('title', false));
        }
        if(isset($Navigation->seokeywords) && !empty($Navigation->seokeywords)){
          CMSSiteInfo::SetAttr('keywords', $Navigation->seokeywords);
        }
        if(isset($Navigation->seodescription) && !empty($Navigation->seodescription)){
          CMSSiteInfo::SetAttr('description', $Navigation->seodescription);
        }
        $page_callback_result = CMSNavigation::GetNavigationTplarticleHtml($navtag, $nodeid);
        $expire = 24 * 60 * 60;// one day
        $becached = false;
        foreach($Navigation->tplcache as $value){
          if(isset($value[0]) && $value[0] == 'tplarticle'){
            $becached = true;
            break;
          }
        }
        if($becached == true){
          cache_set('cmscache_sitenode_' . $navtag . '_' . $nodeid . (isset($_GET['page'])?'_'.$_GET['page']:''), $page_callback_result, 'cache', time() + $expire);
        }
      }
    }
  }
  echo $page_callback_result;exit;
}

function cmssite_ulogin(){
  $variables = array();
  $filepath = CMSSiteInfo::GetAttr('themepath') . '/tpluser/login.tpl.php';
  echo MiniFieldCommon::Render($filepath, $variables);exit;
}
function cmssite_uregister(){
  $variables = array();
  $filepath = CMSSiteInfo::GetAttr('themepath') . '/tpluser/register.tpl.php';
  echo MiniFieldCommon::Render($filepath, $variables);exit;
}
function cmssite_uactivate(){
  $variables = array();
  $filepath = CMSSiteInfo::GetAttr('themepath') . '/tpluser/activate.tpl.php';
  echo MiniFieldCommon::Render($filepath, $variables);exit;
}
function cmssite_uresetpass(){
  $variables = array();
  $filepath = CMSSiteInfo::GetAttr('themepath') . '/tpluser/resetpass.tpl.php';
  echo MiniFieldCommon::Render($filepath, $variables);exit;
}

function cmssite_site403(){
  echo MiniFieldCommon::Render(CMSSITE_MODULE_PATH . '/tpl/403.tpl.php');exit;
}
function cmssite_site404(){
  echo MiniFieldCommon::Render(CMSSITE_MODULE_PATH . '/tpl/404.tpl.php');exit;
}