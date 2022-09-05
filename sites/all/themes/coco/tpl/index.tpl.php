<!DOCTYPE HTML>
<?php $themepath = base_path().CMSSiteInfo::GetAttr('themepath');?>
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
    DD_belatedPNG.fix('#header img,.nav dl');
    </script>
    <![endif]-->
  </head>
<body>
  <div id="messages" style="display: none;"><?php echo theme_status_messages(array('display' => null));?></div>
  <header id="header">
    <div id="logo"><!-- put logo here --></div>
    <nav id="nav">
      <!-- put navigation here -->
      <?php echo cmssiteGenerateNavs();?>
    </nav>
  </header>
  <!-- put content here -->
  <footer id="footer">
    <p>&copy;201X XXXXXXXX <im style="font-style: normal;color: red;">联系电话：(XXX)XXXXXXXX</im> 京ICP备XXXXXXX号</p>
  </footer>
</body>
</html>