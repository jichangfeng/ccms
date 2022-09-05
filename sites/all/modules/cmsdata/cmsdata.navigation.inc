<?php
/**
 * @author Changfeng Ji <jichf@qq.com>
 */
final class CMSNavigation{
  /**
   * Load recursive navigation list.
   * @access public
   * @static
   * @param int $parentid The navigation parent id.
   * @param mixed $includechild Recursive listed in the child navigations.
   * @param bool $format
   * @return type
   */
  public static function LoadNavigations($parentid = 0, $includechild = 0, $format = false){
    if($includechild){
      /* $arrNavigations = MiniFieldData::FilterDatasByCid(CMSDATA_CID_NAVIGATION, array(), array(), array(array('parentid', $parentid)), array(array('weight', 'ASC')), null, $format);
      if($includechild){
        foreach($arrNavigations as $key=>$Navigation){
          $arrNavigations[$key]->children = self::LoadNavigations($Navigation->id, $includechild, $format);
        }
      } */
      $arrDatas = MiniFieldData::FilterDatasByCid(CMSDATA_CID_NAVIGATION, array(), array(), array(), array(array('weight', 'ASC')), null, $format);
      $arrNavigations = array();
      foreach($arrDatas as $key => $Data){ $arrDatas[$key]->children = array(); }
      foreach($arrDatas as $Data){
        if($format == true && isset($Data->parentid[0][0]) && $Data->parentid[0][0] == $parentid){
          $arrNavigations[] = $Data;
          continue;
        }else if($format != true && isset($Data->parentid) && $Data->parentid == $parentid){
          $arrNavigations[] = $Data;
          continue;
        }
        foreach($arrDatas as $ParentData){
          if($format == true && isset($ParentData->parentid[0][0]) && $Data->id != $ParentData->id && $Data->parentid[0][0] == $ParentData->id){
            $ParentData->children[] = $Data;
            break;
          }else if($format != true && isset($ParentData->parentid) && $Data->id != $ParentData->id && $Data->parentid == $ParentData->id){
            $ParentData->children[] = $Data;
            break;
          }
        }
      }
    }else{
      $arrNavigations = MiniFieldData::FilterDatasByCid(CMSDATA_CID_NAVIGATION, array(), array(), array(array('parentid', $parentid)), array(array('weight', 'ASC')), null, $format);
    }
    return $arrNavigations;
  }
  /**
   *
   * @access public
   * @static
   * @param array $arrNavigations The recursive navigation list.
   * @param string $level initial level, defaults to ''.
   * @param string $step Defaults to '-'.
   * @return array
   */
  public static function LoadNavigationTrees($arrNavigations, $level = '', $step = '-'){
    $arrNavigationTrees = array();
    $nextlevel = $level . $step;
    foreach($arrNavigations as $Navigation){
      $Navigation->level = $level;
      $arrNavigationTrees[] = $Navigation;
      if(isset($Navigation->children) && count($Navigation->children) > 0){
        $arrNavigationTrees = array_merge($arrNavigationTrees, self::LoadNavigationTrees($Navigation->children, $nextlevel, $step));
      }
    }
    return $arrNavigationTrees;
  }
  /**
   *
   * @access public
   * @static
   * @param array $arrNavigations The recursive navigation list.
   * @return array
   */
  public static function LoadNavigationNavides($arrNavigations){
    $ides = array();
    foreach($arrNavigations as $Navigation){
      $ides[] = $Navigation->id;
      if(isset($Navigation->children) && count($Navigation->children) > 0){
        $ides = array_merge($ides, self::LoadNavigationNavides($Navigation->children));
      }
    }
    return $ides;
  }

