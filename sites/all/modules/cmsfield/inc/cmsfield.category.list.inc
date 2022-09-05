<?php
function cmsfield_category_list(){
  $arrFields = MiniFieldCategoryStructure::arrFields();
  foreach($arrFields as $key=>$Field){
    if($Field->enabled != 1 || $Field->spotlight != 1){
      unset($arrFields[$key]);
    }
  }
  $arrDatas = MiniFieldCategory::LoadDatasByCid(null, 20, true);
  $variables = array(
    'Category' => MiniFieldCategoryStructure::Category(),
    'arrFields' => $arrFields,
    'arrDatas' => $arrDatas
  );
  $filepath = CMSFIELD_MODULE_PATH."/tpl/cmsfield-category-list.tpl.php";
  return MiniFieldCommon::Render($filepath, $variables);
}