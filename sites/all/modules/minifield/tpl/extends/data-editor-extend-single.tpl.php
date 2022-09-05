<?php if($Field->editor == "pcas"):?>
  <div class="form-item"<?php echo $Field->editor_shown == 0 ? ' style="display: none;"' : '';?>>
    <label><?php echo $Field->name;?> <?php if($Field->required == 1):?><span title="This field is required." class="form-required">*</span><?php endif;?></label>
    <?php $province = is_array($Data->$fieldTag)&&count($Data->$fieldTag)>0 ? array_shift($Data->$fieldTag) : '';?>
    <?php $city = is_array($Data->$fieldTag)&&count($Data->$fieldTag)>0 ? array_shift($Data->$fieldTag) : '';?>
    <?php $area = is_array($Data->$fieldTag)&&count($Data->$fieldTag)>0 ? array_shift($Data->$fieldTag) : '';?>
    <?php $street = is_array($Data->$fieldTag)&&count($Data->$fieldTag)>0 ? array_shift($Data->$fieldTag) : '';?>
    <select name="<?php echo $fieldTag;?>[0]"></select>
    <select name="<?php echo $fieldTag;?>[1]"></select>
    <select name="<?php echo $fieldTag;?>[2]"></select>
    <input type="text" style="<?php echo $Field->editor_style;?>" value="<?php echo $street;?>" name="<?php echo $fieldTag;?>[3]" />
    <div class="description"><?php echo $Field->editor_help;?></div>
    <script>
      $(function(){
        new PCAS("<?php echo $fieldTag;?>[0]","<?php echo $fieldTag;?>[1]","<?php echo $fieldTag;?>[2]", "<?php echo $province;?>", "<?php echo $city;?>", "<?php echo $area;?>");
      })
    </script>
  </div>
<?php elseif($Field->editor == "pcasdict"):?>
  <div class="form-item"<?php echo $Field->editor_shown == 0 ? ' style="display: none;"' : '';?>>
    <label><?php echo $Field->name;?> <?php if($Field->required == 1):?><span title="This field is required." class="form-required">*</span><?php endif;?></label>
    <select id="pcasdict<?php echo $fieldTag;?>"></select>
    <div class="description"><?php echo $Field->editor_help;?></div>
    <script>
      $(function(){
        $('#pcasdict<?php echo $fieldTag;?>').pcasDict('<?php echo $fieldTag;?>', '<?php echo $Data->$fieldTag;?>', {});
      })
    </script>
  </div>