  /**
   * Load navigation inherits list.
   * @access public
   * @static
   * @param string/int $tag The navigation id or urlalias.
   * @param bool $format
   * @param int $depth Default to null.
   */
  public static function LoadNavigationInherits($tag, $format = false, $depth = null){
    $arrNavigationInheritLists = array();
    $Navigation = self::LoadNavigationByTag($tag, $format);
    while(isset($Navigation->id) && is_numeric($Navigation->id) && $Navigation->id > 0){
      $arrNavigationInheritLists[] = $Navigation;
      if(is_numeric($depth) && count($arrNavigationInheritLists) >= $depth){  break; }
      if($format == true && isset($Navigation->parentid[0][0]) && is_numeric($Navigation->parentid[0][0]) && $Navigation->parentid[0][0] > 0){
        $Navigation = MiniFieldData::LoadDataByCidId(CMSDATA_CID_NAVIGATION, $Navigation->parentid[0][0], $format);
      }else if($format != true && isset($Navigation->parentid) && is_numeric($Navigation->parentid) && $Navigation->parentid > 0){
        $Navigation = MiniFieldData::LoadDataByCidId(CMSDATA_CID_NAVIGATION, $Navigation->parentid, $format);
      }else{
        $Navigation = null;
      }
    }
    $arrNavigationInheritLists = array_reverse($arrNavigationInheritLists);
    return $arrNavigationInheritLists;
  }

  /**
   * 
   * @access public
   * @static
   * @param int $fromparentid The source navigation id.
   * @param int $toparentid The destination navigation id.
   * @param string $urlalias_suffix
   */
  public static function CopyNavigations($fromparentid, $toparentid, $urlalias_suffix = 'new'){
    if(preg_match('/^[a-z]*$/', $urlalias_suffix) == 0){ return false; }
    $arrNavigations = self::LoadNavigations($fromparentid, 1, false);
    foreach($arrNavigations as $Navigation){
      $_POST = (array)$Navigation;
      if(isset($_POST['id'])){ unset($_POST['id']); }
      $_POST['parentid'] = $toparentid;
      $_POST['switchofparentid'] = '';
      if(isset($_POST['urlalias']) && !empty($_POST['urlalias'])){ $_POST['urlalias'] .= $urlalias_suffix; }
      $_POST['switchoftplcache'] = '';
      $_POST['switchoftplstatic'] = '';
      $_POST['switchofcategoryid'] = '';
      $_POST['switchofcategoryperm'] = '';
      $_POST['switchofdeliver'] = '';
      $_POST['switchofshown'] = '';
      $mfd = new MiniFieldData(CMSDATA_CID_NAVIGATION);
      $result = $mfd->SaveDataToDB();
      if(is_numeric($result)){
        if(count($Navigation->children)){ self::CopyNavigations($Navigation->id, $result, $urlalias_suffix); }
      }else{
        //print_r($result);
      }
    }
  }

  public static function LoadNavigationById($id, $format = false){
    $StaticName = __CLASS__ . '_' . __FUNCTION__ . '_' . $id . '_' . ($format ? 1 : 0);
    $Data = MiniFieldStatic::GetStatic($StaticName, true);
    if(isset($Data)){
      return $Data;
    }
    $Data = MiniFieldData::LoadDataByCidId(CMSDATA_CID_NAVIGATION, $id, $format);
    MiniFieldStatic::SetStatic($StaticName, $Data, true);
    return $Data;
  }
  
  /**
   *
   * @access public
   * @static
   * @param string/int $tag The navigation id or urlalias.
   * @param bool $format
   * @return type
   */
  public static function LoadNavigationByTag($tag, $format = false){
    $StaticName = __CLASS__ . '_' . __FUNCTION__ . '_' . $tag . '_' . ($format ? 1 : 0);
    $Data = MiniFieldStatic::GetStatic($StaticName, true);
    if(isset($Data)){
      return $Data;
    }
    $Data = null;
    if(is_numeric($tag)){
      $Data = MiniFieldData::LoadDataByCidId(CMSDATA_CID_NAVIGATION, $tag, $format);
    }
    if(!isset($Data->id) && !empty($tag)){
      $Data = MiniFieldData::FilterDatasByCid(CMSDATA_CID_NAVIGATION, array(), array(), array(array('urlalias', $tag)), array(), null, $format, 0, array('fetchObject'));
    }
    MiniFieldStatic::SetStatic($StaticName, $Data, true);
    return $Data;
  }
  
