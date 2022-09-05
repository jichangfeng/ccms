<div id="crumbs">
  <a href="<?php echo base_path().'cmsuser/role/list';?>"><?php echo ts('网站角色', 'ucwords', 'default');?></a> <em>›</em> <?php echo ts('角色权限', 'ucwords', 'role');?>
</div>
<div class="cdata">
  <!--<div class="btnLink"></div>-->
  <!--<div class="searchArea"></div>-->
  <div class="tabTitle">
    <span class="left"><?php echo ts('角色权限', 'ucwords', 'role');?></span>
    <span class="right"></span>
  </div>
  <div id="permscontent"><?php echo $content;?></div>
  <!--<div class="chooseData"></div>-->
  <!--<div class="pagerinfo"><?php //echo MiniFieldCommon::HtmlPager();?></div>-->
</div>
<script>
$(function(){
  $('#permscontent').find('.compact-link').remove();
  <?php
    $module_info = system_get_info('module');
    $modules = array();
    foreach (module_implements('permission') as $module) {
      if(in_array($module, array('cms', 'cmsdata', 'cmsnode', 'cmssite', 'cmsuser', 'cmsfield', 'minifield'))){ continue; }
      $modules[$module] = $module_info[$module]['name'];
    }
    asort($modules);
    foreach ($modules as $module => $display_name) {
      $id =  'module-' . str_replace(' ', '_', $module);
      echo '$("#'.$id.'").parent("tr").hide();';
      if ($permissions = module_invoke($module, 'permission')) {
        foreach ($permissions as $perm => $perm_item) {
          $id = 'edit-' . strtolower(str_replace(' ', '-', $perm));
          echo '$("#'.$id.'").parent("td").parent("tr").hide();';
        }
      }
    }
  ?>
})
</script>
<style>
#user-admin-permissions .module{text-align:left;font-weight: bold; padding: 4px 9px;line-height: 19px;background-color:#FBF8E9;}
#user-admin-permissions .form-item{border-bottom:0 none;padding: 0;}
#user-admin-permissions th{line-height: 19px;}
</style>