<?php elseif($Field->editor == "textselect"):?>
  <div class="form-item"<?php echo $Field->editor_shown == 0 ? ' style="display: none;"' : '';?>>
    <label><?php echo $Field->name;?> <?php if($Field->required == 1):?><span title="This field is required." class="form-required">*</span><?php endif;?></label>
    <label style="display: inline;"><input type="text" style="<?php echo $Field->editor_style;?>" value="<?php $selectedValue = $Data->$fieldTag;echo isset($selectedValue[0][0]) ? $selectedValue[0][0] : '';?>" name="<?php echo $fieldTag;?>" /></label>
    <select onchange="$('input[name=<?php echo $fieldTag;?>]').val($(this).val());">
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
<?php elseif($Field->editor == "texttags"):?>
  <div class="form-item"<?php echo $Field->editor_shown == 0 ? ' style="display: none;"' : '';?>>
    <label><?php echo $Field->name;?> <?php if($Field->required == 1):?><span title="This field is required." class="form-required">*</span><?php endif;?></label>
    <input type="hidden" style="<?php echo $Field->editor_style;?>" value="<?php echo $Data->$fieldTag;?>" name="<?php echo $fieldTag;?>" />
    <div class="taglist_<?php echo $fieldTag;?>">
      <span style="display: inline-block;border: solid 1px #c6c6c6;padding: 2px;margin: 2px;"><?php echo $Data->$fieldTag;?></span><br />
    <?php $selectTags = explode(',', $Data->$fieldTag);?>
    <?php foreach($Field->arrValueArrays as $ValueArray):?>
      <?php if(in_array($ValueArray[0], $selectTags)):?>
      <a style='color: red;text-decoration: underline;' onclick="deltag_<?php echo $fieldTag;?>(this);" rel="<?php echo $ValueArray[0];?>" href="javascript:;"><?php echo $ValueArray[1];?></a>
      <?php else:?>
      <a style='color: black;text-decoration: underline;' onclick="addtag_<?php echo $fieldTag;?>(this);" rel="<?php echo $ValueArray[0];?>" href="javascript:;"><?php echo $ValueArray[1];?></a>
      <?php endif;?>
    <?php endforeach;?>
    </div>
    <script type="text/javascript">
      function addtag_<?php echo $fieldTag;?>(obj){
        tag = $(obj).attr('rel');
        tagsval = $('input[name=<?php echo $fieldTag;?>]').val();
        $('input[name=<?php echo $fieldTag;?>]').val(tagsval + ',' + tag);
        tags = gettags_<?php echo $fieldTag;?>();
        $('input[name=<?php echo $fieldTag;?>]').val(tags.join(','));
        rendertags_<?php echo $fieldTag;?>(tags);
      }
      function deltag_<?php echo $fieldTag;?>(obj){
        tag = $(obj).attr('rel');
        tags = gettags_<?php echo $fieldTag;?>();
        for(var i=0;i<tags.length;i++){
          if(tags[i] === tag){
            tags.splice(i,1);
            i--;
          }
        }
        $('input[name=<?php echo $fieldTag;?>]').val(tags.join(','));
        rendertags_<?php echo $fieldTag;?>(tags);
      }
      function gettags_<?php echo $fieldTag;?>(){
        tagsval = $('input[name=<?php echo $fieldTag;?>]').val();
        tagsval = tagsval.replace(/，/g,",");
        tagsval = tagsval.replace(/\s/g,"");
        tags = tagsval.split(',');
        for(var i=0;i<tags.length;i++){
          for(var j=i+1;j<tags.length;j++){
            if(tags[j]===tags[i]) {
              tags.splice(j,1);
              j--;
            }
          }
        }
        for(var k=0;k<tags.length;k++){
          if(tags[k] === ""){
            tags.splice(k,1);
            k--;
          }
        }
        return tags;
      }
      function rendertags_<?php echo $fieldTag;?>(tags){
        $('.taglist_<?php echo $fieldTag;?> span').text(tags.join(','));
        $('.taglist_<?php echo $fieldTag;?> a').css('color', 'black');
        $('.taglist_<?php echo $fieldTag;?> a').attr('onclick', 'addtag_<?php echo $fieldTag;?>(this);');
        for(var m=0;m<tags.length;m++){
          $('.taglist_<?php echo $fieldTag;?> a[rel="'+tags[m]+'"]').css('color', 'red');
          $('.taglist_<?php echo $fieldTag;?> a[rel="'+tags[m]+'"]').attr('onclick', 'deltag_<?php echo $fieldTag;?>(this);');
        }
      }
    </script>
    <div class="description"><?php echo $Field->editor_help;?></div>
  </div>
<?php elseif($Field->editor == "datepicker"):?>
  <div class="form-item"<?php echo $Field->editor_shown == 0 ? ' style="display: none;"' : '';?>>
    <label><?php echo $Field->name;?> <?php if($Field->required == 1):?><span title="This field is required." class="form-required">*</span><?php endif;?></label>
    <label style="display: inline;"><input type="text" readonly="readonly" style="<?php echo $Field->editor_style;?>" value="<?php echo $Data->$fieldTag->date;?>" name="<?php echo $fieldTag;?>" /></label>
    <label style="display: inline;">
      <select name="<?php echo $fieldTag;?>hour"><?php for($i=0; $i<=23; $i++):?><option<?php echo $Data->$fieldTag->hour==$i?' selected="selected"':'';?> value="<?php echo $i;?>"><?php echo $i;?></option><?php endfor;?></select> 时
      <select name="<?php echo $fieldTag;?>minute"><?php for($i=0; $i<=59; $i++):?><option<?php echo $Data->$fieldTag->minute==$i?' selected="selected"':'';?> value="<?php echo $i;?>"><?php echo $i;?></option><?php endfor;?></select> 分
      <select name="<?php echo $fieldTag;?>second"><?php for($i=0; $i<=59; $i++):?><option<?php echo $Data->$fieldTag->second==$i?' selected="selected"':'';?> value="<?php echo $i;?>"><?php echo $i;?></option><?php endfor;?></select> 秒
    </label>
    <div class="description"><?php echo $Field->editor_help;?></div>
    <script>
      $(function(){
        $.datepicker.setDefaults($.extend($.datepicker.regional['<?php echo ts('zh-CN', '', 'default');?>']));
        $('input[name="<?php echo $fieldTag;?>"]').datepicker({dateFormat: 'yy-mm-dd', changeYear: true, changeMonth: true, yearRange :'<?php $nowyear = date('Y', time());echo ($nowyear - 100) . ':' . ($nowyear + 100);?>'});
      })
    </script>
  </div>
