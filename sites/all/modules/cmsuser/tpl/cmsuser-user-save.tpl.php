<div id="crumbs">
  <a href="<?php echo base_path().'cmsuser/user/'.$Role->rid.'/list';?>"><?php echo $Role->name;?></a> <em>›</em> <?php echo ts('编辑', 'ucwords', 'default');?>
</div>
<?php $arrHtmlCssJs = array();?>
<form style="width: 100%;margin: 0 auto;" method="post" action="<?php echo $action;?>" enctype="multipart/form-data">
  <div class="formArea">
    <div class="formTit"><?php echo ts('网站用户', 'ucwords', 'default') . ' ' . ts('编辑', 'ucwords', 'default');?></div>
    <div class="formEle">
      <div class="form-item<?php echo isset($errors['name']) ?' error': '';?>">
        <label><?php echo ts('用户名', 'ucwords', 'user');?> <sub  title="This field is required." class="form-required">*</sub></label>
        <input class="form-text<?php echo isset($errors['name']) ?' error': '';?>" type="text" style="" value="<?php echo $Data->name;?>" name="name" />
        <?php if(isset($errors['name'])):?><span class="helpMsg"><?php echo $errors['name'];?></span><?php endif;?>
        <div class="description">“<?php echo ts('唯一', 'ucfirst', 'default');?>”</div>
      </div>
      <div class="form-item<?php echo isset($errors['pass']) ?' error': '';?>">
        <label><?php echo ts('密码', 'ucwords', 'user');?> <sub  title="This field is required." class="form-required">*</sub></label>
        <input class="form-text<?php echo isset($errors['pass']) ?' error': '';?>" type="password" style="" value="" name="pass" />
        <?php if(isset($errors['pass'])):?><span class="helpMsg"><?php echo $errors['pass'];?></span><?php endif;?>
        <div class="description">“<?php echo ts('不修改时请留空', 'ucfirst', 'user');?>”</div>
      </div>
      <div class="form-item<?php echo isset($errors['mail']) ?' error': '';?>">
        <label><?php echo ts('邮箱', 'ucwords', 'user');?> <sub  title="This field is required." class="form-required">*</sub></label>
        <input class="form-text<?php echo isset($errors['mail']) ?' error': '';?>" type="text" style="" value="<?php echo $Data->mail;?>" name="mail" />
        <?php if(isset($errors['mail'])):?><span class="helpMsg"><?php echo $errors['mail'];?></span><?php endif;?>
        <div class="description">“<?php echo ts('唯一', 'ucfirst', 'default');?>”</div>
      </div>
      <div class="form-item<?php echo isset($errors['mobile']) ?' error': '';?>">
        <label><?php echo ts('手机号', 'ucwords', 'user');?> <sub  title="This field is required." class="form-required">*</sub></label>
        <input class="form-text<?php echo isset($errors['mobile']) ?' error': '';?>" type="text" style="" value="<?php echo $Data->mobile;?>" name="mobile" />
        <?php if(isset($errors['mobile'])):?><span class="helpMsg"><?php echo $errors['mobile'];?></span><?php endif;?>
        <div class="description">“<?php echo ts('唯一', 'ucfirst', 'default');?>”</div>
      </div>
      <div class="form-item<?php echo isset($errors['status']) ?' error': '';?>">
        <label><?php echo ts('状态', 'ucwords', 'user');?> <sub  title="This field is required." class="form-required">*</sub></label>
        <label style="display: inline;"><input title="<?php echo ts('启用', 'ucwords', 'user');?>" type="radio" style="" value="1" name="status"<?php echo $Data->status == 1 ? ' checked="checked"' : '';?> /><span style='color: green;'><?php echo ts('启用', 'ucwords', 'user');?></span></label>
        <label style="display: inline;"><input title="<?php echo ts('禁用', 'ucwords', 'user');?>" type="radio" style="" value="0" name="status"<?php echo $Data->status != 1 ? ' checked="checked"' : '';?> /><span style='color: red;'><?php echo ts('禁用', 'ucwords', 'user');?></span></label>
        <?php if(isset($errors['status'])):?><span class="helpMsg"><?php echo $errors['status'];?></span><?php endif;?>
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