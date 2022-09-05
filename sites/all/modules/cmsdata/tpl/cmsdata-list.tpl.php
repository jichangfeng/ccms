<div id="crumbs">
  <a href="<?php echo base_path().'cmsdata/'.$Category->id.'/list';?>"><?php echo $Category->name;?></a> <em>›</em> <?php echo ts('列表', 'ucwords', 'default');?>
</div>
<div class="cdata">
  <div class="btnLink">
    <?php if(cmsMenuAccessCheck('cmsdata/'.$Category->id.'/add')):?><a class="ico-button ico-add" href="<?php echo base_path().'cmsdata/'.$Category->id.'/add';?>"><?php echo ts('添加', 'ucwords', 'default');?></a><?php endif;?>
  </div>
  <!--<div class="searchArea"></div>-->
  <div class="tabTitle">
    <span class="left"><?php echo $Category->name;?>（<?php echo $GLOBALS['pager_total_items'][0];?>）</span>
    <span class="right"></span>
  </div>
  <table>
    <thead>
      <tr>
        <th></th>
        <th>ID</th>
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
        $canEditData = cmsMenuAccessCheck('cmsdata/'.$Category->id.'/%/edit');
        $canDeleteData = cmsMenuAccessCheck('cmsdata/'.$Category->id.'/%/delete');
      ?>
      <?php foreach($arrDatas as $key=>$Data):?>
      <tr class="<?php echo $key % 2 == 0 ? 'even' : 'odd';?>">
        <td><input class="ckb" type="checkbox" value="<?php echo $Data->id;?>"></td>
        <td><?php echo $Data->id;?></td>
        <?php if(isset($arrFields)):?>
          <?php foreach($arrFields as $Field):?>
            <td><?php echo MiniFieldData::GetDataOutput($Data, $Field, array('image_width' => '50px'));?></td>
          <?php endforeach;?>
          <td>
            <?php if($canEditData):?><a class="ico-button ico-edit" href="<?php echo base_path().'cmsdata/'.$Category->id.'/'.$Data->id.'/edit';?>"><?php echo ts('编辑', 'ucwords', 'default');?></a><?php endif;?>
            <?php if($canDeleteData):?><a class="ico-button ico-delete" onclick="return confirm('确定要删除此数据吗？');" href="<?php echo base_path().'cmsdata/'.$Category->id.'/'.$Data->id.'/delete/confirm';?>"><?php echo ts('删除', 'ucwords', 'default');?></a><?php endif;?>
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
    <?php if(cmsMenuAccessCheck('cmsdata/'.$Category->id.'/delete')):?><a class="ico-button deletedatas" href="javascript:;"><?php echo ts('删除', 'ucwords', 'default');?></a><?php endif;?>
  </div>
  <div class="pagerinfo"><?php echo MiniFieldCommon::HtmlPager();?></div>
</div>
<script>
$(function(){
  $('.deletedatas').click(DeleteDatas);
});
function DeleteDatas(){
  if($('.deletedatas').hasClass('disabled')){ return; }
  if(!confirm('<?php echo ts('确定要删除勾选的数据吗？', 'ucfirst', 'default');?>')){
    return;
  }
  var ides = new Array();
  $('.ckb:checked').each(function(){
    ides.push($(this).val());
  });
  if(ides.length == 0){ alert('<?php echo ts('请至少勾选一项', 'ucfirst', 'default');?>');return; }
  $('.deletedatas').addClass('disabled');
  $('.deletedatas').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  var url = '<?php echo base_path().'cmsdata/'.$Category->id.'/delete';?>';
  $.post(url, {ides:ides}, function(ret){
    $('.deletedatas').removeClass('disabled');
    $('.deletedatas').removeAttr('title');
    if(ret.toString().indexOf('ok', 0) != -1){
      location.reload(1);
    }else{
      alert('<?php echo ts('删除', 'ucfirst', 'default');?> <?php echo ts('失败', 'strtolower', 'default');?>');
    }
  }, 'text').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
}
</script>