  public static function ExistNavigationById($id){
    $Data = self::LoadNavigationById($id);
    return isset($Data->id) ? true : false;
  }

  /**
   *
   * @access public
   * @static
   * @param string/int $tag The navigation id or urlalias.
   * @return bool
   */
  public static function ExistNavigationByTag($tag){
    $Data = self::LoadNavigationByTag($tag);
    return isset($Data->id) ? true : false;
  }

  public static function CanDeleted($tag){
    $Navigation = self::LoadNavigationByTag($tag, false);
    if(isset($Navigation->id)){
      $arrNavigations = self::LoadNavigations($Navigation->id, 1, false);
      $arrNavigationTrees = self::LoadNavigationTrees($arrNavigations);
      $arrNavigationTrees[] = $Navigation;
      $ides = array();
      foreach($arrNavigationTrees as $NavigationTree){
        if(isset($NavigationTree->categoryid) && is_numeric($NavigationTree->categoryid) && $NavigationTree->categoryid > 0){
          if(!isset($ides[$NavigationTree->categoryid]) || !is_array($ides[$NavigationTree->categoryid])){ $ides[$NavigationTree->categoryid] = array(); }
          $ides[$NavigationTree->categoryid][] = $NavigationTree->id;
        }
      }
      foreach($ides as $cid => $ids){
        $nodecount = MiniFieldData::LoadDataCountByCid($cid, array(array('navid', $ids, 'IN')));
        if($nodecount > 0){ return false; }
      }
      return true;
    }else{
      return false;
    }
  }

  public static function CanChangeCategoryId($tag){
    $Navigation = self::LoadNavigationByTag($tag, false);
    if(isset($Navigation->id)){
      if(isset($Navigation->categoryid) && is_numeric($Navigation->categoryid) && $Navigation->categoryid > 0){
        $nodecount = MiniFieldData::LoadDataCountByCid($Navigation->categoryid, array(array('navid', $Navigation->id)));
        if($nodecount > 0){ return false; }
      }
      return true;
    }else{
      return false;
    }
  }

  /**
   *
   * @access public
   * @static
   * @param string $Permission The permission type, include view add edit delete.
   * @param object/int $User The user object or user id.
   * @param object/int/string $Navigation The formatted navigation object or navigation id or navigation urlalias.
   * @param object/int/null $Node The node object or node id.
   * @return int 0 = can not manage content; 1 = manage self content; 2 = manage all content.
   */
  public static function CanManageContent($Permission, $User, $Navigation, $Node = null){
    if(!in_array($Permission, array('view', 'add', 'edit', 'delete'))){ return 0; }
    if(!isset($User->uid) || !isset($User->roles)){
      $User = user_load($User);
      if(!isset($User->uid) || !isset($User->roles)){ return 0; }
    }
    if($User->uid == 1 || isset($User->roles[3])){ return 2; }
    if(!isset($Navigation->id)){
      $Navigation = self::LoadNavigationByTag($Navigation, true);
      if(!isset($Navigation->id) || !isset($Navigation->categoryperm)){ return 0; }
    }
    $Permissions = array();
    foreach($Navigation->categoryperm as $ValueArray){
      $Permissions[] = $ValueArray[0];
    }
    $rides = array_keys($User->roles);
    $result = 0;
    foreach($rides as $rid){
      if(in_array($Permission . 'all' . $rid, $Permissions)){
        $result = 2;
        break;
      }else if(in_array($Permission . 'self' . $rid, $Permissions)){
        $result = 1;
      }
    }
    if($result == 1 && !isset($Node->userid) && is_numeric($Node)){
      $Node = CMSNode::LoadNodeByNavidId($Navigation->id, $Node, false);
    }
    if($result == 1 && isset($Node->userid) && $Node->userid != $User->uid){
      return 0;
    }
    return $result;
  }
  
