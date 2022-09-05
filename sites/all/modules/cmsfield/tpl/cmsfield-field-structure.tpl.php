<div id="crumbs">
  <a href="<?php echo base_path().'cmsfield/field';?>"><?php echo ts('网站字段', 'ucwords', 'default');?></a> <em>›</em> <?php echo ts('字段结构树', 'ucwords', 'cmsfield');?>
</div>
<div class="cdata">
  <!--<div class="btnLink"></div>-->
  <!--<div class="searchArea"></div>-->
  <div class="tabTitle">
    <span class="left"><?php echo ts('字段结构树', 'ucwords', 'cmsfield');?></span>
    <span class="right"></span>
  </div>
  <div>
    <?php foreach($CategoryGroups as $CategoryGroup):?>
    <div class="grouptags">
      <div class="grouptag"><?php echo $CategoryGroup['ValueArray'][1];?></div>
      <div class="grouplist">
        <?php $arrCategories = $CategoryGroup['arrCategories'];?>
        <?php foreach($arrCategories as $Category):?>
        <a href="<?php echo base_path().'cmsfield/field/'.$Category->id.'/list';?>"><?php echo $Category->name;?></a>
        <?php endforeach;?>
      </div>
    </div>
    <?php endforeach;?>
  </div>
  <!--<div class="chooseData"></div>-->
  <!--<div class="pagerinfo"><?php //echo MiniFieldCommon::HtmlPager();?></div>-->
</div>
<style>
.grouptags{border-top: solid 1px #D3D3D3;}
.grouptags .grouptag{background-color: #FBF8E9;font-weight: bold;line-height: 19px;padding: 4px 9px;text-align: left;}
.grouptags .grouplist{background-color: #FFFFFF;line-height: 19px;padding: 4px 9px;text-align: left;}
</style>