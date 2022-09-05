<?php if($Field->editor == "text"):?>
  <div class="form-item"<?php echo $Field->editor_shown == 0 ? ' style="display: none;"' : '';?>>
    <label><?php echo $Field->name;?> <?php if($Field->required == 1):?><span title="This field is required." class="form-required">*</span><?php endif;?></label>
    <input type="text" style="<?php echo $Field->editor_style;?>" value="<?php echo $Data->$fieldTag;?>" name="<?php echo $fieldTag;?>" />
    <div class="description"><?php echo $Field->editor_help;?></div>
  </div>
<?php elseif($Field->editor == "hidden"):?>
  <div class="form-item"<?php echo $Field->editor_shown == 0 ? ' style="display: none;"' : '';?>>
    <label><?php echo $Field->name;?> <?php if($Field->required == 1):?><span title="This field is required." class="form-required">*</span><?php endif;?></label>
    <input type="hidden" style="<?php echo $Field->editor_style;?>" value="<?php echo $Data->$fieldTag;?>" name="<?php echo $fieldTag;?>" />
    <div class="description"><?php echo $Field->editor_help;?></div>
  </div>
<?php elseif($Field->editor == "password"):?>
  <div class="form-item"<?php echo $Field->editor_shown == 0 ? ' style="display: none;"' : '';?>>
    <label><?php echo $Field->name;?> <?php if($Field->required == 1):?><span title="This field is required." class="form-required">*</span><?php endif;?></label>
    <input type="password" style="<?php echo $Field->editor_style;?>" value="" name="<?php echo $fieldTag;?>" />
    <div class="description"><?php echo $Field->editor_help;?></div>
  </div>
<?php elseif($Field->editor == "radio"):?>
  <div class="form-item"<?php echo $Field->editor_shown == 0 ? ' style="display: none;"' : '';?>>
    <input type="hidden" name="<?php echo 'switchof'.$fieldTag;?>" />
    <label><?php echo $Field->name;?> <?php if($Field->required == 1):?><span title="This field is required." class="form-required">*</span><?php endif;?></label>
    <?php foreach($Field->arrValueArrays as $ValueArray):?>
    <label style="display: inline;"><input title="<?php echo $ValueArray[2];?>" type="radio" style="<?php echo $Field->editor_style;?>" value="<?php echo $ValueArray[0];?>"<?php echo MiniFieldData::InValueArrays($ValueArray, $Data->$fieldTag)?' checked="checked"':'';?> name="<?php echo $fieldTag;?>" /><?php echo $ValueArray[1];?></label>
    <?php endforeach;?>
    <div class="description"><?php echo $Field->editor_help;?></div>
  </div>
<?php elseif($Field->editor == "checkbox"):?>
  <div class="form-item"<?php echo $Field->editor_shown == 0 ? ' style="display: none;"' : '';?>>
    <input type="hidden" name="<?php echo 'switchof'.$fieldTag;?>" />
    <label><?php echo $Field->name;?> <?php if($Field->required == 1):?><span title="This field is required." class="form-required">*</span><?php endif;?></label>
    <?php foreach($Field->arrValueArrays as $ValueArray):?>
    <label style="display: inline;"><input title="<?php echo $ValueArray[2];?>" style="<?php echo $Field->editor_style;?>" onclick="$('input[name=<?php echo $fieldTag;?>]').removeAttr('checked');$(this).attr('checked', 'checked');" type="checkbox" value="<?php echo $ValueArray[0];?>"<?php echo MiniFieldData::InValueArrays($ValueArray, $Data->$fieldTag)?' checked="checked"':'';?> name="<?php echo $fieldTag;?>" /><?php echo $ValueArray[1];?></label>
    <?php endforeach;?>
    <div class="description"><?php echo $Field->editor_help;?></div>
  </div>
<?php elseif($Field->editor == "select"):?>
  <div class="form-item"<?php echo $Field->editor_shown == 0 ? ' style="display: none;"' : '';?>>
    <input type="hidden" name="<?php echo 'switchof'.$fieldTag;?>" />
    <label><?php echo $Field->name;?> <?php if($Field->required == 1):?><span title="This field is required." class="form-required">*</span><?php endif;?></label>
    <select name="<?php echo $fieldTag;?>" style="<?php echo $Field->editor_style;?>">
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
    <div class="description"><?php echo $Field->editor_help;?></div>
  </div>
<?php elseif($Field->editor == "textarea"):?>
  <div class="form-item"<?php echo $Field->editor_shown == 0 ? ' style="display: none;"' : '';?>>
    <label><?php echo $Field->name;?> <?php if($Field->required == 1):?><span title="This field is required." class="form-required">*</span><?php endif;?></label>
    <textarea style="<?php echo $Field->editor_style;?>" name="<?php echo $fieldTag;?>"><?php echo $Data->$fieldTag;?></textarea>
    <div class="description"><?php echo $Field->editor_help;?></div>
  </div>
<?php elseif($Field->editor == "file"):?>
  <div class="form-item"<?php echo $Field->editor_shown == 0 ? ' style="display: none;"' : '';?>>
    <label><?php echo $Field->name;?> <?php if($Field->required == 1):?><span title="This field is required." class="form-required">*</span><?php endif;?></label>
    <input type="file" style="<?php echo $Field->editor_style;?>" value="" name="files[<?php echo $fieldTag;?>][]" />
    <label style="display: inline;">
      <?php $canDeleteFile = MiniFieldCommon::MenuAccessCheck(MINIFIELD_MENU_PATH.'/file/%/del');?>
      <?php foreach($Data->$fieldTag as $file):?>
        <span class="<?php echo 'files'.$fieldTag;?>" style="background-color: #DDDDDD;padding: 2px;"><a target="_blank" href="<?php echo MiniFieldFile::getFilepathByUri($file->uri);?>"><?php echo $file->filename?></a><?php if($canDeleteFile):?><input class="<?php echo $file->fid;?>" type="button" value="<?php echo ts('删除', 'ucwords', 'default');?>"/><?php endif;?></span>
      <?php endforeach;?>
    </label>
    <div class="description"><?php echo $Field->editor_help;?></div>
    <script>
      $(function(){
        $('.<?php echo 'files'.$fieldTag;?> input').click(function(){
          filearea = $(this).parent();
          $.post('<?php echo base_path().MINIFIELD_MENU_PATH;?>/file/'+$(this).attr('class')+'/del', {}, function(data){
            if(data.indexOf("success") > -1){ filearea.remove(); }else{ alert("failed!"); }
          }, 'text').error(function(XMLHttpRequest, textStatus, errorThrown){ alert(XMLHttpRequest.status + ": " + XMLHttpRequest.statusText); });
        });
      })
    </script>
  </div>
<?php endif;?>