  public static function DeleteNavigationById($id){
    if(self::CanDeleted($id)){
      $Data = self::LoadNavigationById($id, false);
      $arrNavigations = self::LoadNavigations($Data->id, 1, false);
      $ides = self::LoadNavigationNavides($arrNavigations);
      $ides[] = $Data->id;
      MiniFieldData::DeleteDatasByCid(CMSDATA_CID_NAVIGATION, array(array('id', $ides, 'IN')), true);
      MiniFieldStatic::ClearStatics(__CLASS__);
    }
  }
  
  /**
   *
   * @access public
   * @static
   * @param string/int $tag The navigation id or urlalias.
   */
  public static function DeleteNavigationByTag($tag){
    if(self::CanDeleted($tag)){
      $Data = self::LoadNavigationByTag($tag, false);
      $arrNavigations = self::LoadNavigations($Data->id, 1, false);
      $ides = self::LoadNavigationNavides($arrNavigations);
      $ides[] = $Data->id;
      MiniFieldData::DeleteDatasByCid(CMSDATA_CID_NAVIGATION, array(array('id', $ides, 'IN')), true);
      MiniFieldStatic::ClearStatics(__CLASS__);
    }
  }

  /**
   *
   * @access public
   * @static
   * @param array $tags Each element is a navigation id or urlalias.
   */
  public static function DeleteNavigations($tags){
    if(is_array($tags)){
      $ides = array();
      foreach($tags as $tag){
        if(self::CanDeleted($tag)){
          $Data = self::LoadNavigationByTag($tag, false);
          $arrNavigations = self::LoadNavigations($Data->id, 1, false);
          $ides = array_merge($ides, self::LoadNavigationNavides($arrNavigations));
          $ides[] = $Data->id;
        }
      }
      $ides = array_unique($ides);
      if(!empty($ides)){
        MiniFieldData::DeleteDatasByCid(CMSDATA_CID_NAVIGATION, array(array('id', $ides, 'IN')), true);
        MiniFieldStatic::ClearStatics(__CLASS__);
      }
    }
  }

  /**
   *
   * @access public
   * @static
   * @param array $id The navigation id.
   * @param boolean $reset to reset the internal cache and load from the database; FALSE (default) to load from the internal cache, if set.
   * @return string The navigation urlalias, or navigation id if not found.
   */
  public static function GetNavigationTagById($id, $reset = false){
    $StaticName = __CLASS__ . '_' . __FUNCTION__;
    $arrs = MiniFieldStatic::GetStatic($StaticName, true);
    if($reset){
      $arrs = MiniFieldStatic::ClearStatic($StaticName);
    }
    if(!isset($arrs)){
      $arrs = MiniFieldData::FilterDatasByCid(CMSDATA_CID_NAVIGATION, array('id', 'urlalias'), array(), array(array('urlalias', '', '!=')), array(), null, false, 0, array('fetchAllKeyed'));
      MiniFieldStatic::SetStatic($StaticName, $arrs, true);
    }
    return isset($arrs[$id]) ? $arrs[$id] : $id;
  }

  /**
   *
   * @access public
   * @static
   * @param array $id The navigation id.
   * @param boolean $reset to reset the internal cache and load from the database; FALSE (default) to load from the internal cache, if set.
   * @return int The navigation category id, or 0 if not found.
   */
  public static function GetNavigationCategoryIdById($id, $reset = false){
    $StaticName = __CLASS__ . '_' . __FUNCTION__;
    $arrs = MiniFieldStatic::GetStatic($StaticName, true);
    if($reset){
      $arrs = MiniFieldStatic::ClearStatic($StaticName);
    }
    if(!isset($arrs)){
      $arrs = MiniFieldData::FilterDatasByCid(CMSDATA_CID_NAVIGATION, array('id', 'categoryid'), array(), array(), array(), null, false, 0, array('fetchAllKeyed'));
      MiniFieldStatic::SetStatic($StaticName, $arrs, true);
    }
    return isset($arrs[$id]) ? $arrs[$id] : 0;
  }