<?php elseif($Field->editor == "farbtastic"):?>
  <div class="form-item"<?php echo $Field->editor_shown == 0 ? ' style="display: none;"' : '';?>>
    <label><?php echo $Field->name;?> <?php if($Field->required == 1):?><span title="This field is required." class="form-required">*</span><?php endif;?></label>
    <label style="display: inline;"><input type="text" style="<?php echo $Field->editor_style;?>" value="<?php echo empty($Data->$fieldTag) ? '#000000' : $Data->$fieldTag;?>" name="<?php echo $fieldTag;?>" id="<?php echo $fieldTag;?>panel" /></label>
    <div id="<?php echo $fieldTag;?>control"></div>
    <div class="description"><?php echo $Field->editor_help;?></div>
    <script>
      $(function(){
        $('#<?php echo $fieldTag;?>control').farbtastic('#<?php echo $fieldTag;?>panel');
      })
    </script>
  </div>
<?php elseif($Field->editor == "baidumappoint"):?>
  <div class="form-item"<?php echo $Field->editor_shown == 0 ? ' style="display: none;"' : '';?>>
    <label><?php echo $Field->name;?> <?php if($Field->required == 1):?><span title="This field is required." class="form-required">*</span><?php endif;?></label>
    <input type="hidden" style="<?php echo $Field->editor_style;?>" value="<?php echo $Data->$fieldTag;?>" name="<?php echo $fieldTag;?>" />
    <div id="<?php echo $fieldTag;?>map" style="width: 100%;height: 300px;"></div>
    <div class="description"><?php echo $Field->editor_help;?></div>
    <script>
      $(function(){
        <?php echo $fieldTag;?>assemblebaidupoint();
      })
      function <?php echo $fieldTag;?>assemblebaidupoint(){
        var map = new BMap.Map("<?php echo $fieldTag;?>map");
        map.setCurrentCity("北京");
        //map.setMinZoom(9);
        //map.setMaxZoom(19);
        //map.centerAndZoom("北京", 7);
        var point = new BMap.Point(116.404, 39.915);
        <?php if(!empty($Data->$fieldTag)):?>
          var tmppoint = "<?php echo $Data->$fieldTag;?>";
          point = new BMap.Point(tmppoint.split(",")[0], tmppoint.split(",")[1]);
          var marker = new BMap.Marker(point);
          var label = new BMap.Label('坐标|Coordinates：' + point.lng + ',' + point.lat);
          label.setOffset(new BMap.Size(marker.getIcon().size.width / 2, -20));
          marker.setLabel(label);
          map.addOverlay(marker);
          //marker.setAnimation(BMAP_ANIMATION_BOUNCE);
        <?php endif;?>
        map.centerAndZoom(point, 7);
        map.setDefaultCursor("default");
        map.addControl(new BMap.MapTypeControl());
        map.addControl(new BMap.NavigationControl());
        map.addControl(new BMap.OverviewMapControl());
        map.enableDragging();
        map.enableInertialDragging();
        map.enableScrollWheelZoom();
        map.disableKeyboard();
        var newMarker = null;
        map.addEventListener("click", function(e){
          if(e.overlay){
            //overlay clicked
          }else{
            //map clicked
          }
          map.removeOverlay(newMarker);
          var marker = new BMap.Marker(e.point);
          newMarker = marker;
          map.addOverlay(newMarker);
          var iw = new BMap.InfoWindow("<b>坐标|Coordinates</b><div>" + e.point.lng + ',' + e.point.lat + "</div>");
          map.openInfoWindow(iw, e.point);
          $('input[name="<?php echo $fieldTag;?>"]').val(e.point.lng + ',' + e.point.lat);
        });
      }
    </script>
  </div>
