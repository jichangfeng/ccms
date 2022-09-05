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
          <th colspan="2">账户注册</th>
        </tr>
      </thead>
      <tbody>
        <tr>
          <td class="login" colspan="2">已拥有账户？<a href="<?php echo base_path();?>ulogin<?php echo isset($_GET['destination']) ? '?destination=' . $_GET['destination'] : '';?>">登录</a></td>
        </tr>
        <tr>
          <td class="label">角色：</td>
          <td class="roles">
            <label><input type="radio" name="roles" value="5"/>机构</label>
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            <label><input type="radio" name="roles" value="6" checked="checked"/>个人</label>
            <span class="error roles"></span>
          </td>
        </tr>
        <tr>
          <td class="label">用户名：</td>
          <td class="item">
            <input type="text" name="name" value=""/>
            <span class="error name"></span>
          </td>
        </tr>
        <tr>
          <td class="label">邮箱：</td>
          <td class="item">
            <input type="text" name="mail" value=""/>
            <span class="error mail"></span>
          </td>
        </tr>
        <tr>
          <td class="label">手机号：</td>
          <td class="item">
            <input type="text" name="mobile" value=""/>
            <span class="error mobile"></span>
          </td>
        </tr>
        <tr>
          <td class="label">密码：</td>
          <td class="item">
            <input type="password" name="pass" value=""/>
            <span class="error pass"></span>
          </td>
        </tr>
        <tr>
          <td class="label">确认密码：</td>
          <td class="item">
            <input type="password" name="pass2" value=""/>
            <span class="error pass2"></span>
          </td>
        </tr>
        <tr>
          <td class="label"></td>
          <td class="action">
            <input type="submit" onclick="register()" value="注 册"/>
          </td>
        </tr>
      </tbody>
    </table></form>
    <style type="text/css">
      #formtable{margin: 50px auto;border-collapse: collapse;border-spacing: 0;}
      #formtable th{text-align: center;padding: 20px;font-size: 18px;}
      #formtable td{padding-top: 15px;}
      #formtable td.login{text-align: center;color: #999;padding: 5px 0;}
      #formtable td.login a{color: blue;}
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
      function register(){
        var roles = $('#formtable').find('input[name=roles]:checked').val();
        var name = $('#formtable').find('input[name=name]').val();
        var mail = $('#formtable').find('input[name=mail]').val();
        var mobile = $('#formtable').find('input[name=mobile]').val();
        var pass = $('#formtable').find('input[name=pass]').val();
        var pass2 = $('#formtable').find('input[name=pass2]').val();
        $('#formtable').find('.error').text('');
        if($.trim(name).length === 0){
          $('#formtable').find('.error.name').text('账户不能留空');return;
        }
        if($.trim(mail).length === 0 && $.trim(mobile).length === 0){
          $('#formtable').find('.error.mail').text('邮箱和手机号至少填写一项');
          $('#formtable').find('.error.mobile').text('邮箱和手机号至少填写一项');
          return;
        }
        if($.trim(pass).length === 0){
          $('#formtable').find('.error.pass').text('密码不能留空');return;
        }
        if(pass !== pass2){
          $('#formtable').find('.error.pass').text('两次密码输入不一致');return;
        }
        var url = Drupal.settings.basePath + 'cmsuser/user/0/register';
        $.post(url, {roles:roles, name:name, mail:mail, mobile:mobile, pass:pass}, function(ret){
          if(ret && ret.uid > 0 && ret.status == 1){
            //alert('注册成功！');
            location.href = '<?php echo isset($_GET['destination']) ? $_GET['destination'] : base_path();?>';
          }else if(ret && ret.uid > 0 && ret.status != 1){
            //alert('注册成功！前往激活');
            location.href = Drupal.settings.basePath + 'uactivate?uid=' + ret.uid;
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