<?php
cmsAddCss('misc/resources/jquery/poshytip/src/tip-yellowsimple/tip-yellowsimple.css');
cmsAddJs('misc/resources/jquery/poshytip/src/jquery.poshytip.min.js');
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
            <span class="step current"><i>1</i> 输入账户名</th></span>
            <b>&gt;</b>
            <span class="step"><i>2</i> 邮箱验证+激活账户</span>
          </td>
        </tr>
        <tr>
          <td class="label">输入账户名：</td>
          <td class="item">
            <input type="text" name="key" value="" placeholder="用户名/邮箱" title="忘记用户名？就使用邮箱吧！"/>
          </td>
        </tr>
        <tr>
          <td class="label"></td>
          <td class="action">
            <input type="submit" onclick="getinfo()" value="下一步"/>
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
      $(function(){
        $('input[name=key]').poshytip({
          className: 'tip-yellowsimple',
          showOn: 'focus',
          alignTo: 'target',
          alignX: 'right',
          alignY: 'center',
          offsetX: 5
        });
      });
      function getinfo(){
        $('#formtable .status').text('');
        var key = $('input[name=key]').val();
        var url = Drupal.settings.basePath + 'cmsuser/user/0/getinfo';
        $.post(url, {key:key}, function(ret){
          if(ret.uid > 0){
            location.href = Drupal.settings.basePath + 'uactivate?uid=' + ret.uid;
          }else{
            $('#formtable .status').css('color', 'red');
            $('#formtable .status').text('账户不存在');
          }
        }, 'json').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
      }
    </script>
  </body>
</html>