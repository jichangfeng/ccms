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
    <div id="messages" style="display: none;"><?php echo theme_status_messages(array('display' => null));?></div>
    <form action="" method="post" onsubmit="return false;"><table id="formtable">
      <thead>
        <tr>
          <th colspan="2">激活账户</th>
        </tr>
      </thead>
      <tbody>
        <tr>
          <td class="steps" colspan="2">
            <span class="step"><i>1</i> 输入账户名</th></span>
            <b>&gt;</b>
            <span class="step current"><i>2</i> 验证+激活账户</span>
          </td>
        </tr>
        <tr>
          <td class="label"><?php echo $account->name;?>，您的邮箱：</td>
          <td class="item">
            <span class="mail"><?php echo preg_replace('/(?<=.).(?=.*@)/u', '*', $account->mail);?></span>
            <button class="sendmail" type="button" onclick="activate_apply(<?php echo $account->uid;?>)">免费获取验证码</button>
          </td>
        </tr>
        <tr>
          <td class="label">输入验证码：</td>
          <td class="item">
            <input type="text" name="activatecode" value=""/>
          </td>
        </tr>
        <tr>
          <td class="label"></td>
          <td class="action">
            <input type="submit" onclick="activate_validate(<?php echo $account->uid;?>)" value="确 定"/>
            <span class="status"></span>
          </td>
        </tr>
      </tbody>
    </table></form>
    <style type="text/css">
      #formtable{margin: 50px auto;border-collapse: collapse;border-spacing: 0;}
      #formtable th{text-align: center;padding: 20px;font-size: 18px;}
      #formtable td{padding-top: 15px;}
      #formtable td.steps{text-align: center;padding: 5px 0;}
      #formtable td.steps .step{padding: 20px;color: #999;font-size: 14px;}
      #formtable td.steps .step.current{color: #ff6824;}
      #formtable td.steps .step i{
        display: inline-block;
        margin: 2px;
        padding: 3px 8px;
        border-radius: 15px;
        background-color: gray;
        font-weight: bold;
        color: #fff;
        font-size: 14px;
      }
      #formtable td.steps .step.current i{color: #ff6824;}
      #formtable td.label{text-align: right;font-size: 14px;width: 180px;}
      #formtable td.item{text-align: left;width: 350px;}
      #formtable td.item .mail{font-size: 14px;color: #404040;}
      #formtable td.item .sendmail{
        height: 31px;
        line-height: 31px;
        border: none;
        font-size: 14px;
        color: #fff;
        background-color: #ff6824;
        cursor: pointer;
      }
      #formtable td.item input{
        border: 1px solid #C8C8C8;
        padding: 4px 3px;
        width: 180px;
        height: 16px;
        line-height: 16px;
        font-size: 100%;
      }
      #formtable td.action{text-align: left;}
      #formtable td.action input{
        display: inline-block;
        height: 31px;
        line-height: 31px;
        padding: 0 10px;
        border: none;
        font-size: 14px;
        color: #fff;
        background-color: #ff6824;
        cursor: pointer;
        overflow: hidden;
      }
      #formtable span.status{vertical-align: bottom;}
    </style>
    <script type="text/javascript">
      function activate_apply(uid){
        $('#formtable .status').text('');
        var url = Drupal.settings.basePath + 'cmsuser/user/0/activate';
        $.post(url, {uid:uid, action:'apply', officialname:'彩带社区'}, function(ret){
          if(ret.status === 'success'){
            $('#formtable .status').css('color', 'green');
          }else{
            $('#formtable .status').css('color', 'red');
          }
          $('#formtable .status').text(ret.text);
        }, 'json').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
      }
      function activate_validate(uid){
        $('#formtable .status').text('');
        var activatecode = $('input[name=activatecode]').val();
        var url = Drupal.settings.basePath + 'cmsuser/user/0/activate';
        $.post(url, {uid:uid, action:'validate', activatecode:activatecode}, function(ret){
          if(ret.status === 'success'){
            $('#formtable .status').css('color', 'green');
            $('#formtable .status').html(ret.text + ' <a style="vertical-align: bottom;" href="' + Drupal.settings.basePath + 'ulogin">【点击登录】</a>');
            $('input[name=activatecode]').val('');
          }else{
            $('#formtable .status').css('color', 'red');
            $('#formtable .status').text(ret.text);
          }
        }, 'json').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
      }
    </script>
  </body>
</html>