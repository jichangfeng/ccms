<?php $arrHtmlCssJs = array();?>
<script src="<?php echo base_path();?>/misc/resources/jquery/jquery-1.7.2.min.js"></script>
<form method="post" action="<?php echo $action;?>" enctype="multipart/form-data">
  <input type="hidden" value="<?php echo $Data->id;?>" name="id"/>
<?php foreach($arrFields as $Field):?>
  <?php $fieldTag = $Field->fieldname;?>
  <?php $arrHtmlCssJs = array_merge($arrHtmlCssJs, MiniFieldRepository::HtmlCssJsOfEditor($Field->editor));?>
  <?php include 'extends/data-editor-basic-single.tpl.php';?>
  <?php include 'extends/data-editor-basic-multiple.tpl.php';?>
  <?php include 'extends/data-editor-extend-single.tpl.php';?>
  <?php include 'extends/data-editor-extend-multiple.tpl.php';?>
<?php endforeach;?>
  <fieldset id="edit-table-args" class="collapsible form-wrapper collapse-processed collapsed">
    <legend>
      <span class="fieldset-legend">
        <a href="javascript:;" onclick="Drupal.toggleFieldset(document.getElementById('edit-table-args'));" class="fieldset-title">
          <span class="fieldset-legend-prefix element-invisible">Hide</span>表参数</a>
        <span class="summary">慎重修改</span>
      </span>
    </legend>
    <div class="fieldset-wrapper" style="display: none;">
    <?php foreach($arrFields_table as $Field):?>
      <?php $fieldTag = $Field->fieldname;?>
      <?php $arrHtmlCssJs = array_merge($arrHtmlCssJs, MiniFieldRepository::HtmlCssJsOfEditor($Field->editor));?>
      <?php include 'extends/data-editor-basic-single.tpl.php';?>
      <?php include 'extends/data-editor-basic-multiple.tpl.php';?>
      <?php include 'extends/data-editor-extend-single.tpl.php';?>
      <?php include 'extends/data-editor-extend-multiple.tpl.php';?>
    <?php endforeach;?>
    </div>
  </fieldset>
  <div class="form-actions">
    <input type="submit" class="form-submit" value="保存字段类型">
  </div>
</form>
<?php if(isset($arrHtmlCssJs['js_jquery'])){unset($arrHtmlCssJs['js_jquery']);};echo implode('', $arrHtmlCssJs);?>