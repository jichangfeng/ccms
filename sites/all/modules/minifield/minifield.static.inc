<?php
/**
 * Provides central static variable storage.
 * @author Changfeng Ji <jichf@qq.com>
 */
final class MiniFieldStatic{
  /**
   * 
   * @access private
   * @static
   * @var array Globally static array.
   */
  private static $statics = array();
  /**
   *
   * @access private
   * @static
   * @var string Cache cid prefix.
   */
  private static $cache_cid_prefix = 'minifieldcache';
  /**
   *
   * @access public
   * @static
   * @var string
   * @return string Cache cid prefix.
   */
  public static function GetCacheCidPrefix(){
    return self::$cache_cid_prefix;
  }
  /**
   *
   * @access private
   * @static
   * @var string Cache bin to store the data in.
   */
  private static $cache_bin = 'cache';
  /**
   *
   * @access public
   * @static
   * @var string
   * @return string Cache bin to store the data in.
   */
  public static function GetCacheBin(){
    return self::$cache_bin;
  }
  /**
   *
   * @access private
   * @static
   * @var int Cache expire.
   */
  private static $cache_expire = 604800;// 7 * 24 * 60 * 60
  /**
   * 
   * @access public
   * @static
   * @return boolean TRUE if current cache object is database cache, FALSE otherwise.
   */
  public static function IsDrupalDatabaseCache(){
    static $cacheObject;
    if(!isset($cacheObject)){
      $cacheObject = _cache_get_object(self::$cache_bin);
    }
    return is_a($cacheObject, 'DrupalDatabaseCache');
  }
  /**
   * 
   * @access public
   * @static
   * @param string $name
   *  Globally unique name for the variable. For a function with only one static,
   *  variable, the function name (e.g. via the PHP magic __FUNCTION__ constant)
   *  is recommended. For a function with multiple static variables add a
   *  distinguishing suffix to the function name for each one. Such as:
   *  - $demo = MiniFieldStatic::GetStatic(__FUNCTION__);
   *  - $demo = MiniFieldStatic::GetStatic(__CLASS__ . '_' . __FUNCTION__);
   * @param mixed $data The static data or NULL to remove static variable.
   * @param boolean/null $cache Whether to set to the cache or not. The default value(NULL) means depends on the cache object.
   * @param int/null $cache_expire Cache expire. Only valid at $cache is equal to TRUE. The default value(NULL) means the expiration time is 604800 seconds(1 week).
   * @return mixed The static data or NULL on failure.
   */
  public static function SetStatic($name, $data, $cache = null, $cache_expire = null){
    if(!isset($name)){
      return null;
    }
    if(!isset($data)){
      return self::ClearStatic($name);
    }
    self::$statics[$name] = $data;
    if(is_null($cache)){
      $cache = self::IsDrupalDatabaseCache() ? false : true;
    }
    if($cache){
      $cache_expire = is_numeric($cache_expire) ? $cache_expire : self::$cache_expire;
      cache_set(self::$cache_cid_prefix . $name, $data, self::$cache_bin, time() + $cache_expire);
    }
    return $data;
  }
  /**
   * 
   * @access public
   * @static
   * @param string $name
   *  Globally unique name for the variable. For a function with only one static,
   *  variable, the function name (e.g. via the PHP magic __FUNCTION__ constant)
   *  is recommended. For a function with multiple static variables add a
   *  distinguishing suffix to the function name for each one. Such as:
   *  - $demo = MiniFieldStatic::GetStatic(__FUNCTION__);
   *  - $demo = MiniFieldStatic::GetStatic(__CLASS__ . '_' . __FUNCTION__);
   * @param boolean $cache Whether to get from the cache or not. The default value(NULL) means depends on the cache object.
   * @return mixed The static data or NULL on failure.
   */
  public static function GetStatic($name, $cache = null){
    if(isset(self::$statics[$name])){
      return self::$statics[$name];
    }
    if(is_null($cache)){
      $cache = self::IsDrupalDatabaseCache() ? false : true;
    }
    if($cache){
      $cached = cache_get(self::$cache_cid_prefix . $name, self::$cache_bin);
      if($cached){
        self::$statics[$name] = $cached->data;
        return self::$statics[$name];
      }
    }
    return null;
  }
  /**
   * Clear static data and cached data if static name is equal to $name.
   * @access public
   * @static
   * @param string $name Static name.
   * @return null Return NULL.
   */
  public static function ClearStatic($name){
    if(!isset($name)){
      return null;
    }
    cache_clear_all(self::$cache_cid_prefix . $name, self::$cache_bin);
    if(isset(self::$statics[$name])){
      unset(self::$statics[$name]);
    }
    return null;
  }
  /**
   * Clear static data and cached data if static name contain a prefix $name_prefix.
   * @access public
   * @static
   * @param string $name_prefix Static name prefix.
   */
  public static function ClearStatics($name_prefix){
    cache_clear_all(self::$cache_cid_prefix . $name_prefix, self::$cache_bin, true);
    $names = array_keys(self::$statics);
    foreach($names as $name){
      if(strrpos($name, $name_prefix) === 0){
        unset(self::$statics[$name]);
      }
    }
  }
  /**
   * Clear all static data and cached data.
   * @access public
   * @static
   */
  public static function ClearStaticsAll(){
    cache_clear_all(self::$cache_cid_prefix, self::$cache_bin, true);
    self::$statics = array();
  }
}