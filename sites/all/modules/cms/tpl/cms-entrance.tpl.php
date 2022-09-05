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
  </head>
  <body>
    <div id="messages"><?php echo theme_status_messages(array('display' => null));?></div>
    <form action="" method="post"><table id="formtable">
      <thead>
        <tr>
          <th colspan="2">账户登录</th>
        </tr>
      </thead>
      <tbody>
        <tr>
          <td class="label">账户：</td>
          <td class="item">
            <input type="text" name="name" value="<?php echo $name;?>" placeholder="用户名/邮箱/手机号"/>
            <span class="error"><?php echo $errors['name'];?></span>
          </td>
        </tr>
        <tr>
          <td class="label">密码：</td>
          <td class="item">
            <input type="password" name="pass" value="<?php echo $pass;?>" placeholder="密码"/>
            <span class="error"><?php echo $errors['pass'];?></span>
          </td>
        </tr>
        <tr>
          <td class="label">验证码：</td>
          <td class="item">
            <input type="text" name="securitycode" value="" placeholder="验证码" size="10" autocomplete="off"/>
            <span class="error"><?php echo $errors['securitycode'];?></span>
          </td>
        </tr>
        <tr>
          <td class="label"></td>
          <td class="item" style="padding-top: 0;">
            <img id="siimage" src="<?php echo base_path();?>cmsdata/file/securimage/show?sid=<?php echo md5(uniqid()) ?>" alt="CAPTCHA Image">
            <object id="siobject" style="width: 32px;height: 32px;" type="application/x-shockwave-flash" data="<?php echo base_path();?>misc/resources/securimage/securimage_play.swf?audio_file=<?php echo base_path();?>cmsdata/file/securimage/play">
              <param name="movie" value="<?php echo base_path();?>misc/resources/securimage/securimage_play.swf?audio_file=<?php echo base_path();?>cmsdata/file/securimage/play">
            </object>
            <a href="javascript:;" title="Refresh Image" onclick="document.getElementById('siimage').src = '<?php echo base_path();?>cmsdata/file/securimage/show?sid=' + Math.random(); this.blur(); return false;">
              <img src="<?php echo base_path();?>misc/resources/securimage/images/refresh.png" alt="Reload Image" onclick="this.blur()">
            </a>
          </td>
        </tr>
        <tr>
          <td class="label"></td>
          <td class="action">
            <input type="submit" value="登 录"/>
          </td>
        </tr>
      </tbody>
    </table></form>
  </body>
</html>