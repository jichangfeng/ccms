<?php
function minifield_data_list($cid){
  $arrFields = MiniField::LoadDatasByCid($cid, null, false);
  foreach($arrFields as $key=>$Field){
    if($Field->enabled != 1 || $Field->spotlight != 1){
      unset($arrFields[$key]);
    }
  }
  $arrDatas = MiniFieldData::LoadDatasByCid($cid, 20, true);
  $variables = array(
    'cid' => $cid,
    'arrFields' => $arrFields,
    'arrDatas' => $arrDatas
  );
  $filepath = MINIFIELD_MODULE_PATH."/tpl/minifield-data-list.tpl.php";
  return MiniFieldCommon::Render($filepath, $variables);
}