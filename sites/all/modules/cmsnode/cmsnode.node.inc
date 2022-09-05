<?php
/**
 * @author Changfeng Ji <jichf@qq.com>
 */
final class CMSNode{
  /**
   * Filter datas by nav id.
   * @access final
   * @static
   * @param int $navid
   * @param type $includechild
   * @param array $fields The selected field list. Each element is a field name. Defaults to select all field(*) if $fields is empty.
   * @param array $fields_alias The selected field alias list. Each element key is a field name; each element value is a field alias.
   * @param array $conditions The filter conditions.
   *      Each element is still a array with two to three elements <b>array($field, $value[, $operator])</b>
   *      or an arbitrary WHERE clause to the query <b>array($snippet, $args, $where)</b>
   *      or nested conditionals <b>array('conjunction' => '$conjunction', 'conditions' => $conditions)</b>:
   *       - $field: The name of the field to check. If you would like to add a more complex condition involving operators or functions, use where().
   *       - $value: The value to test the field against. In most cases, this is a scalar. For more complex options, it is an array. The meaning of each element in the array is dependent on the $operator.
   *       - $operator: The comparison operator, such as =, !=, <, >, <=, or >=. It also accepts more complex options such as IN, NOT IN, LIKE, NOT LIKE, or BETWEEN. Defaults to IN if $value is an array, and = otherwise.
   *       - $snippet: A portion of a WHERE clause as a prepared statement. It must use named placeholders, not ? placeholders.
   *       - $args: An associative array of arguments.
   *       - $where: The unique value is WHERE.
   *       - $conjunction: The conjunctions include AND, OR, XOR.
   * @param array $orderbys Orders the result set by a given field.
   *      Each element is still a array with one to two elements($field[, $direction]):
   *       - $field: The field on which to order.
   *       - $direction: The direction to sort. Legal values are "ASC" or "DESC" or "RANDOM"($field is invalid), Default to "ASC".
   * @param type $limit The page limit or null.
   * @param boolean $format whether to format loaded data or not.
   * @param int $element An optional integer to distinguish between multiple pagers on one page.
   * @return array The filtered data array.
   */
  public static function FilterNodesByNavid($navid, $includechild = 0, $fields = array(), $fields_alias = array(), $conditions = array(), $orderbys = array(), $limit = null, $format = false, $element = 0){
    $Navigation = CMSNavigation::LoadNavigationById($navid, true);
    if(!isset($Navigation->id)){ return array(); }
    if($includechild){
      $arrNavigations = CMSNavigation::LoadNavigations($Navigation->id, 1, false);
      $navides = CMSNavigation::LoadNavigationNavides($arrNavigations);
      $navides[] = $Navigation->id;
      $conditions[] = array('navid', $navides, 'IN');
    }else{
      $conditions[] = array('navid', $Navigation->id);
    }
    $arrDatas = MiniFieldData::FilterDatasByCid($Navigation->categoryid[0][0], $fields, $fields_alias, $conditions, $orderbys, $limit, $format, $element);
    return $arrDatas;
  }

