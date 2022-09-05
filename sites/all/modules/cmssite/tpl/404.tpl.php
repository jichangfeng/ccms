<?php cmssite_init();?>
<!DOCTYPE HTML>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
  <title>404 - <?php echo CMSSiteInfo::GetAttr('title');?></title>
  <?php echo isset(CMSSiteInfo::GetAttr('icon')[0])?'<link rel="shortcut icon" href="'.CMSSiteInfo::GetAttr('icon')[0]->filepath.'">':'';?>
  <meta name=keywords content="<?php echo CMSSiteInfo::GetAttr('keywords');?>">
  <meta name="description" content="<?php echo CMSSiteInfo::GetAttr('description');?>">
  <meta charset="UTF-8">
  <?php echo drupal_get_css(); ?>
  <?php echo drupal_get_js(); ?>
  <style type="text/css">
  .body{ margin:0 auto;}
  .q{ margin-top:200px; text-align:center;}
  </style>
</head>
<body>
  <div class="q"><img src="<?php echo base_path().CMSSITE_MODULE_PATH.'/tpl/images/404.jpg';?>"/></div>
</body>
</html>
