<div id="crumbs">
  <a href="<?php echo base_path().'cmsdata/'.$Category->id.'/list';?>"><?php echo ts($Category->name, 'ucwords', 'default');?></a> <em>›</em> <?php echo ts('列表', 'ucwords', 'default');?>
</div>
<div class="cdata">
  <div class="btnLink">
    <?php if(cmsMenuAccessCheck('cmsdata/'.$Category->id.'/add')):?><a class="ico-button ico-add" href="<?php echo base_path().'cmsdata/'.$Category->id.'/add';?>"><?php echo ts('添加栏目', 'ucwords', 'navigation');?></a><?php endif;?>
    <?php if(cmsMenuAccessCheck('cmsdata/0/menurebuild')):?><a class="ico-button menurebuild" href="javascript:;"><?php echo ts('菜单重建', 'ucwords', 'navigation');?></a><?php endif;?>
    <?php if(cmsMenuAccessCheck('cmsdata/0/clearcache')):?><a class="ico-button clearcache" href="javascript:;"><?php echo ts('清理缓存', 'ucwords', 'navigation');?></a><?php endif;?>
    <?php if(cmsMenuAccessCheck('cmsdata/'.$Category->id.'/GenerateIndexHtml')):?><a class="ico-button GenerateIndexHtml" href="javascript:;"><?php echo ts('生成首页HTML', 'ucwords', 'navigation');?></a><?php endif;?>
    <?php if(cmsMenuAccessCheck('cmsdata/'.$Category->id.'/ClearIndexHtml')):?><a class="ico-button ClearIndexHtml" href="javascript:;"><?php echo ts('清理首页HTML', 'ucwords', 'navigation');?></a><?php endif;?>
    <br />
    <?php if(cmsMenuAccessCheck('cmsdata/'.$Category->id.'/GenerateTplindexHtmlsAll')):?><a class="ico-button GenerateTplindexHtmlsAll" href="javascript:;"><?php echo ts('生成所有栏目的栏目模板HTML', 'ucwords', 'navigation');?></a><?php endif;?>
    <?php if(cmsMenuAccessCheck('cmsdata/'.$Category->id.'/ClearTplindexHtmlsAll')):?><a class="ico-button ClearTplindexHtmlsAll" href="javascript:;"><?php echo ts('清理所有栏目的栏目模板HTML', 'ucwords', 'navigation');?></a><?php endif;?>
    <?php if(cmsMenuAccessCheck('cmsdata/'.$Category->id.'/GenerateTplarticleHtmlsAll')):?><a class="ico-button GenerateTplarticleHtmlsAll" href="javascript:;"><?php echo ts('生成所有栏目的所有内容模板HTML', 'ucwords', 'navigation');?></a><?php endif;?>
    <?php if(cmsMenuAccessCheck('cmsdata/'.$Category->id.'/ClearTplarticleHtmlsAll')):?><a class="ico-button ClearTplarticleHtmlsAll" href="javascript:;"><?php echo ts('清理所有栏目的所有内容模板HTML', 'ucwords', 'navigation');?></a><?php endif;?>
  </div>
  <!--<div class="searchArea"></div>-->
  <div class="tabTitle">
    <span class="left"><?php echo ts($Category->name, 'ucwords', 'default');?>（<?php echo ts('栏目或其子栏目下存在内容时，不能删除', 'ucfirst', 'navigation');?>）</span>
    <span class="right"></span>
  </div>
  <table class="table_tree">
    <thead>
      <tr>
          <th></th>
        <?php if(isset($arrFields)):?>
          <?php foreach($arrFields as $Field):?>
          <th><?php echo $Field->name;?></th>
          <?php endforeach;?>
          <th class="actions"><?php echo ts('操作', 'ucwords', 'default');?></th>
        <?php endif;?>
      </tr>
    </thead>
    <tbody>
    <?php
    function RecursionNavigation($Category, $arrFields, $arrDatas = null, $depth = 0, $collapse = 'node', $permissions = array()){
      $pading_left = $depth * 1.6 + 1;
      $trs = '';
      foreach($arrDatas as $Datakey => $Data){
        $trs_children = '';
        if(isset($Data->children)){ $trs_children .= RecursionNavigation($Category, $arrFields, $Data->children, $depth + 1, $collapse . '_' . ($Datakey + 1), $permissions); }
        $classes = array($collapse);
        if(!empty($trs_children)){ array_push($classes, 'fold');array_push($classes, 'collapsed'); }
        if($depth > 0){ array_push($classes, "tr_hidden"); }
        $trs .= '<tr class="' . implode(' ', $classes) . '" nid="' . $collapse . '_' . ($Datakey + 1).'">';
        $trs .= '<td><input class="ckb" type="checkbox" value="' . $Data->id . '"></td>';
        foreach($arrFields as $Fieldkey => $Field){
          $trs .= '<td style="' . ($Fieldkey == 1 ? "width:20%;text-align: left;padding-left:{$pading_left}em" : "") . '">' . ($Fieldkey == 1 ? '<span class="span_ico">&nbsp;</span>' : '');
          $trs .= MiniFieldData::GetDataOutput($Data, $Field, array('image_width' => '50px'));
          $trs .= '</td>';
        }
        $trs .= '<td style="width: 150px;">';
        if($permissions['canEditData']){ $trs .= '<a class="ico-button ico-edit" href="'.base_path().'cmsdata/'.$Category->id.'/'.$Data->id.'/edit'.'">'.ts('编辑', 'ucwords', 'default').'</a>'; }
        if($permissions['canDeleteData']){
          if(false){//if(!CMSNavigation::CanDeleted($Data->id)){
            $trs .= ' <a class="ico-button ico-delete" onclick="alert(\''.ts('当前栏目或子栏目下存在内容，不能删除', 'ucwords', 'role').'\');return false;" href="#">'.ts('删除', 'ucwords', 'default').'</a>';
          }else{
            $trs .= ' <a class="ico-button ico-delete" onclick="return confirm(\''.ts('确定要删除此栏目吗？', 'ucfirst', 'navigation').'\n'.ts('栏目或其子栏目下存在内容时，不能删除', 'ucfirst', 'navigation').'\');" href="'.base_path().'cmsdata/'.$Category->id.'/'.$Data->id.'/delete/confirm'.'">'.ts('删除', 'ucwords', 'default').'</a>';
          }
        }
        $trs .= '</td>';
        $trs .= '</tr>';
        $trs .= $trs_children;
      }
      return $trs;
    }
    $permissions = array(
      'canEditData' => cmsMenuAccessCheck('cmsdata/'.$Category->id.'/%/edit'),
      'canDeleteData' => cmsMenuAccessCheck('cmsdata/'.$Category->id.'/%/delete')
    );
    echo RecursionNavigation($Category, $arrFields, $arrDatas, 0, 'node', $permissions);
    ?>
    </tbody>
    <tfoot></tfoot>
  </table>
  <div class="chooseData">
    <a class="ico-button ico-spark" href="javascript:;" onclick="$('.ckb').attr('checked', 'checked');"><?php echo ts('全选', 'ucwords', 'default');?></a>
    <a class="ico-button ico-spark" href="javascript:;" onclick="$('.ckb').removeAttr('checked');"><?php echo ts('全反选', 'ucwords', 'default');?></a>
    <?php if(cmsMenuAccessCheck('cmsdata/'.$Category->id.'/delete')):?><a class="ico-button ico-delete deletenavs" href="javascript:;"><?php echo ts('删除', 'ucwords', 'default');?></a><?php endif;?>
    <?php if(cmsMenuAccessCheck('cmsdata/'.$Category->id.'/GenerateTplindexHtmlsByNavtags')):?><a class="ico-button GenerateTplindexHtmlsByNavtags" href="javascript:;"><?php echo ts('生成部分栏目的栏目模板HTML', 'ucwords', 'navigation');?></a><?php endif;?>
    <?php if(cmsMenuAccessCheck('cmsdata/'.$Category->id.'/ClearTplindexHtmlsByNavtags')):?><a class="ico-button ClearTplindexHtmlsByNavtags" href="javascript:;"><?php echo ts('清理部分栏目的栏目模板HTML', 'ucwords', 'navigation');?></a><?php endif;?>
    <?php if(cmsMenuAccessCheck('cmsdata/'.$Category->id.'/GenerateTplarticleHtmlsAllByNavtags')):?><a class="ico-button GenerateTplarticleHtmlsAllByNavtags" href="javascript:;"><?php echo ts('生成部分栏目的所有内容模板HTML', 'ucwords', 'navigation');?></a><?php endif;?>
    <?php if(cmsMenuAccessCheck('cmsdata/'.$Category->id.'/ClearTplarticleHtmlsAllByNavtags')):?><a class="ico-button ClearTplarticleHtmlsAllByNavtags" href="javascript:;"><?php echo ts('清理部分栏目的所有内容模板HTML', 'ucwords', 'navigation');?></a><?php endif;?>
  </div>
  <!--<div class="pagerinfo"><?php //echo MiniFieldCommon::HtmlPager();?></div>-->
