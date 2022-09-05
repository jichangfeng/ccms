<div id="crumbs">
  <a href="<?php echo base_path().'cmsfield/contentfield/'.$Category->id.'/list';?>"><?php echo $Category->name;?></a> <em>›</em> <?php echo ts('编辑', 'ucwords', 'default');?>
</div>
<?php $arrHtmlCssJs = array();?>
<form style="width: 100%;margin: 0 auto;" method="post" action="<?php echo $action;?>" enctype="multipart/form-data">
  <div class="formArea">
    <div class="formTit"><?php echo $Category->name . ' ' . ts('编辑', 'ucwords', 'default');?></div>
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
      <?php if(!empty($arrFields_field)):?>
      <fieldset id="edit-field-args" class="collapsible form-wrapper collapse-processed collapsed">
        <legend>
          <a href="javascript:;" onclick="$('#edit-field-args .fieldset-wrapper').toggle('slow');">字段参数</a>
          <span style="color: #999999;font-size: 0.9em;margin-left: 0.5em;">慎重修改</span>
        </legend>
        <div class="fieldset-wrapper" style="display: none;">
        <?php foreach($arrFields_field as $Field):?>
          <?php $fieldTag = $Field->fieldname;?>
          <?php $fieldTag = $Field->fieldname;?>
          <?php $arrHtmlCssJs = array_merge($arrHtmlCssJs, MiniFieldRepository::HtmlCssJsOfEditor($Field->editor));?>
          <?php include CMSDATA_MODULE_PATH.'/tpl/extends/data-editor-basic-single.tpl.php';?>
          <?php include CMSDATA_MODULE_PATH.'/tpl/extends/data-editor-basic-multiple.tpl.php';?>
          <?php include CMSDATA_MODULE_PATH.'/tpl/extends/data-editor-extend-single.tpl.php';?>
          <?php include CMSDATA_MODULE_PATH.'/tpl/extends/data-editor-extend-multiple.tpl.php';?>
        <?php endforeach;?>
        </div>
      </fieldset>
      <?php endif;?>
      <?php if(!empty($arrFields_editor)):?>
      <fieldset id="edit-editor-args" class="collapsible form-wrapper collapse-processed collapsed">
        <legend>
          <a href="javascript:;" onclick="$('#edit-editor-args .fieldset-wrapper').toggle('slow');">编辑控件参数</a>
          <span style="color: #999999;font-size: 0.9em;margin-left: 0.5em;">慎重修改</span>
        </legend>
        <div class="fieldset-wrapper" style="display: none;">
        <?php foreach($arrFields_editor as $Field):?>
          <?php $fieldTag = $Field->fieldname;?>
          <?php $fieldTag = $Field->fieldname;?>
          <?php $arrHtmlCssJs = array_merge($arrHtmlCssJs, MiniFieldRepository::HtmlCssJsOfEditor($Field->editor));?>
          <?php include CMSDATA_MODULE_PATH.'/tpl/extends/data-editor-basic-single.tpl.php';?>
          <?php include CMSDATA_MODULE_PATH.'/tpl/extends/data-editor-basic-multiple.tpl.php';?>
          <?php include CMSDATA_MODULE_PATH.'/tpl/extends/data-editor-extend-single.tpl.php';?>
          <?php include CMSDATA_MODULE_PATH.'/tpl/extends/data-editor-extend-multiple.tpl.php';?>
        <?php endforeach;?>
        </div>
      </fieldset>
      <?php endif;?>
      <div class="form-actions">
        <input type="submit" class="form-submit" value="<?php echo ts('保存', 'ucwords', 'default');?>">&nbsp;
        <input type="reset" class="form-submit" value="<?php echo ts('重置', 'ucwords', 'default');?>">
      </div>
    </div>
  </div>
</form>
<?php if(isset($arrHtmlCssJs['js_jquery'])){unset($arrHtmlCssJs['js_jquery']);};echo implode('', $arrHtmlCssJs);?>