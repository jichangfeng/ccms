<table>
  <thead>
    <tr>
      <th>ID</th>
      <?php if(isset($arrFields)):?>
        <?php foreach($arrFields as $Field):?>
        <th><?php echo $Field->name;?></th>
        <?php endforeach;?>
        <th>操作</th>
      <?php endif;?>
    </tr>
  </thead>
  <tbody>
  <?php if(isset($arrDatas)):?>
    <?php
      $canEditData = MiniFieldCommon::MenuAccessCheck(MINIFIELD_MENU_PATH.'/data/'.$cid.'/%/edit');
      $canDeleteData = MiniFieldCommon::MenuAccessCheck(MINIFIELD_MENU_PATH.'/data/'.$cid.'/%/delete');
    ?>
    <?php foreach($arrDatas as $key=>$Data):?>
    <tr class="<?php echo $key % 2 == 0 ? 'even' : 'odd';?>">
      <td><?php echo $Data->id;?></td>
      <?php if(isset($arrFields)):?>
        <?php foreach($arrFields as $Field):?>
          <td><?php echo MiniFieldData::GetDataOutput($Data, $Field, array('image_width' => '50px'));?></td>
        <?php endforeach;?>
        <td>
          <?php if($canEditData):?><a href="<?php echo base_path().MINIFIELD_MENU_PATH.'/data/'.$cid.'/'.$Data->id.'/edit';?>">编辑</a><?php endif;?>
          <?php if($canDeleteData):?><a href="<?php echo base_path().MINIFIELD_MENU_PATH.'/data/'.$cid.'/'.$Data->id.'/delete';?>">删除</a><?php endif;?>
        </td>
      <?php endif;?>
    </tr>
    <?php endforeach;?>
  <?php endif;?>
  </tbody>
  <tfoot><?php echo MiniFieldCommon::HtmlPager();?></tfoot>
</table>