  /**
   *
   * @access public
   * @static
   * @param string/int $navtag The navigation id or urlalias.
   * @return string html code.
   */
  public static function GetNavigationTplindexHtml($navtag){
    $Navigation = self::LoadNavigationByTag($navtag, true);
    $arrNodes = CMSNode::LoadNodesByNavtag($navtag, 0, 1, true);
    $variables = array();
    $variables['Navigation'] = $Navigation;
    if(empty($Navigation->tplindex) && isset($arrNodes[0])){
      return self::GetNavigationTplarticleHtml($navtag, $arrNodes[0]->id);
    }else if(empty($Navigation->tplindex)){
      $arrChildNavigations = self::LoadNavigations($Navigation->id, 1, true);
      if(isset($arrChildNavigations[0])){
        $childnavtag = !empty($arrChildNavigations[0]->urlalias) ? $arrChildNavigations[0]->urlalias : $arrChildNavigations[0]->id;
        return self::GetNavigationTplindexHtml($childnavtag);
      }
    }
    $filepath = CMSSiteInfo::GetAttr('themepath') . '/' . $Navigation->tplindex;
    if(!empty($Navigation->tplindex) && file_exists($filepath)){
      return MiniFieldCommon::Render($filepath, $variables);
    }else{
      //return '文件 ' . $filepath . ' 不存在';
      drupal_not_found();
    }
    return '';
  }

  /**
   *
   * @access public
   * @static
   * @param string/int $navtag The navigation id or urlalias.
   * @param int $nodeid The node id. it is required.
   * @return html code.
   */
  public static function GetNavigationTplarticleHtml($navtag, $nodeid){
    $Navigation = self::LoadNavigationByTag($navtag, true);
    $Node = CMSNode::LoadNodeByNavtagId($navtag, $nodeid, true);
    if(!isset($Node->id)){
      $Node = new stdClass();
      $Node->nodeid = $nodeid;
    }
    $variables = array();
    $variables['Navigation'] = $Navigation;
    $variables['Node'] = $Node;
    $filepath = CMSSiteInfo::GetAttr('themepath') . '/' . $Navigation->tplarticle;
    if(!empty($Navigation->tplarticle) && file_exists($filepath)){
      return MiniFieldCommon::Render($filepath, $variables);
    }else{
      //return '文件 ' . $filepath . ' 不存在';
      drupal_not_found();
    }
    return '';
  }

  /**
   *
   * @access public
   * @static
   * @param object $Navigation The formatted navigation object.
   * @param string $dynamicpath
   * @return string
   */
  public static function GetNavigationTplindexUri($Navigation, $dynamicpath = 'sts'){
    $navtag = !empty($Navigation->urlalias) ? $Navigation->urlalias : $Navigation->id;
    $bestatic = false;
    foreach($Navigation->tplstatic as $value){
      if(isset($value[0]) && $value[0] == 'tplindex'){
        $bestatic = true;
        break;
      }
    }
    if($bestatic == true){
      $staticpath = CMSSiteInfo::GetAttr('staticpath');
      return base_path() . (empty($staticpath) ? '' : $staticpath.'/') . 'index/' . $navtag . '.html';
    }else{
      return base_path() . (empty($dynamicpath) ? '' : $dynamicpath.'/') . $navtag;
    }
    return '';
  }

  /**
   *
   * @access public
   * @static
   * @param object $Navigation The formatted navigation object.
   * @param object/int $Node The node object or node id.
   * @param string $dynamicpath
   * @return string
   */
  public static function GetNavigationTplarticleUri($Navigation, $Node, $dynamicpath = 'sts'){
    $navtag = !empty($Navigation->urlalias) ? $Navigation->urlalias : $Navigation->id;
    $nodeid = isset($Node->id) ? $Node->id : $Node;
    $bestatic = false;
    foreach($Navigation->tplstatic as $value){
      if(isset($value[0]) && $value[0] == 'tplarticle'){
        $bestatic = true;
        break;
      }
    }
    if($bestatic == true){
      $staticpath = CMSSiteInfo::GetAttr('staticpath');
      return base_path() . (empty($staticpath) ? '' : $staticpath.'/') . 'article/' . $navtag . '-' . $nodeid . '.html';
    }else{
      return base_path() . (empty($dynamicpath) ? '' : $dynamicpath.'/') . $navtag . '/' . $nodeid;
    }
    return '';
  }

