<?php if($Field->editor == "texts"):?>
  <div class="form-item<?php echo isset($errors[$fieldTag]) ?' error': '';?>"<?php echo $Field->editor_shown == 0 ? ' style="display: none;"' : '';?>>
    <label><?php echo $Field->name;?> <?php if($Field->required == 1):?><sub  title="This field is required." class="form-required">*</sub><?php endif;?></label>
    <?php foreach($Data->$fieldTag as $singleValue):?>
    <input class="form-text<?php echo isset($errors[$fieldTag]) ?' error': '';?>" type="text" style="<?php echo $Field->editor_style;?>" value="<?php echo $singleValue;?>" name="<?php echo $fieldTag;?>[]" />
    <?php endforeach;?>
    <input id="<?php echo 'texts'.$fieldTag.'add';?>" type="button" value="<?php echo ts('添加', 'ucwords', 'default');?>"/>
    <?php if(isset($errors[$fieldTag])):?><span class="helpMsg"><?php echo $errors[$fieldTag];?></span><?php endif;?>
    <div class="description"><?php echo $Field->editor_help?"“".$Field->editor_help."”":"";?></div>
    <script>
      $(function(){
        $('#<?php echo 'texts'.$fieldTag.'add';?>').click(function(){
          var textinput = '<input class="form-text<?php echo isset($errors[$fieldTag]) ?' error': '';?>" type="text" style="<?php echo $Field->editor_style;?>" value="" name="<?php echo $fieldTag;?>[]" /> ';
          $(this).before(textinput);
        });
      })
    </script>
  </div>
<?php elseif($Field->editor == "checkboxes"):?>
  <div class="form-item<?php echo isset($errors[$fieldTag]) ?' error': '';?>"<?php echo $Field->editor_shown == 0 ? ' style="display: none;"' : '';?>>
    <input type="hidden" name="<?php echo 'switchof'.$fieldTag;?>" />
    <label><?php echo $Field->name;?> <?php if($Field->required == 1):?><sub  title="This field is required." class="form-required">*</sub><?php endif;?></label>
    <?php foreach($Field->arrValueArrays as $ValueArray):?>
      <?php if(!empty($Field->editor_values_group) && (string)$ValueArray[0] == $Field->editor_values_group . '_begin'):?>
      <label style="display: inline;"><?php echo $ValueArray[1];?></label>
      <?php elseif(!empty($Field->editor_values_group) && (string)$ValueArray[0] == $Field->editor_values_group . '_end'):?>
      <br /><label></label>
      <?php else:?>
      <label style="display: inline;"><input title="<?php echo $ValueArray[2];?>" type="checkbox" style="<?php echo $Field->editor_style;?>" value="<?php echo $ValueArray[0];?>"<?php echo MiniFieldData::InValueArrays($ValueArray, $Data->$fieldTag)?' checked="checked"':'';?> name="<?php echo $fieldTag;?>[]" /><?php echo $ValueArray[1];?></label>
      <?php endif;?>
    <?php endforeach;?>
    <?php if(isset($errors[$fieldTag])):?><span class="helpMsg"><?php echo $errors[$fieldTag];?></span><?php endif;?>
    <div class="description"><?php echo $Field->editor_help?"“".$Field->editor_help."”":"";?></div>
  </div>
<?php elseif($Field->editor == "selects"):?>
  <div class="form-item<?php echo isset($errors[$fieldTag]) ?' error': '';?>"<?php echo $Field->editor_shown == 0 ? ' style="display: none;"' : '';?>>
    <input type="hidden" name="<?php echo 'switchof'.$fieldTag;?>" />
    <label><?php echo $Field->name;?> <?php if($Field->required == 1):?><sub  title="This field is required." class="form-required">*</sub><?php endif;?></label>
    <select name="<?php echo $fieldTag;?>[]" multiple="multiple" style="<?php echo $Field->editor_style;?>">
    <?php foreach($Field->arrValueArrays as $ValueArray):?>
      <?php if(!empty($Field->editor_values_group) && (string)$ValueArray[0] == $Field->editor_values_group . '_begin'):?>
      <optgroup title="<?php echo $ValueArray[2];?>" label="<?php echo $ValueArray[1];?>">
      <?php elseif(!empty($Field->editor_values_group) && (string)$ValueArray[0] == $Field->editor_values_group . '_end'):?>
      </optgroup>
      <?php else:?>
      <option title="<?php echo $ValueArray[2];?>" value="<?php echo $ValueArray[0];?>"<?php echo MiniFieldData::InValueArrays($ValueArray, $Data->$fieldTag)?' selected="selected"':'';?>><?php echo $ValueArray[1];?></option>
      <?php endif;?>
    <?php endforeach;?>
    </select>
    <?php if(isset($errors[$fieldTag])):?><span class="helpMsg"><?php echo $errors[$fieldTag];?></span><?php endif;?>
    <div class="description"><?php echo $Field->editor_help?"“".$Field->editor_help."”":"";?></div>
  </div>
<?php elseif($Field->editor == "files"):?>
  <div class="form-item<?php echo isset($errors[$fieldTag]) ?' error': '';?>"<?php echo $Field->editor_shown == 0 ? ' style="display: none;"' : '';?>>
    <label><?php echo $Field->name;?> <?php if($Field->required == 1):?><sub  title="This field is required." class="form-required">*</sub><?php endif;?></label>
    <input type="file" style="<?php echo $Field->editor_style;?>" value="" name="files[<?php echo $fieldTag;?>][]" />
    <input id="<?php echo 'files'.$fieldTag.'add';?>" type="button" value="<?php echo ts('添加', 'ucwords', 'default');?>"/>
    <div style="padding: 5px 0;">
      <?php $canDeleteFile = cmsMenuAccessCheck('cmsdata/file/delete');?>
      <?php foreach($Data->$fieldTag as $key=>$file):?>
        <label><?php echo $Field->name;?><?php echo $key+1;?></label><span class="<?php echo 'files'.$fieldTag;?>" style="background-color: #DDDDDD;padding: 2px;"><a target="_blank" href="<?php echo MiniFieldFile::getFilepathByUri($file->uri);?>"><?php echo $file->filename?></a><?php if($canDeleteFile):?><input class="<?php echo $file->fid;?>" type="button" value="<?php echo ts('删除', 'ucwords', 'default');?>"/><?php endif;?></span><br/>
      <?php endforeach;?>
    </div>
    <?php if(isset($errors[$fieldTag])):?><span class="helpMsg"><?php echo $errors[$fieldTag];?></span><?php endif;?>
    <div class="description"><?php echo $Field->editor_help?"“".$Field->editor_help."”":"";?></div>
    <script>
      $(function(){
        $('.<?php echo 'files'.$fieldTag;?> input').click(function(){
          filearea = $(this).parent();
          $.post('<?php echo base_path()?>cmsdata/file/delete/' + $(this).attr('class'), {}, function(data){
            if(data.indexOf("success") > -1){ filearea.remove(); }else{ alert("failed!"); }
          }, 'text').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
        });
        $('#<?php echo 'files'.$fieldTag.'add';?>').click(function(){
          var num=$("input[name='files[<?php echo $fieldTag;?>][]']").size();
          var fileinput = '<br /><label><?php echo ts('添加', 'ucwords', 'default');?>'+num+'</label> <input type="file" style="<?php echo $Field->editor_style;?>" value="" name="files[<?php echo $fieldTag;?>][]" /> ';
          $(this).before(fileinput);
        });
      })
    </script>
  </div>
<?php endif;?>