  /**
   * Filter datas by nav tag.
   * @access final
   * @static
   * @param string/int $navtag The navigation id or urlalias.
   * @param type $includechild
   * @param array $fields The selected field list. Each element is a field name. Defaults to select all field(*) if $fields is empty.
   * @param array $fields_alias The selected field alias list. Each element key is a field name; each element value is a field alias.
   * @param array $conditions The filter conditions.
   *      Each element is still a array with two to three elements <b>array($field, $value[, $operator])</b>
   *      or an arbitrary WHERE clause to the query <b>array($snippet, $args, $where)</b>
   *      or nested conditionals <b>array('conjunction' => '$conjunction', 'conditions' => $conditions)</b>:
   *       - $field: The name of the field to check. If you would like to add a more complex condition involving operators or functions, use where().
   *       - $value: The value to test the field against. In most cases, this is a scalar. For more complex options, it is an array. The meaning of each element in the array is dependent on the $operator.
   *       - $operator: The comparison operator, such as =, !=, <, >, <=, or >=. It also accepts more complex options such as IN, NOT IN, LIKE, NOT LIKE, or BETWEEN. Defaults to IN if $value is an array, and = otherwise.
   *       - $snippet: A portion of a WHERE clause as a prepared statement. It must use named placeholders, not ? placeholders.
   *       - $args: An associative array of arguments.
   *       - $where: The unique value is WHERE.
   *       - $conjunction: The conjunctions include AND, OR, XOR.
   * @param array $orderbys Orders the result set by a given field.
   *      Each element is still a array with one to two elements($field[, $direction]):
   *       - $field: The field on which to order.
   *       - $direction: The direction to sort. Legal values are "ASC" or "DESC" or "RANDOM"($field is invalid), Default to "ASC".
   * @param type $limit The page limit or null.
   * @param boolean $format whether to format loaded data or not.
   * @param int $element An optional integer to distinguish between multiple pagers on one page.
   * @return array The filtered data array.
   */
  public static function FilterNodesByNavtag($navtag, $includechild = 0, $fields = array(), $fields_alias = array(), $conditions = array(), $orderbys = array(), $limit = null, $format = false, $element = 0){
    $Navigation = CMSNavigation::LoadNavigationByTag($navtag, true);
    if(!isset($Navigation->id)){ return array(); }
    if($includechild){
      $arrNavigations = CMSNavigation::LoadNavigations($Navigation->id, 1, false);
      $navides = CMSNavigation::LoadNavigationNavides($arrNavigations);
      $navides[] = $Navigation->id;
      $conditions[] = array('navid', $navides, 'IN');
    }else{
      $conditions[] = array('navid', $Navigation->id);
    }
    $arrDatas = MiniFieldData::FilterDatasByCid($Navigation->categoryid[0][0], $fields, $fields_alias, $conditions, $orderbys, $limit, $format, $element);
    return $arrDatas;
  }

  /**
   *
   * @access public
   * @static
   * @param int $navid
   * @param mixed $includechild Recursive listed in the child navigations.
   * @param type $limit The page limit or null.
   * @param bool $format
   * @param int $element An optional integer to distinguish between multiple pagers on one page.
   * @return type
   */
  public static function LoadNodesByNavid($navid, $includechild = 0, $limit = null, $format = false, $element = 0){
    $Navigation = CMSNavigation::LoadNavigationById($navid, true);
    if(!isset($Navigation->id)){ return array(); }
    $conditions = array();
    $conditions[] = array('status', 1);
    if($includechild){
      $arrNavigations = CMSNavigation::LoadNavigations($Navigation->id, 1, false);
      $navides = CMSNavigation::LoadNavigationNavides($arrNavigations);
      $navides[] = $Navigation->id;
      $conditions[] = array('navid', $navides, 'IN');
    }else{
      $conditions[] = array('navid', $Navigation->id);
    }
    $arrDatas = MiniFieldData::FilterDatasByCid($Navigation->categoryid[0][0], array(), array(), $conditions, array(array('created', 'DESC')), $limit, $format, $element);
    return $arrDatas;
  }
  
  /**
   *
   * @access public
   * @static
   * @param string/int $navtag The navigation id or urlalias.
   * @param type $includechild
   * @param type $limit The page limit or null.
   * @param bool $format
   * @param int $element An optional integer to distinguish between multiple pagers on one page.
   * @return type
   */
  public static function LoadNodesByNavtag($navtag, $includechild = 0, $limit = null, $format = false, $element = 0){
    $Navigation = CMSNavigation::LoadNavigationByTag($navtag, true);
    if(!isset($Navigation->id)){ return array(); }
    $conditions = array();
    $conditions[] = array('status', 1);
    if($includechild){
      $arrNavigations = CMSNavigation::LoadNavigations($Navigation->id, 1, false);
      $navides = CMSNavigation::LoadNavigationNavides($arrNavigations);
      $navides[] = $Navigation->id;
      $conditions[] = array('navid', $navides, 'IN');
    }else{
      $conditions[] = array('navid', $Navigation->id);
    }
    $arrDatas = MiniFieldData::FilterDatasByCid($Navigation->categoryid[0][0], array(), array(), $conditions, array(array('created', 'DESC')), $limit, $format, $element);
    return $arrDatas;
  }
  
