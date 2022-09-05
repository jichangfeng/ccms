<?php
/**
 * @author Changfeng Ji <jichf@qq.com>
 */
final class CMSSiteInfo{
  /**
   * @access private
   * @static
   * @var array Dynamic site information.
   */
  private static $dynamic = array();
  /**
   * Site information.
   * @access public
   * @static
   * @param boolean $format whether to format loaded data or not. Defaults to TRUE.
   * @param boolean $reset to reset the internal cache and load from the database; FALSE (default) to load from the internal cache, if set.
   * @return object The loaded site info object.
   */
  public static function Info($format = true, $reset = false){
    $StaticName = __CLASS__ . '_' . __FUNCTION__;
    $infos = MiniFieldStatic::GetStatic($StaticName, true);
    $infokey = $format ? 1 : 0;
    if(!is_array($infos)){
      $infos = array();
    }else if($reset && isset($infos[$infokey])){
      unset($infos[$infokey]);
      MiniFieldStatic::SetStatic($StaticName, $infos, true);
    }
    if(isset($infos[$infokey])){
      return $infos[$infokey];
    }
    $Data = MiniFieldData::FilterDatasByCid(CMSDATA_CID_SITEINFO, array(), array(), array(), array(), null, $format, 0, array('fetchObject'));
    $infos[$infokey] = $Data;
    MiniFieldStatic::SetStatic($StaticName, $infos, true);
    return $Data;
  }
  /**
   * Get a attribute of site information.
   * @access public
   * @static
   * @param string $attr The site information attribute. includes:
   *  - title:
   *  - icon
   *  - keywords
   *  - description
   *  - themepath
   *  - staticpath
   * @param boolean $dynamic whether to get dynamic attributes or not. Defaults to TRUE.
   * @param boolean $format whether to format loaded data or not. Defaults to TRUE.
   * @param boolean $reset to reset the internal cache and load from the database; FALSE (default) to load from the internal cache, if set.
   * @return mixed The loaded site info attribute.
   */
  public static function GetAttr($attr, $dynamic = true, $format = true, $reset = false){
    if($dynamic && isset(self::$dynamic[$attr])){
      return self::$dynamic[$attr];
    }
    $Data = self::Info($format, $reset);
    if(isset($Data->$attr)){
      return $Data->$attr;
    }
    return false;
  }
  /**
   * Set a dynamic attribute of site information.
   * @access public
   * @static
   * @param string $attr The site information attribute. includes:
   *  - title:
   *  - keywords
   *  - description
   * @param mixed $value The attribute value.
   */
  public static function SetAttr($attr, $value){
    if(in_array($attr, array('title', 'keywords', 'description'))){
      self::$dynamic[$attr] = $value;
    }
  }
}