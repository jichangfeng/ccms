<div id="crumbs">
  <a href="<?php echo base_path().'cmsuser/user';?>"><?php echo ts('网站用户', 'ucwords', 'default');?></a> <em>›</em> <?php echo ts('用户结构树', 'ucwords', 'user');?>
</div>
<div class="cdata">
  <!--<div class="btnLink"></div>-->
  <!--<div class="searchArea"></div>-->
  <div class="tabTitle">
    <span class="left"><?php echo ts('用户结构树', 'ucwords', 'user');?></span>
    <span class="right">当前在线：<?php echo CMSUser::LoadUserCountOnlineByRides();?></span>
  </div>
  <div class="rolelist">
    <?php foreach($arrRoles as $Role):?>
      <?php if(CMSRole::CanManageUser('view', $GLOBALS['user'], $Role) == 0){ continue; };?>
      <a href="<?php echo base_path().'cmsuser/user/'.$Role->rid.'/list';?>"><?php echo $Role->name;?>(<?php echo CMSUser::LoadUserCountByRides(array($Role->rid));?>)</a>
    <?php endforeach;?>
  </div>
  <!--<div class="chooseData"></div>-->
  <!--<div class="pagerinfo"><?php //echo MiniFieldCommon::HtmlPager();?></div>-->
</div>
<style>
.rolelist{border-top: solid 1px #D3D3D3;background-color: #FFFFFF;line-height: 19px;padding: 4px 9px;text-align: left;}
</style>