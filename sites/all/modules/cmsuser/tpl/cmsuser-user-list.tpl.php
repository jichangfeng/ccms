<div id="crumbs">
  <a href="<?php echo base_path().'cmsuser/user/'.$Role->rid.'/list';?>"><?php echo $Role->name;?></a> <em>›</em> <?php echo ts('列表', 'ucwords', 'default');?>
</div>
<div class="cdata">
  <div class="btnLink">
    <?php if(CMSRole::CanManageUser('add', $GLOBALS['user'], $Role)):?><a class="ico-button ico-add" href="<?php echo base_path().'cmsuser/user/'.$Role->rid.'/add';?>"><?php echo ts('添加用户', 'ucwords', 'user');?></a><?php endif;?>
    <?php if(cmsMenuAccessCheck('cmsuser/user/'.$Role->rid.'/clear')):?><a class="ico-button" href="<?php echo base_path().'cmsuser/user/'.$Role->rid.'/clear';?>"><?php echo ts('清理用户', 'ucwords', 'user');?></a><?php endif;?>
  </div>
  <div class="searchArea"><form id="searchform" method="get" action="">
    <select name="limit" onchange="document.getElementById('searchform').submit();" style="float: right;">
      <option<?php echo isset($_GET['limit'])&&$_GET['limit']=='5' ? ' selected="selected"' : '';?> value="5">5 条/页</option>
      <option<?php echo isset($_GET['limit'])&&$_GET['limit']=='10' ? ' selected="selected"' : '';?> value="10">10 条/页</option>
      <option<?php echo isset($_GET['limit'])&&$_GET['limit']=='20' ? ' selected="selected"' : '';?> value="20">20 条/页</option>
      <option<?php echo isset($_GET['limit'])&&$_GET['limit']=='50' ? ' selected="selected"' : '';?> value="50">50 条/页</option>
      <option<?php echo isset($_GET['limit'])&&$_GET['limit']=='100' ? ' selected="selected"' : '';?> value="100">100 条/页</option>
    </select>
    <?php if(isset($_GET['status'])):?><input type="hidden" name="status" value="<?php echo $_GET['status'];?>"/><?php endif;?>
    <select name="sortfield">
    <?php if(isset($arrFields)):?>
      <option value="uid"<?php echo isset($_GET['sortfield'])&&$_GET['sortfield']=='uid' ? ' selected="selected"' : '';?>>UID</option>
      <option value="name"<?php echo isset($_GET['sortfield'])&&$_GET['sortfield']=='name' ? ' selected="selected"' : '';?>><?php echo ts('用户名', 'ucwords', 'user');?></option>
      <option value="mail"<?php echo isset($_GET['sortfield'])&&$_GET['sortfield']=='mail' ? ' selected="selected"' : '';?>><?php echo ts('邮箱', 'ucwords', 'user');?></option>
      <option value="mobile"<?php echo isset($_GET['sortfield'])&&$_GET['sortfield']=='mobile' ? ' selected="selected"' : '';?>><?php echo ts('手机号', 'ucwords', 'user');?></option>
      <?php foreach($arrFields as $Field):?>
      <option<?php echo isset($_GET['sortfield'])&&$_GET['sortfield']==$Field->fieldname ? ' selected="selected"' : '';?> value="<?php echo $Field->fieldname;?>"><?php echo $Field->name;?></option>
      <?php endforeach;?>
    <?php endif;?>
    </select><select name="sortdirection">
      <option<?php echo isset($_GET['sortdirection'])&&$_GET['sortdirection']=='DESC' ? ' selected="selected"' : '';?> value="DESC"><?php echo ts('降序', 'ucwords', 'default');?></option>
      <option<?php echo isset($_GET['sortdirection'])&&$_GET['sortdirection']=='ASC' ? ' selected="selected"' : '';?> value="ASC"><?php echo ts('升序', 'ucwords', 'default');?></option>
    </select>
    <input type="text" name="sortfilter" value="<?php echo isset($_GET['sortfilter']) ? $_GET['sortfilter'] : '';?>"/>
    <input type="checkbox" name="sortmatch"<?php echo isset($_GET['sortmatch'])&&$_GET['sortmatch']=='1' ? ' checked="checked"' : '';?> value="1"/>完整匹配
    <input type="submit" value="<?php echo ts('搜索', 'ucwords', 'default');?>"/></form>
  </div>
  <div class="tabTitle">
    <span class="left"><?php echo ts('网站用户', 'ucwords', 'default');?>（<?php echo $GLOBALS['pager_total_items'][0];?>）</span>
    <span class="right">
      <a<?php echo !isset($_GET['status']) ? ' style="color: red;"' : '';?> href="<?php echo base_path().'cmsuser/user/'.$Role->rid.'/list';?>"><?php echo ts('全部用户', 'ucwords', 'user');?></a>
      | <a<?php echo (isset($_GET['status']) && $_GET['status'] == 0)? ' style="color: red;"' : '';?> href="<?php echo base_path().'cmsuser/user/'.$Role->rid.'/list';?>?status=0"><?php echo ts('被限制的用户', 'ucwords', 'user');?></a>
      | <a<?php echo (isset($_GET['status']) && $_GET['status'] == 1)? ' style="color: red;"' : '';?> href="<?php echo base_path().'cmsuser/user/'.$Role->rid.'/list';?>?status=1"><?php echo ts('正常用户', 'ucwords', 'user');?></a>
    </span>
  </div>
  <table>
    <thead>
      <tr>
        <th></th>
        <th>UID</th>
        <th><?php echo ts('用户名', 'ucwords', 'user');?></th>
        <th><?php echo ts('邮箱', 'ucwords', 'user');?></th>
        <th><?php echo ts('手机号', 'ucwords', 'user');?></th>
        <th><?php echo ts('创建时间', 'ucwords', 'user');?></th>
        <th><?php echo ts('状态', 'ucwords', 'user');?></th>
        <th><?php echo ts('角色', 'ucwords', 'user');?></th>
        <th style="display: none;">ID</th>
        <?php if(isset($arrFields)):?>
          <?php foreach($arrFields as $Field):?>
          <th><?php echo $Field->name;?></th>
          <?php endforeach;?>
          <th class="actions"><?php echo ts('操作', 'ucwords', 'default');?></th>
        <?php endif;?>
      </tr>
    </thead>
    <tbody>
    <?php if(isset($arrDatas)):?>
      <?php
        $canEditData = CMSRole::CanManageUser('edit', $GLOBALS['user'], $Role);
        $canDeleteData = CMSRole::CanManageUser('delete', $GLOBALS['user'], $Role);
      ?>
      <?php foreach($arrDatas as $key=>$Data):?>
      <tr class="<?php echo $key % 2 == 0 ? 'even' : 'odd';?>">
        <td><input class="ckb" type="checkbox" value="<?php echo $Data->uid;?>"></td>
        <td><?php echo $Data->uid;?></td>
        <td><?php echo $Data->name;?></td>
        <td><?php echo $Data->mail;?></td>
        <td><?php echo $Data->mobile;?></td>
        <td><?php echo date('Y-m-d', $Data->created);?></td>
        <td><?php echo $Data->status == 1 ? "<span style='color: green;'>" . ts('启用', 'ucwords', 'user') . "</span>" : "<span style='color: red;'>" . ts('禁用', 'ucwords', 'user') . "</span>";?></td>
        <td>
          <?php foreach($Data->roles as $Rid=>$RoleName):?>
            <?php if(in_array($Rid, array(DRUPAL_ANONYMOUS_RID, DRUPAL_AUTHENTICATED_RID))){continue;}?>
            [<?php echo $RoleName?>]
          <?php endforeach;?>
        </td>
        <td style="display: none;"><?php echo isset($Data->id)? $Data->id : '';?></td>
        <?php if(isset($arrFields)):?>
          <?php foreach($arrFields as $Field):?>
            <td><?php echo MiniFieldData::GetDataOutput($Data, $Field, array('image_width' => '50px'));?></td>
          <?php endforeach;?>
          <td>
            <?php if($canEditData):?><a class="ico-button ico-edit" href="<?php echo base_path().'cmsuser/user/'.$Role->rid.'/'.$Data->uid.'/edit';?>"><?php echo ts('编辑', 'ucwords', 'default');?></a><?php endif;?>
            <?php if($canDeleteData):?><a class="ico-button ico-delete" onclick="return confirm('<?php echo ts('确定要删除此用户吗？', 'ucfirst', 'user');?>');" href="<?php echo base_path().'cmsuser/user/'.$Role->rid.'/'.$Data->uid.'/del/confirm';?>"><?php echo ts('删除', 'ucwords', 'default');?></a><?php endif;?>
          </td>
        <?php endif;?>
      </tr>
      <?php endforeach;?>
    <?php endif;?>
    </tbody>
    <tfoot></tfoot>
  </table>
  <div class="chooseData">
    <a class="ico-button ico-spark" href="javascript:;" onclick="$('.ckb').attr('checked', 'checked');"><?php echo ts('全选', 'ucwords', 'default');?></a>
    <a class="ico-button ico-spark" href="javascript:;" onclick="$('.ckb').removeAttr('checked');"><?php echo ts('全反选', 'ucwords', 'default');?></a>
    <?php if(CMSRole::CanManageUser('delete', $GLOBALS['user'], $Role)):?><a class="ico-button deleteusers" href="javascript:;"><?php echo ts('删除', 'ucwords', 'default');?></a><?php endif;?>
  </div>
  <div class="pagerinfo"><?php echo MiniFieldCommon::HtmlPager();?></div>
</div>
<script>
$(function(){
  $('.deleteusers').click(DeleteUsers);
});
function DeleteUsers(){
  if($('.deleteusers').hasClass('disabled')){ return; }
  if(!confirm('<?php echo ts('确定要删除勾选的用户吗？', 'ucfirst', 'user');?>')){
    return;
  }
  var uides = new Array();
  $('.ckb:checked').each(function(){
    uides.push($(this).val());
  });
  if(uides.length == 0){ alert('<?php echo ts('请至少勾选一项', 'ucfirst', 'default');?>');return; }
  $('.deleteusers').addClass('disabled');
  $('.deleteusers').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  var url = '<?php echo base_path().'cmsuser/user/'.$Role->rid.'/delete';?>';
  $.post(url, {uides:uides}, function(ret){
    $('.deleteusers').removeClass('disabled');
    $('.deleteusers').removeAttr('title');
    if(ret.toString().indexOf('ok', 0) != -1){
      location.reload(1);
    }else{
      alert('<?php echo ts('删除', 'ucfirst', 'default');?> <?php echo ts('失败', 'strtolower', 'default');?>');
    }
  }, 'text').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
}
</script>