<?php elseif($Field->editor == "browserfile"):?>
  <div class="form-item"<?php echo $Field->editor_shown == 0 ? ' style="display: none;"' : '';?>>
    <label><?php echo $Field->name;?> <?php if($Field->required == 1):?><span title="This field is required." class="form-required">*</span><?php endif;?></label>
    <label style="display: inline;"><input type="text" readonly="readonly" style="<?php echo $Field->editor_style;?>" value="<?php echo $Data->$fieldTag;?>" name="<?php echo $fieldTag;?>" /></label>
    <label style="display: inline;">
      <input type="button" value="Browse Server" onclick="CKFinder.popup({selectActionFunction:SetFileFieldOf<?php echo $fieldTag;?>});" />
    </label>
    <div class="description"><?php echo $Field->editor_help;?></div>
    <script>
      function SetFileFieldOf<?php echo $fieldTag;?>(fileUrl){
        $('input[name="<?php echo $fieldTag;?>"]').val(fileUrl);
      }
    </script>
  </div>
<?php elseif($Field->editor == "ckeditor"):?>
  <div class="form-item"<?php echo $Field->editor_shown == 0 ? ' style="display: none;"' : '';?>>
    <label><?php echo $Field->name;?> <?php if($Field->required == 1):?><span title="This field is required." class="form-required">*</span><?php endif;?></label>
    <span style="display: inline-block;<?php echo $Field->editor_style;?>">
      <textarea name="<?php echo $fieldTag;?>" id="<?php echo $fieldTag;?>ckeditor"><?php echo $Data->$fieldTag;?></textarea>
    </span>
    <div class="description"><?php echo $Field->editor_help;?></div>
    <script>
      $(function(){
        CKEDITOR.config.toolbar_Full[10].items[4] = "lineheight";
        CKEDITOR.config.toolbar_Full[10].items[5] = "autoformat";
        CKEDITOR.replace('<?php echo $fieldTag;?>ckeditor', {
          //toolbar : CKEDITOR.config.toolbar_Full,
          extraPlugins : 'lineheight,autoformat',
          filebrowserBrowseUrl : '<?php echo base_path();?>misc/resources/ckfinder/ckfinder.html',
          filebrowserImageBrowseUrl : '<?php echo base_path();?>misc/resources/ckfinder/ckfinder.html?Type=Images',
          filebrowserFlashBrowseUrl : '<?php echo base_path();?>misc/resources/ckfinder/ckfinder.html?Type=Flash',
          filebrowserUploadUrl : '<?php echo base_path();?>misc/resources/ckfinder/core/connector/php/connector.php?command=QuickUpload&type=Files',
          filebrowserImageUploadUrl : '<?php echo base_path();?>misc/resources/ckfinder/core/connector/php/connector.php?command=QuickUpload&type=Images',
          filebrowserFlashUploadUrl : '<?php echo base_path();?>misc/resources/ckfinder/core/connector/php/connector.php?command=QuickUpload&type=Flash'
        });
        /* CKEDITOR.instances.<?php echo $fieldTag;?>ckeditor.config.toolbar = [
          { name: 'basicstyles', items : [ 'Bold','Italic','Underline','TextColor','-','RemoveFormat' ] },
          { name: 'insert', items : [ 'Smiley' ] }
        ]; */
        //var <?php echo $fieldTag;?> = CKEDITOR.instances.<?php echo $fieldTag;?>ckeditor.getData();
      })
    </script>
  </div>
<?php elseif($Field->editor == "ueditor"):?>
  <div class="form-item"<?php echo $Field->editor_shown == 0 ? ' style="display: none;"' : '';?>>
    <label><?php echo $Field->name;?> <?php if($Field->required == 1):?><span title="This field is required." class="form-required">*</span><?php endif;?></label>
    <span style="display: inline-block;<?php echo $Field->editor_style;?>">
      <script type="text/plain" name="<?php echo $fieldTag;?>" id="<?php echo $fieldTag;?>ueditor"><?php echo $Data->$fieldTag;?></script>
    </span>
    <div class="description"><?php echo $Field->editor_help;?></div>
    <script>
      $(function(){
        UE.getEditor('<?php echo $fieldTag;?>ueditor');
        /* UE.getEditor('<?php echo $fieldTag;?>ueditor',{
          toolbars: [['bold', 'italic', 'underline', 'forecolor', 'removeformat', 'formatmatch', '|', 'emotion']],
          contextMenu:[],
          initialFrameWidth: 500,
          initialFrameHeight: 200,
          autoHeightEnabled: false,
          initialContent: '',
          autoClearinitialContent: true,
          focus: false,
          wordCount: false,
          elementPathEnabled: false
        }); */
        //var <?php echo $fieldTag;?> = UE.getEditor('<?php echo $fieldTag;?>ueditor').getContent();
      })
    </script>
  </div>
<?php endif;?>