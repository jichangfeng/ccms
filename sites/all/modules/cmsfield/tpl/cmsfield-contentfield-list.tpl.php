<div id="crumbs">
  <a href="<?php echo base_path().'cmsfield/contentfield/'.$Category->id.'/list';?>"><?php echo $Category->name;?></a> <em>›</em> <?php echo ts('列表', 'ucwords', 'default');?>
</div>
<div class="cdata">
  <!--<div class="btnLink"></div>-->
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
        $canEditData = cmsMenuAccessCheck('cmsfield/contentfield/'.$Category->id.'/%/edit');
      ?>
      <?php foreach($arrDatas as $key=>$Data):?>
      <tr class="<?php echo $key % 2 == 0 ? 'even' : 'odd';?>">
        <td><?php echo $Data->id;?></td>
        <?php if(isset($arrFields)):?>
          <?php foreach($arrFields as $Field):?>
            <?php if($Field->fieldname == 'fieldname'):?>
            <td><?php echo '<span style="color: ' . (db_field_exists($Data->tablename, $Data->fieldname) ? 'green' : 'red') . ';">' . $Data->fieldname . '</span>';?></td>
            <?php else:?>
            <td><?php echo MiniFieldData::GetDataOutput($Data, $Field, array('image_width' => '50px'));?></td>
            <?php endif;?>
          <?php endforeach;?>
          <td>
            <?php if($canEditData && !in_array($Data->fieldname, array('navid', 'userid', 'created', 'status'))):?>
            <a class="ico-button ico-edit" href="<?php echo base_path().'cmsfield/contentfield/'.$Category->id.'/'.$Data->id.'/edit';?>"><?php echo ts('编辑', 'ucwords', 'default');?></a>
            <?php endif;?>
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