  /**
   * Generate one tpl index html.
   * @access public
   * @static
   * @param string $staticpath static html root directory.
   * @param string/int $navtag The navigation id or urlalias.
   */
  public static function GenerateTplindexHtmlByNavtag($staticpath, $navtag){
    $Navigation = self::LoadNavigationByTag($navtag, true);
    $bestatic = false;
    foreach($Navigation->tplstatic as $value){
      if(isset($value[0]) && $value[0] == 'tplindex'){
        $bestatic = true;
        break;
      }
    }
    if($bestatic == false){ return; }
    if(isset($Navigation->seotitle) && !empty($Navigation->seotitle)){
      CMSSiteInfo::SetAttr('title', $Navigation->seotitle);
    }else{
      CMSSiteInfo::SetAttr('title', $Navigation->name . '_' . CMSSiteInfo::GetAttr('title', false));
    }
    if(isset($Navigation->seokeywords) && !empty($Navigation->seokeywords)){
      CMSSiteInfo::SetAttr('keywords', $Navigation->seokeywords);
    }
    if(isset($Navigation->seodescription) && !empty($Navigation->seodescription)){
      CMSSiteInfo::SetAttr('description', $Navigation->seodescription);
    }
    $navtag = !empty($Navigation->urlalias) ? $Navigation->urlalias : $Navigation->id;
    $page_callback_result = self::GetNavigationTplindexHtml($navtag);
    if(!empty($staticpath) && !is_dir($staticpath)){
      mkdir($staticpath);
    }
    if(!is_dir((empty($staticpath) ? '' : $staticpath.'/') . 'index/')){
      mkdir((empty($staticpath) ? '' : $staticpath.'/') . 'index/');
    }
    $destination = (empty($staticpath) ? '' : $staticpath.'/') . 'index/' . $navtag . '.html';
    file_unmanaged_save_data($page_callback_result, $destination, FILE_EXISTS_REPLACE);
  }
  /**
   * Clear one tpl index html.
   * @access public
   * @static
   * @param string $staticpath static html root directory.
   * @param string/int $navtag The navigation id or urlalias.
   */
  public static function ClearTplindexHtmlByNavtag($staticpath, $navtag){
    $Navigation = self::LoadNavigationByTag($navtag, true);
    $destination = (empty($staticpath) ? '' : $staticpath.'/') . 'index/' . $Navigation->id . '.html';
    MiniFieldFile::Remove($destination);
    $destination = (empty($staticpath) ? '' : $staticpath.'/') . 'index/' . $Navigation->urlalias . '.html';
    MiniFieldFile::Remove($destination);
  }

  /**
   * Generate one or more tpl index htmls.
   * @access public
   * @static
   * @param string $staticpath static html root directory.
   * @param array $navtags An array, each element is a navigation id or urlalias.
   */
  public static function GenerateTplindexHtmlsByNavtags($staticpath, $navtags){
    foreach($navtags as $navtag){
      if(is_string($navtag) || is_numeric($navtag)){
        self::GenerateTplindexHtmlByNavtag($staticpath, $navtag);
      }
    }
  }
  /**
   * Clear one or more tpl index htmls.
   * @access public
   * @static
   * @param string $staticpath static html root directory.
   * @param array $navtags An array, each element is a navigation id or urlalias.
   */
  public static function ClearTplindexHtmlsByNavtags($staticpath, $navtags){
    foreach($navtags as $navtag){
      if(is_string($navtag) || is_numeric($navtag)){
        self::ClearTplindexHtmlByNavtag($staticpath, $navtag);
      }
    }
  }

