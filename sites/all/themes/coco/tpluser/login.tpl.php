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
          <th colspan="2">账户登录</th>
        </tr>
      </thead>
      <tbody>
        <tr>
          <td class="register" colspan="2">还没有账户？<a href="<?php echo base_path();?>uregister<?php echo isset($_GET['destination']) ? '?destination=' . $_GET['destination'] : '';?>">注册</a></td>
        </tr>
        <tr>
          <td class="label">账户：</td>
          <td class="item">
            <input type="text" name="name" value="" placeholder="用户名/邮箱/手机号"/>
            <span class="error name"></span>
          </td>
        </tr>
        <tr>
          <td class="label">密码：</td>
          <td class="item">
            <input type="password" name="pass" value="" placeholder="密码"/>
            <span class="error pass"></span>
          </td>
        </tr>
        <tr>
          <td class="label"></td>
          <td class="action">
            <input type="submit" onclick="login()" value="登 录"/>
            <!--<a style="color: blue;vertical-align: bottom;" href="<?php echo base_path() . 'uactivate';?>">激活账户</a>-->
            <a style="color: blue;vertical-align: bottom;" href="<?php echo base_path() . 'uresetpass';?>">找回密码</a>
          </td>
        </tr>
      </tbody>
    </table></form>
    <style type="text/css">
      #formtable{margin: 50px auto;border-collapse: collapse;border-spacing: 0;}
      #formtable th{text-align: center;padding: 20px;font-size: 18px;}
      #formtable td{padding-top: 15px;}
      #formtable td.register{text-align: center;color: #999;padding: 5px 0;}
      #formtable td.register a{color: blue;}
      #formtable td.label{text-align: right;font-size: 14px;width: 180px;}
      #formtable td.item{text-align: left;width: 350px;}
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
      #formtable span.error{color: red;}
    </style>
    <script type="text/javascript">
      function login(){
        var name = $('#formtable').find('input[name=name]').val();
        var pass = $('#formtable').find('input[name=pass]').val();
        $('#formtable').find('.error').text('');
        if($.trim(name).length === 0){
          $('#formtable').find('.error.name').text('账户不能留空');return;
        }
        if($.trim(pass).length === 0){
          $('#formtable').find('.error.pass').text('密码不能留空');return;
        }
        var url = Drupal.settings.basePath + 'cmsuser/user/0/login';
        $.post(url, {name:name, pass:pass}, function(ret){
          if(ret && ret.uid > 0){
            //alert('登录成功！');
            location.href = '<?php echo isset($_GET['destination']) ? $_GET['destination'] : base_path();?>';
          }else{
            for(var j in ret){
              $('#formtable').find('.error.' + j).text(ret[j]);
            }
          }
        }, 'json').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
      }
    </script>
  </body>
</html>