</div>
<script>
$(function(){
  $('.menurebuild').click(menurebuild);
  $('.clearcache').click(clearcache);
  $('.deletenavs').click(deletenavs);
  $('.GenerateIndexHtml').click(GenerateIndexHtml);
  $('.ClearIndexHtml').click(ClearIndexHtml);
  $('.GenerateTplindexHtmlsAll').click(GenerateTplindexHtmlsAll);
  $('.ClearTplindexHtmlsAll').click(ClearTplindexHtmlsAll);
  $('.GenerateTplarticleHtmlsAll').click(GenerateTplarticleHtmlsAll);
  $('.ClearTplarticleHtmlsAll').click(ClearTplarticleHtmlsAll);
  $('.GenerateTplindexHtmlsByNavtags').click(GenerateTplindexHtmlsByNavtags);
  $('.ClearTplindexHtmlsByNavtags').click(ClearTplindexHtmlsByNavtags);
  $('.GenerateTplarticleHtmlsAllByNavtags').click(GenerateTplarticleHtmlsAllByNavtags);
  $('.ClearTplarticleHtmlsAllByNavtags').click(ClearTplarticleHtmlsAllByNavtags);
  <?php if(variable_get('GenerateIndexHtml')):?>
  $('.GenerateIndexHtml').addClass('disabled');
  $('.GenerateIndexHtml').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  <?php endif;?>
  <?php if(variable_get('GenerateTplindexHtml')):?>
  $('.GenerateTplindexHtmlsAll').addClass('disabled');
  $('.GenerateTplindexHtmlsAll').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  $('.GenerateTplindexHtmlsByNavtags').addClass('disabled');
  $('.GenerateTplindexHtmlsByNavtags').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  <?php endif;?>
  <?php if(variable_get('GenerateTplarticleHtml')):?>
  $('.GenerateTplarticleHtmlsAll').addClass('disabled');
  $('.GenerateTplarticleHtmlsAll').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  $('.GenerateTplarticleHtmlsAllByNavtags').addClass('disabled');
  $('.GenerateTplarticleHtmlsAllByNavtags').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  <?php endif;?>
});
function menurebuild(){
  if($('.menurebuild').hasClass('disabled')){ return; }
  if(!confirm('<?php echo ts('确定要执行菜单重建操作吗？', 'ucfirst', 'navigation');?>')){
    return;
  }
  $('.menurebuild').addClass('disabled');
  $('.menurebuild').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  var url = '<?php echo base_path().'cmsdata/0/menurebuild';?>';
  $.post(url, {}, function(ret){
    $('.menurebuild').removeClass('disabled');
    $('.menurebuild').removeAttr('title');
    if(ret.toString().indexOf('ok', 0) == -1){
      alert('<?php echo ts('菜单重建', 'ucfirst', 'navigation');?> <?php echo ts('失败', 'strtolower', 'default');?>');
    }
  }, 'text').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
}
function clearcache(){
  if($('.clearcache').hasClass('disabled')){ return; }
  if(!confirm('<?php echo ts('确定要执行清理缓存操作吗？', 'ucfirst', 'navigation');?>')){
    return;
  }
  $('.clearcache').addClass('disabled');
  $('.clearcache').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  var url = '<?php echo base_path().'cmsdata/0/clearcache';?>';
  $.post(url, {}, function(ret){
    $('.clearcache').removeClass('disabled');
    $('.clearcache').removeAttr('title');
    if(ret.toString().indexOf('ok', 0) == -1){
      alert('<?php echo ts('清理缓存', 'ucfirst', 'navigation');?> <?php echo ts('失败', 'strtolower', 'default');?>');
    }
  }, 'text').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
}
function deletenavs(){
  if($('.deletenavs').hasClass('disabled')){ return; }
  if(!confirm('<?php echo ts('确定要删除勾选的栏目吗？', 'ucfirst', 'navigation');?>')){
    return;
  }
  var navtags = new Array();
  $('.ckb:checked').each(function(){
    navtags.push($(this).val());
  });
  if(navtags.length == 0){ alert('<?php echo ts('请至少勾选一项', 'ucfirst', 'default');?>');return; }
  $('.deletenavs').addClass('disabled');
  $('.deletenavs').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  var url = '<?php echo base_path().'cmsdata/'.$Category->id.'/delete';?>';
  $.post(url, {navtags:navtags}, function(ret){
    $('.deletenavs').removeClass('disabled');
    $('.deletenavs').removeAttr('title');
    if(ret.toString().indexOf('ok', 0) != -1){
      location.reload(1);
    }else{
      alert('<?php echo ts('删除', 'ucfirst', 'default');?> <?php echo ts('失败', 'strtolower', 'default');?>');
    }
  }, 'text').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
}
function GenerateIndexHtml(){
  if($('.GenerateIndexHtml').hasClass('disabled')){ return; }
  if(!confirm('<?php echo ts('确定要', 'ucfirst', 'default');?> <?php echo ts('生成首页HTML', '', 'navigation');?>？')){
    return;
  }
  $('.GenerateIndexHtml').addClass('disabled');
  $('.GenerateIndexHtml').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  var url = '<?php echo base_path().'cmsdata/'.$Category->id.'/GenerateIndexHtml';?>';
  $.post(url, {}, function(ret){
    $('.GenerateIndexHtml').removeClass('disabled');
    $('.GenerateIndexHtml').removeAttr('title');
    if(ret.toString().indexOf('ok', 0) == -1){
      alert('<?php echo ts('生成首页HTML', 'ucfirst', 'navigation');?> <?php echo ts('失败', 'strtolower', 'default');?>');
    }
  }, 'text').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
}
function ClearIndexHtml(){
  if($('.ClearIndexHtml').hasClass('disabled')){ return; }
  if(!confirm('<?php echo ts('确定要', 'ucfirst', 'default');?> <?php echo ts('清理首页HTML', '', 'navigation');?>？')){
    return;
  }
  $('.ClearIndexHtml').addClass('disabled');
  $('.ClearIndexHtml').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  var url = '<?php echo base_path().'cmsdata/'.$Category->id.'/ClearIndexHtml';?>';
  $.post(url, {}, function(ret){
    $('.ClearIndexHtml').removeClass('disabled');
    $('.ClearIndexHtml').removeAttr('title');
    if(ret.toString().indexOf('ok', 0) == -1){
      alert('<?php echo ts('清理首页HTML', 'ucfirst', 'navigation');?> <?php echo ts('失败', 'strtolower', 'default');?>');
    }
  }, 'text').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
}
function GenerateTplindexHtmlsAll(){
  if($('.GenerateTplindexHtmlsAll').hasClass('disabled')){ return; }
  if(!confirm('<?php echo ts('确定要', 'ucfirst', 'default');?> <?php echo ts('生成所有栏目的栏目模板HTML', '', 'navigation');?>？')){
    return;
  }
  $('.GenerateTplindexHtmlsAll').addClass('disabled');
  $('.GenerateTplindexHtmlsAll').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  $('.GenerateTplindexHtmlsByNavtags').addClass('disabled');
  $('.GenerateTplindexHtmlsByNavtags').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  var url = '<?php echo base_path().'cmsdata/'.$Category->id.'/GenerateTplindexHtmlsAll';?>';
  $.post(url, {}, function(ret){
    $('.GenerateTplindexHtmlsAll').removeClass('disabled');
    $('.GenerateTplindexHtmlsAll').removeAttr('title');
    $('.GenerateTplindexHtmlsByNavtags').removeClass('disabled');
    $('.GenerateTplindexHtmlsByNavtags').removeAttr('title');
    if(ret.toString().indexOf('ok', 0) == -1){
      alert('<?php echo ts('生成所有栏目的栏目模板HTML', 'ucfirst', 'navigation');?> <?php echo ts('失败', 'strtolower', 'default');?>');
    }
  }, 'text').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
}
function ClearTplindexHtmlsAll(){
  if($('.ClearTplindexHtmlsAll').hasClass('disabled')){ return; }
  if(!confirm('<?php echo ts('确定要', 'ucfirst', 'default');?> <?php echo ts('清理所有栏目的栏目模板HTML', '', 'navigation');?>？')){
    return;
  }
  $('.ClearTplindexHtmlsAll').addClass('disabled');
  $('.ClearTplindexHtmlsAll').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  $('.ClearTplindexHtmlsByNavtags').addClass('disabled');
  $('.ClearTplindexHtmlsByNavtags').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  var url = '<?php echo base_path().'cmsdata/'.$Category->id.'/ClearTplindexHtmlsAll';?>';
  $.post(url, {}, function(ret){
    $('.ClearTplindexHtmlsAll').removeClass('disabled');
    $('.ClearTplindexHtmlsAll').removeAttr('title');
    $('.ClearTplindexHtmlsByNavtags').removeClass('disabled');
    $('.ClearTplindexHtmlsByNavtags').removeAttr('title');
    if(ret.toString().indexOf('ok', 0) == -1){
      alert('<?php echo ts('清理所有栏目的栏目模板HTML', 'ucfirst', 'navigation');?> <?php echo ts('失败', 'strtolower', 'default');?>');
    }
  }, 'text').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
}
function GenerateTplarticleHtmlsAll(){
  if($('.GenerateTplarticleHtmlsAll').hasClass('disabled')){ return; }
  if(!confirm('<?php echo ts('确定要', 'ucfirst', 'default');?> <?php echo ts('生成所有栏目的所有内容模板HTML', '', 'navigation');?>？')){
    return;
  }
  $('.GenerateTplarticleHtmlsAll').addClass('disabled');
  $('.GenerateTplarticleHtmlsAll').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  $('.GenerateTplarticleHtmlsAllByNavtags').addClass('disabled');
  $('.GenerateTplarticleHtmlsAllByNavtags').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  var url = '<?php echo base_path().'cmsdata/'.$Category->id.'/GenerateTplarticleHtmlsAll';?>';
  $.post(url, {}, function(ret){
    $('.GenerateTplarticleHtmlsAll').removeClass('disabled');
    $('.GenerateTplarticleHtmlsAll').removeAttr('title');
    $('.GenerateTplarticleHtmlsAllByNavtags').removeClass('disabled');
    $('.GenerateTplarticleHtmlsAllByNavtags').removeAttr('title');
    if(ret.toString().indexOf('ok', 0) == -1){
      alert('<?php echo ts('生成所有栏目的所有内容模板HTML', 'ucfirst', 'navigation');?> <?php echo ts('失败', 'strtolower', 'default');?>');
    }
  }, 'text').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
}
function ClearTplarticleHtmlsAll(){
  if($('.ClearTplarticleHtmlsAll').hasClass('disabled')){ return; }
  if(!confirm('<?php echo ts('确定要', 'ucfirst', 'default');?> <?php echo ts('清理所有栏目的所有内容模板HTML', '', 'navigation');?>？')){
    return;
  }
  $('.ClearTplarticleHtmlsAll').addClass('disabled');
  $('.ClearTplarticleHtmlsAll').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  $('.ClearTplarticleHtmlsAllByNavtags').addClass('disabled');
  $('.ClearTplarticleHtmlsAllByNavtags').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  var url = '<?php echo base_path().'cmsdata/'.$Category->id.'/ClearTplarticleHtmlsAll';?>';
  $.post(url, {}, function(ret){
    $('.ClearTplarticleHtmlsAll').removeClass('disabled');
    $('.ClearTplarticleHtmlsAll').removeAttr('title');
    $('.ClearTplarticleHtmlsAllByNavtags').removeClass('disabled');
    $('.ClearTplarticleHtmlsAllByNavtags').removeAttr('title');
    if(ret.toString().indexOf('ok', 0) == -1){
      alert('<?php echo ts('清理所有栏目的所有内容模板HTML', 'ucfirst', 'navigation');?> <?php echo ts('失败', 'strtolower', 'default');?>');
    }
  }, 'text').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
}
function GenerateTplindexHtmlsByNavtags(){
  if($('.GenerateTplindexHtmlsByNavtags').hasClass('disabled')){ return; }
  if(!confirm('<?php echo ts('确定要', 'ucfirst', 'default');?> <?php echo ts('生成部分栏目的栏目模板HTML', '', 'navigation');?>？')){
    return;
  }
  var navtags = new Array();
  $('.ckb:checked').each(function(){
    navtags.push($(this).val());
  });
  if(navtags.length == 0){ alert('<?php echo ts('请至少勾选一项', 'ucfirst', 'default');?>');return; }
  $('.GenerateTplindexHtmlsAll').addClass('disabled');
  $('.GenerateTplindexHtmlsAll').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  $('.GenerateTplindexHtmlsByNavtags').addClass('disabled');
  $('.GenerateTplindexHtmlsByNavtags').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  var url = '<?php echo base_path().'cmsdata/'.$Category->id.'/GenerateTplindexHtmlsByNavtags';?>';
  $.post(url, {navtags:navtags}, function(ret){
    $('.GenerateTplindexHtmlsAll').removeClass('disabled');
    $('.GenerateTplindexHtmlsAll').removeAttr('title');
    $('.GenerateTplindexHtmlsByNavtags').removeClass('disabled');
    $('.GenerateTplindexHtmlsByNavtags').removeAttr('title');
    if(ret.toString().indexOf('ok', 0) == -1){
      alert('<?php echo ts('生成部分栏目的栏目模板HTML', 'ucfirst', 'navigation');?> <?php echo ts('失败', 'strtolower', 'default');?>');
    }
  }, 'text').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
}
function ClearTplindexHtmlsByNavtags(){
  if($('.ClearTplindexHtmlsByNavtags').hasClass('disabled')){ return; }
  if(!confirm('<?php echo ts('确定要', 'ucfirst', 'default');?> <?php echo ts('清理部分栏目的栏目模板HTML', '', 'navigation');?>？')){
    return;
  }
  var navtags = new Array();
  $('.ckb:checked').each(function(){
    navtags.push($(this).val());
  });
  if(navtags.length == 0){ alert('<?php echo ts('请至少勾选一项', 'ucfirst', 'default');?>');return; }
  $('.ClearTplindexHtmlsAll').addClass('disabled');
  $('.ClearTplindexHtmlsAll').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  $('.ClearTplindexHtmlsByNavtags').addClass('disabled');
  $('.ClearTplindexHtmlsByNavtags').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  var url = '<?php echo base_path().'cmsdata/'.$Category->id.'/ClearTplindexHtmlsByNavtags';?>';
  $.post(url, {navtags:navtags}, function(ret){
    $('.ClearTplindexHtmlsAll').removeClass('disabled');
    $('.ClearTplindexHtmlsAll').removeAttr('title');
    $('.ClearTplindexHtmlsByNavtags').removeClass('disabled');
    $('.ClearTplindexHtmlsByNavtags').removeAttr('title');
    if(ret.toString().indexOf('ok', 0) == -1){
      alert('<?php echo ts('清理部分栏目的栏目模板HTML', 'ucfirst', 'navigation');?> <?php echo ts('失败', 'strtolower', 'default');?>');
    }
  }, 'text').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
}
function GenerateTplarticleHtmlsAllByNavtags(){
  if($('.GenerateTplarticleHtmlsAllByNavtags').hasClass('disabled')){ return; }
  if(!confirm('<?php echo ts('确定要', 'ucfirst', 'default');?> <?php echo ts('生成部分栏目的所有内容模板HTML', '', 'navigation');?>？')){
    return;
  }
  var navtags = new Array();
  $('.ckb:checked').each(function(){
    navtags.push($(this).val());
  });
  if(navtags.length == 0){ alert('<?php echo ts('请至少勾选一项', 'ucfirst', 'default');?>');return; }
  $('.GenerateTplarticleHtmlsAll').addClass('disabled');
  $('.GenerateTplarticleHtmlsAll').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  $('.GenerateTplarticleHtmlsAllByNavtags').addClass('disabled');
  $('.GenerateTplarticleHtmlsAllByNavtags').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  var url = '<?php echo base_path().'cmsdata/'.$Category->id.'/GenerateTplarticleHtmlsAllByNavtags';?>';
  $.post(url, {navtags:navtags}, function(ret){
    $('.GenerateTplarticleHtmlsAll').removeClass('disabled');
    $('.GenerateTplarticleHtmlsAll').removeAttr('title');
    $('.GenerateTplarticleHtmlsAllByNavtags').removeClass('disabled');
    $('.GenerateTplarticleHtmlsAllByNavtags').removeAttr('title');
    if(ret.toString().indexOf('ok', 0) == -1){
      alert('<?php echo ts('生成部分栏目的所有内容模板HTML', 'ucfirst', 'navigation');?> <?php echo ts('失败', 'strtolower', 'default');?>');
    }
  }, 'text').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
}
function ClearTplarticleHtmlsAllByNavtags(){
  if($('.ClearTplarticleHtmlsAllByNavtags').hasClass('disabled')){ return; }
  if(!confirm('<?php echo ts('确定要', 'ucfirst', 'default');?> <?php echo ts('清理部分栏目的所有内容模板HTML', '', 'navigation');?>？')){
    return;
  }
  var navtags = new Array();
  $('.ckb:checked').each(function(){
    navtags.push($(this).val());
  });
  if(navtags.length == 0){ alert('<?php echo ts('请至少勾选一项', 'ucfirst', 'default');?>');return; }
  $('.ClearTplarticleHtmlsAll').addClass('disabled');
  $('.ClearTplarticleHtmlsAll').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  $('.ClearTplarticleHtmlsAllByNavtags').addClass('disabled');
  $('.ClearTplarticleHtmlsAllByNavtags').attr('title', '<?php echo ts('执行中', 'ucwords', 'default');?>');
  var url = '<?php echo base_path().'cmsdata/'.$Category->id.'/ClearTplarticleHtmlsAllByNavtags';?>';
  $.post(url, {navtags:navtags}, function(ret){
    $('.ClearTplarticleHtmlsAll').removeClass('disabled');
    $('.ClearTplarticleHtmlsAll').removeAttr('title');
    $('.ClearTplarticleHtmlsAllByNavtags').removeClass('disabled');
    $('.ClearTplarticleHtmlsAllByNavtags').removeAttr('title');
    if(ret.toString().indexOf('ok', 0) == -1){
      alert('<?php echo ts('清理部分栏目的所有内容模板HTML', 'ucfirst', 'navigation');?> <?php echo ts('失败', 'strtolower', 'default');?>');
    }
  }, 'text').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
}
</script>