  /**
   * Generate all tpl index htmls.
   * @access public
   * @static
   * @param string $staticpath static html root directory.
   */
  public static function GenerateTplindexHtmlsAll($staticpath){
    $arrNavigations = MiniFieldData::FilterDatasByCid(CMSDATA_CID_NAVIGATION, array(), array(), array(array('tplstatic', '%'.db_like('tplindex').'%', 'LIKE')), array(), null, false);
    foreach($arrNavigations as $Navigation){
      self::GenerateTplindexHtmlByNavtag($staticpath, $Navigation->id);
    }
  }
  /**
   * Clear all tpl index htmls.
   * @access public
   * @static
   * @param string $staticpath static html root directory.
   */
  public static function ClearTplindexHtmlsAll($staticpath){
    $destination = (empty($staticpath) ? '' : $staticpath.'/') . 'index/*.html';
    MiniFieldFile::Remove($destination);
  }

  /**
   * Generate one tpl article html of one navigation.
   * @access public
   * @static
   * @param string $staticpath static html root directory.
   * @param string/int $navtag The navigation id or urlalias.
   * @param int $nodeid The node id.
   */
  public static function GenerateTplarticleHtmlByNavtagNodeid($staticpath, $navtag, $nodeid){
    $Navigation = self::LoadNavigationByTag($navtag, true);
    $bestatic = false;
    foreach($Navigation->tplstatic as $value){
      if(isset($value[0]) && $value[0] == 'tplarticle'){
        $bestatic = true;
        break;
      }
    }
    if($bestatic == false){ return; }
    if(isset($Navigation->seotitle) && !empty($Navigation->seotitle)){
      CMSSiteInfo::SetAttr('title', $Navigation->seotitle);
    }else{
      CMSSiteInfo::SetAttr('title', $Navigation->name . '_' . CMSSiteInfo::GetAttr('title', false));
    }
    if(isset($Navigation->seokeywords) && !empty($Navigation->seokeywords)){
      CMSSiteInfo::SetAttr('keywords', $Navigation->seokeywords);
    }
    if(isset($Navigation->seodescription) && !empty($Navigation->seodescription)){
      CMSSiteInfo::SetAttr('description', $Navigation->seodescription);
    }
    $navtag = !empty($Navigation->urlalias) ? $Navigation->urlalias : $Navigation->id;
    $page_callback_result = self::GetNavigationTplarticleHtml($navtag, $nodeid);
    if(!empty($staticpath) && !is_dir($staticpath)){
      mkdir($staticpath);
    }
    if(!is_dir((empty($staticpath) ? '' : $staticpath.'/') . 'article/')){
      mkdir((empty($staticpath) ? '' : $staticpath.'/') . 'article/');
    }
    $destination = (empty($staticpath) ? '' : $staticpath.'/') . 'article/' . $navtag . '-' . $nodeid . '.html';
    file_unmanaged_save_data($page_callback_result, $destination, FILE_EXISTS_REPLACE);
  }
  /**
   * Clear one tpl article html of one navigation.
   * @access public
   * @static
   * @param string $staticpath static html root directory.
   * @param string/int $navtag The navigation id or urlalias.
   * @param int $nodeid The node id.
   */
  public static function ClearTplarticleHtmlByNavtagNodeid($staticpath, $navtag, $nodeid){
    $Navigation = self::LoadNavigationByTag($navtag, true);
    $destination = (empty($staticpath) ? '' : $staticpath.'/') . 'article/' . $Navigation->id . '-' . $nodeid . '.html';
    MiniFieldFile::Remove($destination);
    $destination = (empty($staticpath) ? '' : $staticpath.'/') . 'article/' . $Navigation->urlalias . '-' . $nodeid . '.html';
    MiniFieldFile::Remove($destination);
  }

