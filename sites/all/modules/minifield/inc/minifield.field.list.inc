<?php
function minifield_field_list($cid){
  $arrFields = MiniFieldStructure::arrFields();
  foreach($arrFields as $key=>$Field){
    if($Field->enabled != 1 || $Field->spotlight != 1){
      unset($arrFields[$key]);
    }
  }
  $arrDatas = MiniField::LoadDatasByCid($cid, 20, true);
  $variables = array(
    'cid' => $cid,
    'arrFields' => $arrFields,
    'arrDatas' => $arrDatas
  );
  $filepath = MINIFIELD_MODULE_PATH."/tpl/minifield-field-list.tpl.php";
  return MiniFieldCommon::Render($filepath, $variables);
}