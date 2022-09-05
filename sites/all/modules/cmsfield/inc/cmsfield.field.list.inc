<?php
function cmsfield_field_list($cid){
  $arrFields = MiniFieldStructure::arrFields();
  foreach($arrFields as $key=>$Field){
    if($Field->enabled != 1 || $Field->spotlight != 1){
      unset($arrFields[$key]);
    }
  }
  $arrDatas = MiniField::LoadDatasByCid($cid, 20, true);
  $variables = array(
    'Category' => MiniFieldCategory::LoadDataByCidId(null, $cid, true),
    'arrFields' => $arrFields,
    'arrDatas' => $arrDatas
  );
  $filepath = CMSFIELD_MODULE_PATH."/tpl/cmsfield-field-list.tpl.php";
  return MiniFieldCommon::Render($filepath, $variables);
}