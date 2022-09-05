<div id="crumbs">
  <a href="<?php echo base_path().'cmsuser/role/list';?>"><?php echo ts('网站角色', 'ucwords', 'default');?></a> <em>›</em> <?php echo ts('编辑', 'ucwords', 'default');?>
</div>
<?php $arrHtmlCssJs = array();?>
<form style="width: 100%;margin: 0 auto;" method="post" action="<?php echo $action;?>" enctype="multipart/form-data">
  <div class="formArea">
    <div class="formTit"><?php echo ts('网站角色', 'ucwords', 'default') . ' ' . ts('编辑', 'ucwords', 'default');?></div>
    <div class="formEle">
      <div class="form-item<?php echo isset($errors['name']) ?' error': '';?>">
        <label><?php echo ts('角色名', 'ucwords', 'role');?> <sub  title="This field is required." class="form-required">*</sub></label>
        <input class="form-text<?php echo isset($errors['name']) ?' error': '';?>" type="text" style="" value="<?php echo $Data->name;?>" name="name" />
        <?php if(isset($errors['name'])):?><span class="helpMsg"><?php echo $errors['name'];?></span><?php endif;?>
        <div class="description">“<?php echo ts('唯一', 'ucfirst', 'default');?>”</div>
      </div>
      <div class="form-item<?php echo isset($errors['weight']) ?' error': '';?>">
        <label><?php echo ts('权重', 'ucwords', 'default');?> <sub  title="This field is required." class="form-required">*</sub></label>
        <select name="weight" style="">
          <option title="0" value="0"<?php echo $Data->weight == 0 ? ' selected="selected"' : '';?>>0</option>
          <option title="1" value="1"<?php echo $Data->weight == 1 ? ' selected="selected"' : '';?>>1</option>
          <option title="2" value="2"<?php echo $Data->weight == 2 ? ' selected="selected"' : '';?>>2</option>
          <option title="3" value="3"<?php echo $Data->weight == 3 ? ' selected="selected"' : '';?>>3</option>
          <option title="4" value="4"<?php echo $Data->weight == 4 ? ' selected="selected"' : '';?>>4</option>
          <option title="5" value="5"<?php echo $Data->weight == 5 ? ' selected="selected"' : '';?>>5</option>
          <option title="6" value="6"<?php echo $Data->weight == 6 ? ' selected="selected"' : '';?>>6</option>
          <option title="7" value="7"<?php echo $Data->weight == 7 ? ' selected="selected"' : '';?>>7</option>
          <option title="8" value="8"<?php echo $Data->weight == 8 ? ' selected="selected"' : '';?>>8</option>
          <option title="9" value="9"<?php echo $Data->weight == 9 ? ' selected="selected"' : '';?>>9</option>
        </select>
        <?php if(isset($errors['weight'])):?><span class="helpMsg"><?php echo $errors['weight'];?></span><?php endif;?>
        <div class="description"></div>
      </div>
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