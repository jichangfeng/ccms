<div id="crumbs">
  <a href="<?php echo base_path().'cmsdata/'.$Category->id.'/list';?>"><?php echo $Category->name;?></a> <em>›</em> <?php echo ts('固定翻译', 'ucwords', 'translation');?>
</div>
<div class="cdata">
  <!--<div class="btnLink"></div>-->
  <!--<div class="searchArea"></div>-->
  <div class="tabTitle">
    <span class="left"><?php echo ts('固定翻译', 'ucwords', 'translation');?>(<?php echo ts('优先级较高', 'ucfirst', 'translation');?>)</span>
    <span class="right"></span>
  </div>
  <table>
    <thead>
      <tr>
        <th><?php echo ts('隶属于', 'ucwords', 'translation');?></th>
        <th><?php echo ts('简体中文', 'ucwords', 'translation');?></th>
        <th><?php echo ts('繁体中文', 'ucwords', 'translation');?></th>
        <th><?php echo ts('美国英文', 'ucwords', 'translation');?></th>
      </tr>
    </thead>
    <tbody>
      <?php foreach ($arrDatas as $key => $Datas):?>
        <?php foreach ($Datas as $Data):?>
      <tr><td><?php echo $key;?></td><td><?php echo $Data->zhs;?></td><td><?php echo $Data->zht;?></td><td><?php echo $Data->enus;?></td></tr>
        <?php endforeach;?>
      <?php endforeach;?>
    </tbody>
    <tfoot></tfoot>
  </table>
  <!--<div class="chooseData"></div>-->
  <!--<div class="pagerinfo"><?php //echo MiniFieldCommon::HtmlPager();?></div>-->
</div>