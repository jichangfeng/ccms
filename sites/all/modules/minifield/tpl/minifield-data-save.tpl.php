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
  <div class="form-actions">
    <input type="submit" class="form-submit" value="保存">
  </div>
</form>
<?php if(isset($arrHtmlCssJs['js_jquery'])){unset($arrHtmlCssJs['js_jquery']);};echo implode('', $arrHtmlCssJs);?>