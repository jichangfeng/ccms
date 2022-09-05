<div id="crumbs">
  <a href="<?php echo base_path().'cmsnode/'.$Navigation->id.'/list';?>"><?php echo $Navigation->name;?></a> <em>›</em> <?php echo ts('内容列表', 'ucwords', 'node');?>[<?php echo $Navigation->categoryid[0][1];?>]
</div>
<div class="cdata">
  <div class="btnLink">
    <?php if(CMSNavigation::CanManageContent('add', $GLOBALS['user'], $Navigation)):?><a class="ico-button ico-add" href="<?php echo base_path().'cmsnode/'.$Navigation->id.'/add';?>"><?php echo ts('添加', 'ucwords', 'default');?></a><?php endif;?>
    <?php if(cmsMenuAccessCheck('cmsdata/'.CMSDATA_CID_NAVIGATION.'/GenerateTplarticleHtmlsAllByNavtag')):?><a class="ico-button GenerateTplarticleHtmlsAllByNavtag" href="javascript:;"><?php echo ts('生成单个栏目的所有内容模板HTML', 'ucwords', 'navigation');?></a><?php endif;?>
    <?php if(cmsMenuAccessCheck('cmsdata/'.CMSDATA_CID_NAVIGATION.'/ClearTplarticleHtmlsAllByNavtag')):?><a class="ico-button ClearTplarticleHtmlsAllByNavtag" href="javascript:;"><?php echo ts('清理单个栏目的所有内容模板HTML', 'ucwords', 'navigation');?></a><?php endif;?>
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
      <option value="id"<?php echo isset($_GET['sortfield'])&&$_GET['sortfield']=='id' ? ' selected="selected"' : '';?>>ID</option>
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
    <span class="left"><?php echo ts('内容列表', 'ucwords', 'node');?>（<?php echo $GLOBALS['pager_total_items'][0];?>）</span>
    <span class="right">
      <a<?php echo !isset($_GET['status']) ? ' style="font-weight: bold;"' : '';?> href="<?php echo base_path().'cmsnode/'.$Navigation->id.'/list';?>"><?php echo ts('全部', 'ucwords', 'node');?></a>
      <?php $arrValueArrays = MiniFieldData::GetValueArrays($Field_status);?>
      <?php foreach($arrValueArrays as $ValueArray):?>
      | <a<?php echo (isset($_GET['status']) && $_GET['status'] == $ValueArray[0])? ' style="font-weight: bold;"' : '';?> href="<?php echo base_path().'cmsnode/'.$Navigation->id.'/list?status='.$ValueArray[0];?>"><?php echo $ValueArray[1];?></a>
      <?php endforeach;?>
    </span>
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
    <?php if(isset($arrDatas[0])):?>
      <?php foreach($arrDatas as $key=>$Data):?>
      <tr class="<?php echo $key % 2 == 0 ? 'even' : 'odd';?>">
        <td><input class="ckb" type="checkbox" value="<?php echo $Data->id;?>"></td>
        <td><?php echo $Data->id;?></td>
        <?php if(isset($arrFields)):?>
          <?php foreach($arrFields as $Field):?>
            <?php if($Field->fieldname == 'userid'):?>
            <td><?php $User = user_load($Data->userid);echo isset($User->uid) ? '<span title="' . $User->uid . '">' . $User->name . '</span>' : '';?></td>
            <?php else:?>
            <td><?php echo MiniFieldData::GetDataOutput($Data, $Field, array('image_width' => '50px'));?></td>
            <?php endif;?>
          <?php endforeach;?>
          <td>
            <?php if(CMSNavigation::CanManageContent('edit', $GLOBALS['user'], $Navigation, $Data)):?><a class="ico-button ico-edit" href="<?php echo base_path().'cmsnode/'.$Navigation->id.'/'.$Data->id.'/edit'.(isset($_GET['page'])?'?page='.$_GET['page']:'');?>"><?php echo ts('编辑', 'ucwords', 'default');?></a><?php endif;?>
            <?php if(CMSNavigation::CanManageContent('delete', $GLOBALS['user'], $Navigation, $Data)):?><a class="ico-button ico-delete" onclick="return confirm('<?php echo ts('确定要删除此数据吗？', 'ucfirst', 'default');?>');" href="<?php echo base_path().'cmsnode/'.$Navigation->id.'/'.$Data->id.'/delete/confirm'.(isset($_GET['page'])?'?page='.$_GET['page']:'');?>"><?php echo ts('删除', 'ucwords', 'default');?></a><?php endif;?>
            <?php if(!empty($Navigation->tplarticle)):?>
              <?php $navtag = !empty($Navigation->urlalias) ? $Navigation->urlalias : $Navigation->id;?>
              <a class="ico-button ico-play" target="_blank" href="<?php echo base_path().'sts/'.$navtag.'/'.$Data->id;?>"><?php echo ts('预览', 'ucwords', 'default');?></a>
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
    <?php if(CMSNavigation::CanManageContent('delete', $GLOBALS['user'], $Navigation)):?><a class="ico-button ico-delete deletenodes" href="javascript:;"><?php echo ts('删除', 'ucwords', 'default');?></a><?php endif;?>
    <?php if(CMSNavigation::CanManageContent('edit', $GLOBALS['user'], $Navigation)):?>
      <select class="marknodes">
        <option value="-1"><?php echo ts('标记为', 'ucwords', 'node');?>...</option>
        <?php $arrValueArrays = MiniFieldData::GetValueArrays($Field_status);?>
        <?php foreach($arrValueArrays as $ValueArray):?>
        <option value="<?php echo $ValueArray[0];?>"><?php echo strip_tags($ValueArray[1]);?></option>
        <?php endforeach;?>
      </select>
    <?php endif;?>
    <?php if(CMSNavigation::CanManageContent('edit', $GLOBALS['user'], $Navigation)):?>
      <select class="movenodes" title="<?php echo ts('仅支持绿色字体栏目', 'ucfirst', 'node');?>">
        <option value="-1" title="<?php echo ts('仅支持绿色字体栏目', 'ucfirst', 'node');?>"><?php echo ts('移动到', 'ucwords', 'node');?>...</option>
        <?php $arrNavigationTrees = CMSNavigation::LoadNavigationTrees(CMSNavigation::LoadNavigations(0, 1, false));?>
        <?php foreach($arrNavigationTrees as $NavigationTree):?>
          <?php $color = $NavigationTree->categoryid == $Navigation->categoryid[0][0] ? 'green' : 'red';?>
          <?php $color = $NavigationTree->id == $Navigation->id ? 'blue' : $color;?>
          <option style="color: <?php echo $color;?>;" value="<?php echo $NavigationTree->id;?>"><?php echo $NavigationTree->level . $NavigationTree->name;?></option>
        <?php endforeach;?>
      </select>
    <?php endif;?>
    <?php if(cmsMenuAccessCheck('cmsdata/'.CMSDATA_CID_NAVIGATION.'/GenerateTplarticleHtmlsByNavtagNodeides')):?><a class="ico-button GenerateTplarticleHtmlsByNavtagNodeides" href="javascript:;"><?php echo ts('生成单个栏目的部分内容模板HTML', 'ucwords', 'navigation');?></a><?php endif;?>
    <?php if(cmsMenuAccessCheck('cmsdata/'.CMSDATA_CID_NAVIGATION.'/ClearTplarticleHtmlsByNavtagNodeides')):?><a class="ico-button ClearTplarticleHtmlsByNavtagNodeides" href="javascript:;"><?php echo ts('清理单个栏目的部分内容模板HTML', 'ucwords', 'navigation');?></a><?php endif;?>
  </div>
  <div class="pagerinfo"><?php echo MiniFieldCommon::HtmlPager();?></div>