  /**
   * Generate one or more tpl article htmls of one navigation.
   * @access public
   * @static
   * @param string $staticpath static html root directory.
   * @param string/int $navtag The navigation id or urlalias.
   * @param array $nodeides An array, each element is a node id.
   */
  public static function GenerateTplarticleHtmlsByNavtagNodeides($staticpath, $navtag, $nodeides){
    foreach($nodeides as $nodeid){
      self::GenerateTplarticleHtmlByNavtagNodeid($staticpath, $navtag, $nodeid);
    }
  }
  /**
   * Clear one or more tpl article htmls of one navigation.
   * @access public
   * @static
   * @param string $staticpath static html root directory.
   * @param string/int $navtag The navigation id or urlalias.
   * @param array $nodeides An array, each element is a node id.
   */
  public static function ClearTplarticleHtmlsByNavtagNodeides($staticpath, $navtag, $nodeides){
    foreach($nodeides as $nodeid){
      self::ClearTplarticleHtmlByNavtagNodeid($staticpath, $navtag, $nodeid);
    }
  }

  /**
   * Generate all tpl article htmls of one navigation.
   * @access public
   * @static
   * @param string $staticpath static html root directory.
   * @param string/int $navtag The navigation id or urlalias.
   */
  public static function GenerateTplarticleHtmlsAllByNavtag($staticpath, $navtag){
    $arrNodes = CMSNode::FilterNodesByNavtag($navtag, 0, array(), array(), array(array('status', 1)), array(array('created', 'DESC')), null, false);
    foreach($arrNodes as $Node){
      self::GenerateTplarticleHtmlByNavtagNodeid($staticpath, $navtag, $Node->id);
    }
  }
  /**
   * Clear all tpl article htmls of one navigation.
   * @access public
   * @static
   * @param string $staticpath static html root directory.
   * @param string/int $navtag The navigation id or urlalias.
   */
  public static function ClearTplarticleHtmlsAllByNavtag($staticpath, $navtag){
    $Navigation = self::LoadNavigationByTag($navtag, true);
    $destination = (empty($staticpath) ? '' : $staticpath.'/') . 'article/' . $Navigation->id . '-*.html';
    MiniFieldFile::Remove($destination);
    $destination = (empty($staticpath) ? '' : $staticpath.'/') . 'article/' . $Navigation->urlalias . '-*.html';
    MiniFieldFile::Remove($destination);
  }

  /**
   * Generate all tpl article htmls of one or more navigations.
   * @access public
   * @static
   * @param string $staticpath static html root directory.
   * @param array $navtags An array, each element is a navigation id or urlalias.
   */
  public static function GenerateTplarticleHtmlsAllByNavtags($staticpath, $navtags){
    foreach($navtags as $navtag){
      if(is_string($navtag) || is_numeric($navtag)){
        self::GenerateTplarticleHtmlsAllByNavtag($staticpath, $navtag);
      }
    }
  }
  /**
   * Clear all tpl article htmls of one or more navigations.
   * @access public
   * @static
   * @param string $staticpath static html root directory.
   * @param array $navtags An array, each element is a navigation id or urlalias.
   */
  public static function ClearTplarticleHtmlsAllByNavtags($staticpath, $navtags){
    foreach($navtags as $navtag){
      if(is_string($navtag) || is_numeric($navtag)){
        self::ClearTplarticleHtmlsAllByNavtag($staticpath, $navtag);
      }
    }
  }

  /**
   * Generate all tpl article htmls of all navigations.
   * @access public
   * @static
   * @param string $staticpath static html root directory.
   */
  public static function GenerateTplarticleHtmlsAll($staticpath){
    $arrNavigations = MiniFieldData::FilterDatasByCid(CMSDATA_CID_NAVIGATION, array(), array(), array(array('tplstatic', '%'.db_like('tplarticle').'%', 'LIKE')), array(), null, true);
    foreach($arrNavigations as $Navigation){
      self::GenerateTplarticleHtmlsAllByNavtag($staticpath, $Navigation->id);
    }
  }
  /**
   * Clear all tpl article htmls of all navigations.
   * @access public
   * @static
   * @param string $staticpath static html root directory.
   */
  public static function ClearTplarticleHtmlsAll($staticpath){
    $destination = (empty($staticpath) ? '' : $staticpath.'/') . 'article/*.html';
    MiniFieldFile::Remove($destination);
  }
}