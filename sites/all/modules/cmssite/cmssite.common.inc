<?php
function cmssiteGenerateNavs($navtag = ''){
  $as = array();
  $classes = array();
  $classes[] = 'first';
  if($navtag == ''){ $classes[] = 'current'; }
  $classes = !empty($classes) ? ' class="'.  implode(' ', $classes).'"' : '';
  $as[] = '<a'.$classes.' href="'.base_path().'">首页|Home</a>';
  $arrNavigationInheritLists = CMSNavigation::LoadNavigationInherits($navtag, false);
  //$arrNavigations = CMSNavigation::LoadNavigations(0, 0, true);
  $arrNavigations = MiniFieldData::FilterDatasByCid(CMSDATA_CID_NAVIGATION, array(), array(), array(array('parentid', 0), array('shown', 1)), array(array('weight', 'ASC')), null, true);
  foreach($arrNavigations as $key => $Navigation){
    if(!$Navigation->shown[0][0]){ continue; }
    $classes = array();
    //if($key == 0){ $classes[] = 'first'; }
    if($key == count($arrNavigations) - 1){ $classes[] = 'last'; }
    if(isset($arrNavigationInheritLists[0]) && $arrNavigationInheritLists[0]->id == $Navigation->id){
      $classes[] = 'current';
    }
    $classes = !empty($classes) ? ' class="'.  implode(' ', $classes).'"' : '';
    $as[] = '<a'.$classes.' href="'.CMSNavigation::GetNavigationTplindexUri($Navigation).'">'.$Navigation->name.'</a>';
  }
  return implode('', $as);
}