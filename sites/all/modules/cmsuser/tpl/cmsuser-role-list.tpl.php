<div id="crumbs">
  <a href="<?php echo base_path().'cmsuser/role/list';?>"><?php echo ts('网站角色', 'ucwords', 'default');?></a> <em>›</em> <?php echo ts('列表', 'ucwords', 'default');?>
</div>
<div class="cdata">
  <div class="btnLink">
    <?php if(cmsMenuAccessCheck('cmsuser/role/add')):?><a class="ico-button ico-add" href="<?php echo base_path().'cmsuser/role/add';?>"><?php echo ts('添加角色', 'ucwords', 'role');?></a><?php endif;?>
    <?php if(cmsMenuAccessCheck('cmsuser/role/clear')):?><a class="ico-button" href="<?php echo base_path().'cmsuser/role/clear';?>"><?php echo ts('清理角色', 'ucwords', 'role');?></a><?php endif;?>
    <?php if(cmsMenuAccessCheck('cmsuser/role/permissions')):?><a class="ico-button" href="<?php echo base_path().'cmsuser/role/permissions';?>"><?php echo ts('角色权限', 'ucwords', 'role');?></a><?php endif;?>
  </div>
  <!--<div class="searchArea"></div>-->
  <div class="tabTitle">
    <span class="left"><?php echo ts('网站角色', 'ucwords', 'default');?>（<?php echo ts('角色下存在用户时，不能删除', 'ucfirst', 'role');?>）</span>
    <span class="right"></span>
  </div>
  <table>
    <thead>
      <tr>
        <th></th>
        <th>RID</th>
        <th><?php echo ts('角色名', 'ucwords', 'role');?></th>
        <th><?php echo ts('权重', 'ucwords', 'default');?></th>
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
        $canEditData = cmsMenuAccessCheck('cmsuser/role/%/edit');
        $canDeleteData = cmsMenuAccessCheck('cmsuser/role/%/del');
      ?>
      <?php foreach($arrDatas as $key=>$Data):?>
      <tr class="<?php echo $key % 2 == 0 ? 'even' : 'odd';?>">
        <td><input class="ckb" type="checkbox" value="<?php echo $Data->rid;?>"></td>
        <td><?php echo $Data->rid;?></td>
        <td><?php echo $Data->name;?></td>
        <td><?php echo $Data->weight;?></td>
        <td style="display: none;"><?php echo $Data->id;?></td>
        <?php if(isset($arrFields)):?>
          <?php foreach($arrFields as $Field):?>
            <td><?php echo MiniFieldData::GetDataOutput($Data, $Field, array('image_width' => '50px'));?></td>
          <?php endforeach;?>
          <td>
            <?php if($canEditData):?><a class="ico-button ico-edit" href="<?php echo base_path().'cmsuser/role/'.$Data->rid.'/edit';?>"><?php echo ts('编辑', 'ucwords', 'default');?></a><?php endif;?>
            <?php if($canDeleteData):?>
              <?php if(!CMSRole::CanDeleted($Data->rid)):?>
              <a class="ico-button ico-delete" onclick="alert('<?php echo ts('当前角色下存在用户，不能删除', 'ucwords', 'role');?>');return false;" href="#"><?php echo ts('删除', 'ucwords', 'default');?></a>
              <?php else:?>
              <a class="ico-button ico-delete" onclick="return confirm('<?php echo ts('确定要删除此角色吗？', 'ucfirst', 'role');?>');" href="<?php echo base_path().'cmsuser/role/'.$Data->rid.'/del/confirm';?>"><?php echo ts('删除', 'ucwords', 'default');?></a>
              <?php endif;?>
            <?php endif;?>
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
    <?php if(cmsMenuAccessCheck('cmsuser/role/delete')):?><a class="ico-button deleteroles" href="javascript:;"><?php echo ts('删除', 'ucwords', 'default');?></a><?php endif;?>
  </div>
  <!--<div class="pagerinfo"><?php //echo MiniFieldCommon::HtmlPager();?></div>-->
</div>
<script>
$(function(){
  $('.deleteroles').click(DeleteRoles);
});
function DeleteRoles(){
  if($('.deleteroles').hasClass('disabled')){ return; }
  if(!confirm('<?php echo ts('确定要删除勾选的角色吗？', 'ucfirst', 'role');?>')){
    return;
  }
  var rides = new Array();
  $('.ckb:checked').each(function(){
    rides.push($(this).val());
  });
  if(rides.length == 0){ alert('<?php echo ts('请至少勾选一项', 'ucfirst', 'default');?>');return; }
  $('.deleteroles').addClass('disabled');
  $('.deleteroles').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  var url = '<?php echo base_path().'cmsuser/role/delete';?>';
  $.post(url, {rides:rides}, function(ret){
    $('.deleteroles').removeClass('disabled');
    $('.deleteroles').removeAttr('title');
    if(ret.toString().indexOf('ok', 0) != -1){
      location.reload(1);
    }else{
      alert('<?php echo ts('删除', 'ucfirst', 'default');?> <?php echo ts('失败', 'strtolower', 'default');?>');
    }
  }, 'text').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
}
</script>