  public static function LoadNodeByNavidId($navid, $id, $format = false){
    $Navigation = CMSNavigation::LoadNavigationById($navid, false);
    if(!isset($Navigation->id)){ return null; }
    $Data = MiniFieldData::LoadDataByCidId($Navigation->categoryid, $id, $format);
    if(isset($Data->navid) && $Data->navid != $Navigation->id){ return null; }
    return $Data;
  }
  
  /**
   *
   * @access public
   * @static
   * @param string/int $navtag The navigation id or urlalias.
   * @param int $id
   * @param bool $format
   * @return type
   */
  public static function LoadNodeByNavtagId($navtag, $id, $format = false){
    $Navigation = CMSNavigation::LoadNavigationByTag($navtag, false);
    if(!isset($Navigation->id)){ return null; }
    $Data = MiniFieldData::LoadDataByCidId($Navigation->categoryid, $id, $format);
    if(isset($Data->navid) && $Data->navid != $Navigation->id){ return null; }
    return $Data;
  }

  public static function ExistNodeByNavidId($navid, $id, $conditions = array()){
    if(is_array($conditions) && !empty($conditions)){
      $conditions[] = array('id', $id);
      $arrDatas = self::FilterNodesByNavid($navid, 0, array(), array(), $conditions, array(), 1, false);
      return isset($arrDatas[0])&&isset($arrDatas[0]->id) ? true : false;
    }else{
      $Data = self::LoadNodeByNavidId($navid, $id, false);
      return isset($Data->id) ? true : false;
    }
    return false;
    
  }

  /**
   *
   * @access public
   * @static
   * @param string/int $navtag The navigation id or urlalias.
   * @param int $id
   * @return type
   */
  public static function ExistNodeByNavtagId($navtag, $id, $conditions = array()){
    if(is_array($conditions) && !empty($conditions)){
      $conditions[] = array('id', $id);
      $arrDatas = self::FilterNodesByNavtag($navtag, 0, array(), array(), $conditions, array(), 1, false);
      return isset($arrDatas[0])&&isset($arrDatas[0]->id) ? true : false;
    }else{
      $Data = self::LoadNodeByNavtagId($navtag, $id, false);
      return isset($Data->id) ? true : false;
    }
    return false;
  }
  
  public static function DeleteNodeByNavidId($navid, $id){
    $Navigation = CMSNavigation::LoadNavigationById($navid, false);
    if(isset($Navigation->id) && is_numeric($Navigation->categoryid)){
      MiniFieldData::DeleteDatasByCid($Navigation->categoryid, array(array('id', $id), array('navid', $Navigation->id)), true);
    }
  }

  /**
   *
   * @access public
   * @static
   * @param string/int $navtag The navigation id or urlalias.
   * @param int $id
   * @return type
   */
  public static function DeleteNodeByNavtagId($navtag, $id){
    $Navigation = CMSNavigation::LoadNavigationByTag($navtag, false);
    if(isset($Navigation->id) && is_numeric($Navigation->categoryid)){
      MiniFieldData::DeleteDatasByCid($Navigation->categoryid, array(array('id', $id), array('navid', $Navigation->id)), true);
    }
  }

  /**
   *
   * @access public
   * @static
   * @param string/int $navtag The navigation id or urlalias.
   * @param array $ides Each element is a node id.
   */
  public static function DeleteNodes($navtag, $ides){
    $Navigation = CMSNavigation::LoadNavigationByTag($navtag, false);
    if(isset($Navigation->id) && is_numeric($Navigation->categoryid)){
      MiniFieldData::DeleteDatasByCid($Navigation->categoryid, array(array('id', $ides, 'IN'), array('navid', $Navigation->id)), true);
    }
  }
}