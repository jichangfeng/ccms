<?php
/**
 * Cms frame page
 */
?>
<!DOCTYPE HTML>
<html xmlns="http://www.w3.org/1999/xhtml">
  <head>
    <title><?php echo CMSSiteInfo::GetAttr('title');?></title>
    <?php echo isset(CMSSiteInfo::GetAttr('icon')[0])?'<link rel="shortcut icon" href="'.CMSSiteInfo::GetAttr('icon')[0]->filepath.'">':'';?>
    <meta name=keywords content="<?php echo CMSSiteInfo::GetAttr('keywords');?>">
    <meta name="description" content="<?php echo CMSSiteInfo::GetAttr('description');?>">
    <meta charset="UTF-8">
    <?php echo drupal_get_css(); ?>
    <?php echo drupal_get_js(); ?>
    <!--[if lt IE 9]>
    <script type="text/javascript" src="<?php echo base_path();?>misc/resources/html5.js"></script>
    <![endif]-->
    <!--[if lte IE 6]>
    <script type="text/javascript" src="<?php echo base_path();?>misc/resources/DD_belatedPNG/DD_belatedPNG_0.0.8a-min.js"></script>
    <script>
    DD_belatedPNG.fix('');
    </script>
    <![endif]-->
  </head>
  <body>
  <div id="wrap">
    <div id="top-header">
      <span class="left">
        <a<?php echo (isset($_COOKIE['clang']) && $_COOKIE['clang'] == 'zhs') ? ' class="current"' : '';?> onclick="$.cookie('clang', 'zhs', {expires: 7, path: '<?php echo base_path();?>'});location.reload(1);" href="javascript:;"><?php echo ts('简体中文', 'ucwords', 'translation');?></a>
        <a<?php echo (isset($_COOKIE['clang']) && $_COOKIE['clang'] == 'zht') ? ' class="current"' : '';?> onclick="$.cookie('clang', 'zht', {expires: 7, path: '<?php echo base_path();?>'});location.reload(1);" href="javascript:;"><?php echo ts('繁体中文', 'ucwords', 'translation');?></a>
        <a<?php echo (isset($_COOKIE['clang']) && $_COOKIE['clang'] == 'enus') ? ' class="current"' : '';?> onclick="$.cookie('clang', 'enus', {expires: 7, path: '<?php echo base_path();?>'});location.reload(1);" href="javascript:;"><?php echo ts('美国英文', 'ucwords', 'translation');?></a>
      </span>
      <span class="right">
        <span style="color: #2f8bc9;"><?php echo $GLOBALS['user']->name;?></span>（uid:<?php echo $GLOBALS['user']->uid;?>）
        <a href="<?php echo base_path();?>user/logout?destination=cms"><?php echo ts('注销', 'ucwords', 'default');?></a>
        | <a target="_blank" href="<?php echo base_path();?>"><?php echo ts('前台首页', 'ucwords', 'default');?></a>
      </span>
    </div>
    <div id="head">
      <div class="logo"><img src="<?php echo base_path().CMS_MODULE_PATH;?>/tpl/images/head_logo.png"></div>
      <?php
      $front = cmsMenuAccessCheck('cms/front');
      $translation = cmsMenuAccessCheck('cmsdata/1/list');
      $siteinfo = cmsMenuAccessCheck('cmsdata/2/list');
      $navigation = cmsMenuAccessCheck('cmsdata/3/list');
      $content = cmsMenuAccessCheck('cmsnode');
      $usermaterial = cmsMenuAccessCheck('cmsuser/usermaterial');
      $role = cmsMenuAccessCheck('cmsuser/role/list');
      $user = cmsMenuAccessCheck('cmsuser/user');
      $category = cmsMenuAccessCheck('cmsfield/category/list');
      $field = cmsMenuAccessCheck('cmsfield/field');
      $userfield = cmsMenuAccessCheck('cmsfield/userfield');
      $contentfield = cmsMenuAccessCheck('cmsfield/contentfield');
      ?>
      <ul class="nav">
        <?php if($front):?><li class="first<?php echo arg(0)=='cms'&&arg(1)=='front'?' current':'';?>"><a href="<?php echo base_path().'cms/front';?>"><?php echo ts('后台首页', 'ucwords', 'default');?></a></li><?php endif;?>
        <!--<?php if($translation):?><li<?php echo arg(0)=='cmsdata'&&arg(1)=='1'?' class="current"':'';?>><a href="<?php echo base_path().'cmsdata/1/list';?>"><?php echo ts('网站翻译', 'ucwords', 'default');?></a></li><?php endif;?>-->
        <?php if($siteinfo):?><li<?php echo arg(0)=='cmsdata'&&arg(1)=='2'?' class="current"':'';?>><a href="<?php echo base_path().'cmsdata/2/list';?>"><?php echo ts('网站信息', 'ucwords', 'default');?></a></li><?php endif;?>
        <?php if($navigation):?><li<?php echo arg(0)=='cmsdata'&&arg(1)=='3'?' class="current"':'';?>><a href="<?php echo base_path().'cmsdata/3/list';?>"><?php echo ts('网站栏目', 'ucwords', 'default');?></a></li><?php endif;?>
        <?php if($content):?><li<?php echo arg(0)=='cmsnode'?' class="current"':'';?>><a href="<?php echo base_path().'cmsnode';?>"><?php echo ts('栏目内容', 'ucwords', 'default');?></a></li><?php endif;?>
        <?php if($role):?><li<?php echo arg(0)=='cmsuser'&&arg(1)=='role'?' class="current"':'';?>><a href="<?php echo base_path().'cmsuser/role/list';?>"><?php echo ts('网站角色', 'ucwords', 'default');?></a></li><?php endif;?>
        <?php if($user):?><li class="last<?php echo arg(0)=='cmsuser'&&arg(1)=='user'?' current':'';?>"><a href="<?php echo base_path().'cmsuser/user';?>"><?php echo ts('网站用户', 'ucwords', 'default');?></a></li><?php endif;?>
      </ul>
    </div>
    <div id="messages"><?php echo theme_status_messages(array('display' => null));?></div>
    <div id="side-menu">
      <?php if($translation || $siteinfo || $navigation || $content):?>
      <div class="menu_box"><p><?php echo ts('内容管理', 'ucwords', 'default');?></p><span></span>
        <ul>
          <?php if($translation):?><li<?php echo arg(0)=='cmsdata'&&arg(1)=='1'?' class="current"':'';?>><a href="<?php echo base_path().'cmsdata/1/list';?>"><?php echo ts('网站翻译', 'ucwords', 'default');?></a></li><?php endif;?>
          <?php if($siteinfo):?><li<?php echo arg(0)=='cmsdata'&&arg(1)=='2'?' class="current"':'';?>><a href="<?php echo base_path().'cmsdata/2/list';?>"><?php echo ts('网站信息', 'ucwords', 'default');?></a></li><?php endif;?>
          <?php if($navigation):?><li<?php echo arg(0)=='cmsdata'&&arg(1)=='3'?' class="current"':'';?>><a href="<?php echo base_path().'cmsdata/3/list';?>"><?php echo ts('网站栏目', 'ucwords', 'default');?></a></li><?php endif;?>
          <?php if($content):?><li<?php echo arg(0)=='cmsnode'?' class="current"':'';?>><a href="<?php echo base_path().'cmsnode';?>"><?php echo ts('栏目内容', 'ucwords', 'default');?></a></li><?php endif;?>
          <?php foreach(CMSDataField::$AllowedCides as $cid => $cname):?>
            <?php if(in_array($cid, array(1, 2, 3))){ continue; }?>
            <?php if(cmsMenuAccessCheck('cmsdata/'.$cid.'/list')):?><li<?php echo arg(0)=='cmsdata'&&arg(1)==$cid?' class="current"':'';?>><a href="<?php echo base_path().'cmsdata/'.$cid.'/list';?>"><?php echo ts($cname, 'ucwords', 'all');?></a></li><?php endif;?>
          <?php endforeach;?>
        </ul>
      </div>
      <?php endif;?>
      <?php if($usermaterial || $role || $user):?>
      <div class="menu_box"><p><?php echo ts('会员管理', 'ucwords', 'default');?></p><span></span>
        <ul>
          <?php if($usermaterial):?><li<?php echo arg(0)=='cmsuser'&&arg(1)=='usermaterial'?' class="current"':'';?>><a href="<?php echo base_path().'cmsuser/usermaterial';?>"><?php echo ts('用户资料', 'ucwords', 'default');?></a></li><?php endif;?>
          <?php if($role):?><li<?php echo arg(0)=='cmsuser'&&arg(1)=='role'?' class="current"':'';?>><a href="<?php echo base_path().'cmsuser/role/list';?>"><?php echo ts('网站角色', 'ucwords', 'default');?></a></li><?php endif;?>
          <?php if($user):?><li<?php echo arg(0)=='cmsuser'&&arg(1)=='user'?' class="current"':'';?>><a href="<?php echo base_path().'cmsuser/user';?>"><?php echo ts('网站用户', 'ucwords', 'default');?></a></li><?php endif;?>
        </ul>
      </div>
      <?php endif;?>
      <?php if($category || $field || $userfield || $contentfield):?>
      <div class="menu_box"><p><?php echo ts('字段管理', 'ucwords', 'default');?></p><span></span>
        <ul>
          <?php if($category):?><li<?php echo arg(0)=='cmsfield'&&arg(1)=='category'?' class="current"':'';?>><a href="<?php echo base_path().'cmsfield/category/list';?>"><?php echo ts('网站类别', 'ucwords', 'default');?></a></li><?php endif;?>
          <?php if($field):?><li<?php echo arg(0)=='cmsfield'&&arg(1)=='field'?' class="current"':'';?>><a href="<?php echo base_path().'cmsfield/field';?>"><?php echo ts('网站字段', 'ucwords', 'default');?></a></li><?php endif;?>
          <?php if($userfield):?><li<?php echo arg(0)=='cmsfield'&&arg(1)=='userfield'?' class="current"':'';?>><a href="<?php echo base_path().'cmsfield/userfield';?>"><?php echo ts('网站用户字段', 'ucwords', 'default');?></a></li><?php endif;?>
          <?php if($contentfield):?><li<?php echo arg(0)=='cmsfield'&&arg(1)=='contentfield'?' class="current"':'';?>><a href="<?php echo base_path().'cmsfield/contentfield';?>"><?php echo ts('网站内容字段', 'ucwords', 'default');?></a></li><?php endif;?>
        </ul>
      </div>
      <?php endif;?>
    </div>
    <div class="frame-main">
      <?php echo $page_callback_result;?>
    </div>
  </div>
  <div id="footer">CCMS内容管理系统(c)版权所有 姬先生。联系邮箱：<a href="mailto:jichf@qq.com">jichf@qq.com</a></div>
  </body>
</html>
