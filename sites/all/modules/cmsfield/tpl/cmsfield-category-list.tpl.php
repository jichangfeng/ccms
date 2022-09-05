<div id="crumbs">
  <a href="<?php echo base_path().'cmsfield/category/list';?>"><?php echo ts($Category->name, 'ucwords', 'cmsfield');?></a> <em>›</em> <?php echo ts('列表', 'ucwords', 'default');?>
</div>
<div class="cdata">
  <div class="btnLink">
    <?php if(cmsMenuAccessCheck('cmsfield/category/add')):?><a class="ico-button ico-add" href="<?php echo base_path().'cmsfield/category/add';?>"><?php echo ts('添加', 'ucwords', 'default');?></a><?php endif;?>
  </div>
  <!--<div class="searchArea"></div>-->
  <div class="tabTitle">
    <span class="left"><?php echo $Category->name;?>（<?php echo $GLOBALS['pager_total_items'][0];?>）</span>
    <span class="right"></span>
  </div>
  <table>
    <thead>
      <tr>
        <th>ID</th>
        <?php if(isset($arrFields)):?>
          <?php foreach($arrFields as $Field):?>
          <th><?php echo $Field->name;?></th>
          <?php endforeach;?>
          <th>操作</th>
        <?php endif;?>
      </tr>
    </thead>
    <tbody>
    <?php if(isset($arrDatas)):?>
      <?php
        $canEditData = cmsMenuAccessCheck('cmsfield/category/%/edit');
        $canDeleteData = cmsMenuAccessCheck('cmsfield/category/%/delete');
      ?>
      <?php foreach($arrDatas as $key=>$Data):?>
      <tr class="<?php echo $key % 2 == 0 ? 'even' : 'odd';?>">
        <td><?php echo $Data->id;?></td>
        <?php if(isset($arrFields)):?>
          <?php foreach($arrFields as $Field):?>
            <?php if($Field->fieldname == 'tablename'):?>
            <td><?php echo '<span style="color: ' . (db_table_exists($Data->tablename) ? 'green' : 'red') . ';">' . $Data->tablename . '</span>';?></td>
            <?php else:?>
            <td><?php echo MiniFieldData::GetDataOutput($Data, $Field, array('image_width' => '50px'));?></td>
            <?php endif;?>
          <?php endforeach;?>
          <td>
            <?php if($canEditData):?><a class="ico-button ico-edit" href="<?php echo base_path().'cmsfield/category/'.$Data->id.'/edit';?>"><?php echo ts('编辑', 'ucwords', 'default');?></a><?php endif;?>
            <?php if($canDeleteData):?><a class="ico-button ico-delete" onclick="return confirm('确定要删除此数据吗？');" href="<?php echo base_path().'cmsfield/category/'.$Data->id.'/delete/confirm';?>"><?php echo ts('删除', 'ucwords', 'default');?></a><?php endif;?>
          </td>
        <?php endif;?>
      </tr>
      <?php endforeach;?>
    <?php endif;?>
    </tbody>
    <tfoot></tfoot>
  </table>
  <!--<div class="chooseData"></div>-->
  <div class="pagerinfo"><?php echo MiniFieldCommon::HtmlPager();?></div>
</div>