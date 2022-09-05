<div id="crumbs">
  <a href="<?php echo base_path().'cmsdata/'.$Category->id.'/list';?>"><?php echo ts($Category->name, 'ucwords', 'default');?></a> <em>›</em> <?php echo ts('编辑', 'ucwords', 'default');?>
</div>
<?php $arrHtmlCssJs = array();?>
<form style="width: 100%;margin: 0 auto;" method="post" action="<?php echo $action;?>" enctype="multipart/form-data">
  <div class="formArea">
    <div class="formTit"><?php echo ts($Category->name, 'ucwords', 'default') . ' ' . ts('编辑', 'ucwords', 'default');?></div>
    <div class="formEle">
      <input type="hidden" value="<?php echo $Data->id;?>" name="id"/>
      <?php foreach($arrFields as $Field):?>
        <?php $fieldTag = $Field->fieldname;?>
        <?php $arrHtmlCssJs = array_merge($arrHtmlCssJs, MiniFieldRepository::HtmlCssJsOfEditor($Field->editor));?>
        <?php include CMSDATA_MODULE_PATH.'/tpl/extends/data-editor-basic-single.tpl.php';?>
        <?php include CMSDATA_MODULE_PATH.'/tpl/extends/data-editor-basic-multiple.tpl.php';?>
        <?php include CMSDATA_MODULE_PATH.'/tpl/extends/data-editor-extend-single.tpl.php';?>
        <?php include CMSDATA_MODULE_PATH.'/tpl/extends/data-editor-extend-multiple.tpl.php';?>
      <?php endforeach;?>
      <div class="form-actions">
        <input type="submit" class="form-submit" value="<?php echo ts('保存', 'ucwords', 'default');?>">&nbsp;
        <input type="reset" class="form-submit" value="<?php echo ts('重置', 'ucwords', 'default');?>">
      </div>
    </div>
  </div>
</form>
<?php if(isset($arrHtmlCssJs['js_jquery'])){unset($arrHtmlCssJs['js_jquery']);};echo implode('', $arrHtmlCssJs);?>