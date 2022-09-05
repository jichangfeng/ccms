<div id="crumbs">
  <a href="<?php echo base_path().'cmsnode';?>"><?php echo ts('栏目内容', 'ucwords', 'default');?></a> <em>›</em> <?php echo ts('内容结构树', 'ucwords', 'node');?>
</div>
<div class="cdata">
  <!--<div class="btnLink"></div>-->
  <!--<div class="searchArea"></div>-->
  <div class="tabTitle">
    <span class="left"><?php echo ts('内容结构树', 'ucwords', 'node');?></span>
    <span class="right"></span>
  </div>
  <table class="table_tree">
    <thead>
      <tr>
        <?php if(isset($arrFields)):?>
          <?php foreach($arrFields as $Field):?>
          <th><?php echo $Field->name;?></th>
          <?php endforeach;?>
        <?php endif;?>
          <th><?php echo ts('操作', 'ucwords', 'default');?></th>
      </tr>
    </thead>
    <tbody>
    <?php
    function RecursionNavigation($Category, $arrFields, $arrDatas = null, $depth = 0, $collapse = 'node'){
      $pading_left = $depth * 1.6 + 1;
      $trs = '';
      foreach($arrDatas as $Datakey => $Data){
        $trs_children = '';
        if(isset($Data->children)){ $trs_children .= RecursionNavigation($Category, $arrFields, $Data->children, $depth + 1, $collapse . '_' . ($Datakey + 1)); }
        $ManageContent = CMSNavigation::CanManageContent('view', $GLOBALS['user'], $Data);
        if($ManageContent == 0 && empty($trs_children)){ continue; }
        $classes = array($collapse);
        if(!empty($trs_children)){ array_push($classes, 'fold');array_push($classes, 'collapsed'); }
        if($depth > 0){ array_push($classes, "tr_hidden"); }
        $trs .= '<tr class="' . implode(' ', $classes) . '" nid="' . $collapse . '_' . ($Datakey + 1).'">';
        foreach($arrFields as $Fieldkey => $Field){
          $fieldTag = $Field->fieldname;
          $trs .= '<td style="' . ($Fieldkey == 1 ? "width:20%;text-align: left;padding-left:{$pading_left}em" : "") . '">' . ($Fieldkey == 1 ? '<span class="span_ico">&nbsp;</span>' : '');
          $trs .= MiniFieldData::GetDataOutput($Data, $Field, array('image_width' => '50px'));
          $trs .= '</td>';
        }
        $trs .= '<td style="width: 150px;">';
        if($Data->categoryid[0][0] > 0 && $ManageContent > 0){
          $nodecount = MiniFieldData::LoadDataCountByCid($Data->categoryid[0][0], array(array('navid', $Data->id)));
          $trs .= '<a class="ico-button ico-play" href="'.base_path().'cmsnode/'.$Data->id.'/list'.'">'.ts('内容', 'ucwords', 'node').'('.sprintf("%05d", $nodecount).')</a>';
        }
        $trs .= '</td>';
        $trs .= '</tr>';
        $trs .= $trs_children;
      }
      return $trs;
    }
    echo RecursionNavigation($Category, $arrFields, $arrDatas, 0, 'node');
    ?>
    </tbody>
    <tfoot></tfoot>
  </table>
  <!--<div class="chooseData"></div>-->
  <!--<div class="pagerinfo"><?php //echo MiniFieldCommon::HtmlPager();?></div>-->
</div>