</div>
<script>
$(function(){
  $('.deletenodes').click(deletenodes);
  $('.marknodes').change(marknodes);
  $('.movenodes').change(movenodes);
  $('.GenerateTplarticleHtmlsAllByNavtag').click(GenerateTplarticleHtmlsAllByNavtag);
  $('.ClearTplarticleHtmlsAllByNavtag').click(ClearTplarticleHtmlsAllByNavtag);
  $('.GenerateTplarticleHtmlsByNavtagNodeides').click(GenerateTplarticleHtmlsByNavtagNodeides);
  $('.ClearTplarticleHtmlsByNavtagNodeides').click(ClearTplarticleHtmlsByNavtagNodeides);
  <?php if(variable_get('GenerateTplarticleHtml')):?>
  $('.GenerateTplarticleHtmlsAllByNavtag').addClass('disabled');
  $('.GenerateTplarticleHtmlsAllByNavtag').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  $('.GenerateTplarticleHtmlsByNavtagNodeides').addClass('disabled');
  $('.GenerateTplarticleHtmlsByNavtagNodeides').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  <?php endif;?>
});
function deletenodes(){
  if($('.deletenodes').hasClass('disabled')){ return; }
  if(!confirm('<?php echo ts('确定要删除勾选的数据吗？', 'ucfirst', 'default');?>')){
    return;
  }
  var nodeides = new Array();
  $('.ckb:checked').each(function(){
    nodeides.push($(this).val());
  });
  if(nodeides.length == 0){ alert('<?php echo ts('请至少勾选一项', 'ucfirst', 'default');?>');return; }
  $('.deletenodes').addClass('disabled');
  $('.deletenodes').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  var url = '<?php echo base_path().'cmsnode/'.$Navigation->id.'/delete';?>';
  $.post(url, {nodeides:nodeides}, function(ret){
    $('.deletenodes').removeClass('disabled');
    $('.deletenodes').removeAttr('title');
    if(ret.toString().indexOf('ok', 0) != -1){
      location.reload(1);
    }else{
      alert('<?php echo ts('删除', 'ucfirst', 'default');?> <?php echo ts('失败', 'strtolower', 'default');?>');
    }
  }, 'text').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
}
function marknodes(){
  var status = $('.marknodes').val();
  if(status == -1){ return; }
  if(!confirm('<?php echo ts('确定要执行标记吗？', 'ucfirst', 'node');?>')){
    return;
  }
  var nodeides = new Array();
  $('.ckb:checked').each(function(){
    nodeides.push($(this).val());
  });
  if(nodeides.length == 0){ alert('<?php echo ts('请至少勾选一项', 'ucfirst', 'default');?>');$('.marknodes').find('option[value=-1]').attr('selected', 'selected');return; }
  var url = '<?php echo base_path().'cmsnode/'.$Navigation->id.'/mark';?>';
  $.post(url, {nodeides:nodeides, status:status}, function(ret){
    $('.marknodes').find('option[value=-1]').attr('selected', 'selected');
    if(ret.toString().indexOf('ok', 0) != -1){
      location.reload(1);
    }else{
      alert('<?php echo ts('标记为', 'ucfirst', 'node');?> <?php echo ts('失败', 'strtolower', 'default');?>');
    }
  }, 'text').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
}
function movenodes(){
  var tonavid = $('.movenodes').val();
  if(tonavid == -1){ return; }
  if(!confirm('<?php echo ts('确定要执行移动吗？', 'ucfirst', 'node');?>')){
    return;
  }
  var nodeides = new Array();
  $('.ckb:checked').each(function(){
    nodeides.push($(this).val());
  });
  if(nodeides.length == 0){ alert('<?php echo ts('请至少勾选一项', 'ucfirst', 'default');?>');$('.movenodes').find('option[value=-1]').attr('selected', 'selected');return; }
  var url = '<?php echo base_path().'cmsnode/'.$Navigation->id.'/move';?>';
  $.post(url, {nodeides:nodeides, tonavid:tonavid}, function(ret){
    $('.movenodes').find('option[value=-1]').attr('selected', 'selected');
    if(ret.toString().indexOf('ok', 0) != -1){
      location.reload(1);
    }else{
      alert('<?php echo ts('移动到', 'ucfirst', 'node');?> <?php echo ts('失败', 'strtolower', 'default');?>');
    }
  }, 'text').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
}
function GenerateTplarticleHtmlsAllByNavtag(){
  if($('.GenerateTplarticleHtmlsAllByNavtag').hasClass('disabled')){ return; }
  if(!confirm('<?php echo ts('确定要', 'ucfirst', 'default');?> <?php echo ts('生成单个栏目的所有内容模板HTML', '', 'navigation');?>？')){
    return;
  }
  $('.GenerateTplarticleHtmlsAllByNavtag').addClass('disabled');
  $('.GenerateTplarticleHtmlsAllByNavtag').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  $('.GenerateTplarticleHtmlsByNavtagNodeides').addClass('disabled');
  $('.GenerateTplarticleHtmlsByNavtagNodeides').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  var url = '<?php echo base_path().'cmsdata/'.CMSDATA_CID_NAVIGATION.'/GenerateTplarticleHtmlsAllByNavtag';?>';
  $.post(url, {navtag:'<?php echo $Navigation->id;?>'}, function(ret){
    $('.GenerateTplarticleHtmlsAllByNavtag').removeClass('disabled');
    $('.GenerateTplarticleHtmlsAllByNavtag').removeAttr('title');
    $('.GenerateTplarticleHtmlsByNavtagNodeides').removeClass('disabled');
    $('.GenerateTplarticleHtmlsByNavtagNodeides').removeAttr('title');
    if(ret.toString().indexOf('ok', 0) == -1){
      alert('<?php echo ts('生成单个栏目的所有内容模板HTML', 'ucfirst', 'navigation');?> <?php echo ts('失败', 'strtolower', 'default');?>');
    }
  }, 'text').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
}
function ClearTplarticleHtmlsAllByNavtag(){
  if($('.ClearTplarticleHtmlsAllByNavtag').hasClass('disabled')){ return; }
  if(!confirm('<?php echo ts('确定要', 'ucfirst', 'default');?> <?php echo ts('清理单个栏目的所有内容模板HTML', '', 'navigation');?>？')){
    return;
  }
  $('.ClearTplarticleHtmlsAllByNavtag').addClass('disabled');
  $('.ClearTplarticleHtmlsAllByNavtag').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  $('.ClearTplarticleHtmlsByNavtagNodeides').addClass('disabled');
  $('.ClearTplarticleHtmlsByNavtagNodeides').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  var url = '<?php echo base_path().'cmsdata/'.CMSDATA_CID_NAVIGATION.'/ClearTplarticleHtmlsAllByNavtag';?>';
  $.post(url, {navtag:'<?php echo $Navigation->id;?>'}, function(ret){
    $('.ClearTplarticleHtmlsAllByNavtag').removeClass('disabled');
    $('.ClearTplarticleHtmlsAllByNavtag').removeAttr('title');
    $('.ClearTplarticleHtmlsByNavtagNodeides').removeClass('disabled');
    $('.ClearTplarticleHtmlsByNavtagNodeides').removeAttr('title');
    if(ret.toString().indexOf('ok', 0) == -1){
      alert('<?php echo ts('清理单个栏目的所有内容模板HTML', 'ucfirst', 'navigation');?> <?php echo ts('失败', 'strtolower', 'default');?>');
    }
  }, 'text').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
}
function GenerateTplarticleHtmlsByNavtagNodeides(){
  if($('.GenerateTplarticleHtmlsByNavtagNodeides').hasClass('disabled')){ return; }
  if(!confirm('<?php echo ts('确定要', 'ucfirst', 'default');?> <?php echo ts('生成单个栏目的部分内容模板HTML', '', 'navigation');?>？')){
    return;
  }
  var nodeides = new Array();
  $('.ckb:checked').each(function(){
    nodeides.push($(this).val());
  });
  if(nodeides.length == 0){ alert('<?php echo ts('请至少勾选一项', 'ucfirst', 'default');?>');return; }
  $('.GenerateTplarticleHtmlsAllByNavtag').addClass('disabled');
  $('.GenerateTplarticleHtmlsAllByNavtag').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  $('.GenerateTplarticleHtmlsByNavtagNodeides').addClass('disabled');
  $('.GenerateTplarticleHtmlsByNavtagNodeides').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  var url = '<?php echo base_path().'cmsdata/'.CMSDATA_CID_NAVIGATION.'/GenerateTplarticleHtmlsByNavtagNodeides';?>';
  $.post(url, {navtag:'<?php echo $Navigation->id;?>', nodeides:nodeides}, function(ret){
    $('.GenerateTplarticleHtmlsAllByNavtag').removeClass('disabled');
    $('.GenerateTplarticleHtmlsAllByNavtag').removeAttr('title');
    $('.GenerateTplarticleHtmlsByNavtagNodeides').removeClass('disabled');
    $('.GenerateTplarticleHtmlsByNavtagNodeides').removeAttr('title');
    if(ret.toString().indexOf('ok', 0) == -1){
      alert('<?php echo ts('生成单个栏目的部分内容模板HTML', 'ucfirst', 'navigation');?> <?php echo ts('失败', 'strtolower', 'default');?>');
    }
  }, 'text').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
}
function ClearTplarticleHtmlsByNavtagNodeides(){
  if($('.ClearTplarticleHtmlsByNavtagNodeides').hasClass('disabled')){ return; }
  if(!confirm('<?php echo ts('确定要', 'ucfirst', 'default');?> <?php echo ts('清理单个栏目的部分内容模板HTML', '', 'navigation');?>？')){
    return;
  }
  var nodeides = new Array();
  $('.ckb:checked').each(function(){
    nodeides.push($(this).val());
  });
  if(nodeides.length == 0){ alert('<?php echo ts('请至少勾选一项', 'ucfirst', 'default');?>');return; }
  $('.ClearTplarticleHtmlsAllByNavtag').addClass('disabled');
  $('.ClearTplarticleHtmlsAllByNavtag').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  $('.ClearTplarticleHtmlsByNavtagNodeides').addClass('disabled');
  $('.ClearTplarticleHtmlsByNavtagNodeides').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  var url = '<?php echo base_path().'cmsdata/'.CMSDATA_CID_NAVIGATION.'/ClearTplarticleHtmlsByNavtagNodeides';?>';
  $.post(url, {navtag:'<?php echo $Navigation->id;?>', nodeides:nodeides}, function(ret){
    $('.ClearTplarticleHtmlsAllByNavtag').removeClass('disabled');
    $('.ClearTplarticleHtmlsAllByNavtag').removeAttr('title');
    $('.ClearTplarticleHtmlsByNavtagNodeides').removeClass('disabled');
    $('.ClearTplarticleHtmlsByNavtagNodeides').removeAttr('title');
    if(ret.toString().indexOf('ok', 0) == -1){
      alert('<?php echo ts('清理单个栏目的部分内容模板HTML', 'ucfirst', 'navigation');?> <?php echo ts('失败', 'strtolower', 'default');?>');
    }
  }, 'text').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
}
</script>