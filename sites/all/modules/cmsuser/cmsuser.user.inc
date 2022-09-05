<?php
/**
 * Available hooks:
 *  - hook_cmsuser_presave($user, &$edit, &$errors)
 *  - hook_cmsuser_save($user)
 *  - hook_cmsuser_delete($uides = array())
 *  - hook_cmsuser_register($user)
 *  - hook_cmsuser_prelogin($name, $pass, &$user, &$errors)
 *  - hook_cmsuser_login($user)
 * @author Changfeng Ji <jichf@qq.com>
 */
final class CMSUser{
  /**
   * Determines if the user is logged in.
   * @access public
   * @static
   * @param int $uid specifying the user ID to check.
   * @return boolean TRUE if the user is logged in, FALSE if the user is not logged in.
   */
  public static function IsLoggedIn($uid){
    $session_inc = variable_get('session_inc', 'includes/session.inc');
    if($session_inc == 'includes/session.inc'){
      return (bool)MiniFieldSql::LoadDataCountByTableField('sessions', 'uid', array(array('uid', $uid)), true);
    }else if(strrpos($session_inc, 'memcache') !== false && function_exists('dmemcache_get')){
      $user = dmemcache_get($uid, 'users');
      if(isset($user->sid) && dmemcache_get($user->sid, 'session')){
        return true;
      }
    }
    return false;
  }
  /**
   * Determines if the user is online.
   * @access public
   * @static
   * @param int $uid specifying the user ID to check.
   * @return boolean TRUE if the user is online, FALSE if the user is offline.
   */
  public static function IsOnline($uid){
    if(!self::IsLoggedIn($uid)){
      return false;
    }
    $access = MiniFieldSql::FilterDatasByTable('users', array('access'), array(), array(array('uid', $uid)), array(), null, 0, array('fetchField'));
    if($access && time() - $access < variable_get('session_write_interval', 180)){
      return true;
    }
    return false;
  }

  /**
   * 
   * @access public
   * @static
   * @param int $uid The user ID.
   * @return array An array, each element is a session id.
   */
  public static function LoadSessionSidsByUid($uid){
    $session_inc = variable_get('session_inc', 'includes/session.inc');
    if($session_inc == 'includes/session.inc'){
      return MiniFieldSql::LoadDataColByTableField('sessions', 'sid', array(array('uid', $uid)), array(array('timestamp', 'DESC')));
    }else if(strrpos($session_inc, 'memcache') !== false && function_exists('dmemcache_get')){
      $user = dmemcache_get($uid, 'users');
      if(isset($user->sid) && dmemcache_get($user->sid, 'session')){
        return array($user->sid);
      }
    }
    return array();
  }
  /**
   * 
   * @access public
   * @static
   * @param string $sid The session id.
   */
  public static function DeleteSessionBySid($sid){
    $session_inc = variable_get('session_inc', 'includes/session.inc');
    if($session_inc == 'includes/session.inc'){
      MiniFieldSql::DeleteDatasByTable('sessions', array(array('sid', $sid)));
    }else if(strrpos($session_inc, 'memcache') !== false && function_exists('dmemcache_delete')){
      dmemcache_delete($sid, 'session');
    }
  }
  /**
   * 
   * @access public
   * @static
   * @param string $uid The user ID.
   */
  public static function DeleteSessionsByUid($uid){
    $old_state = drupal_save_session();
    drupal_save_session(true);
    drupal_session_destroy_uid($uid);
    drupal_save_session($old_state);
  }

  /**
   * 
   * @access public
   * @static
   * @param string $name The user name.
   * @return int The user id, or FALSE if not found.
   */
  public static function LoadUserUidByName($name){
    return MiniFieldSql::FilterDatasByTable('users', array('uid'), array(), array(array('name', $name)), array(), null, 0, array('fetchField'));
  }
  /**
   * 
   * @access public
   * @static
   * @param string $mail The user mail.
   * @return int The user id, or FALSE if not found.
   */
  public static function LoadUserUidByMail($mail){
    return MiniFieldSql::FilterDatasByTable('users', array('uid'), array(), array(array('mail', $mail)), array(), null, 0, array('fetchField'));
  }
  /**
   * 
   * @access public
   * @static
   * @param string $mobile The user mobile phone number.
   * @return int The user id, or FALSE if not found.
   */
  public static function LoadUserUidByMobile($mobile){
    return MiniFieldSql::FilterDatasByTable('users', array('uid'), array(), array(array('mobile', $mobile)), array(), null, 0, array('fetchField'));
  }
  /**
   * 
   * @access public
   * @static
   * @param string $sid The session id.
   * @return int The user id, or FALSE if not found.
   */
  public static function LoadUserUidBySid($sid){
    $session_inc = variable_get('session_inc', 'includes/session.inc');
    if($session_inc == 'includes/session.inc'){
      return MiniFieldSql::FilterDatasByTable('sessions', array('uid'), array(), array(array('sid', $sid)), array(), null, 0, array('fetchField'));
    }else if(strrpos($session_inc, 'memcache') !== false && function_exists('dmemcache_get')){
      $session = dmemcache_get($sid, 'session');
      if(isset($session->uid)){
        return $session->uid;
      }
    }
    return false;
  }
  /**
   * 
   * @access public
   * @static
   * @param string $mail The user mail.
   * @return string The user name, or FALSE if not found.
   */
  public static function LoadUserNameByMail($mail){
    return MiniFieldSql::FilterDatasByTable('users', array('name'), array(), array(array('mail', $mail)), array(), null, 0, array('fetchField'));
  }
  /**
   * 
   * @access public
   * @static
   * @param string $mobile The user mobile phone number.
   * @return string The user name, or FALSE if not found.
   */
  public static function LoadUserNameByMobile($mobile){
    return MiniFieldSql::FilterDatasByTable('users', array('name'), array(), array(array('mobile', $mobile)), array(), null, 0, array('fetchField'));
  }

  /**
   *
   * @access public
   * @static
   * @param array $rides Each element is a role id. Indicates that all role ides if empty.
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
   * @return array An array, each element is a user id.
   */
  public static function LoadUserUidesByRides($rides = array(), $conditions = array()){
    $tables = array(
      array('users', '', 'uid', array('uid'), array()),
      array('users_roles', 'INNER', 'uid', array('rid'), array())
    );
    if(is_array($rides) && !empty($rides)){
      $conditions[] = array('rid', $rides, 'IN');
    }
    $orderbys = array();
    $orderbys[] = array('uid', 'ASC');
    return MiniFieldSql::FilterDatasByTables($tables, $conditions, $orderbys, null, 0, array('fetchCol'));
  }
  /**
   *
   * @access public
   * @static
   * @param array $rides Each element is a role id. Indicates that all role ides if empty.
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
   * @return int The user count.
   */
  public static function LoadUserCountByRides($rides = array(), $conditions = array()){
    $tables = array(
      array('users', '', 'uid', array('uid'), array()),
      array('users_roles', 'INNER', 'uid', array('rid'), array())
    );
    if(is_array($rides) && !empty($rides)){
      $conditions[] = array('rid', $rides, 'IN');
    }
    $orderbys = array();
    $orderbys[] = array('uid', 'ASC');
    return MiniFieldSql::FilterDatasByTables($tables, $conditions, $orderbys, null, 0, array('rowCount'));
  }

  /**
   *
   * @access public
   * @static
   * @param array $rides Each element is a role id. Indicates that all role ides if empty.
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
   * @return array An array, each element is a user id.
   */
  public static function LoadUserUidesOnlineByRides($rides = array(), $conditions = array()){
    $tables = array(
      array('users', '', 'uid', array('uid'), array()),
      array('users_roles', 'INNER', 'uid', array('rid'), array())
    );
    if(is_array($rides) && !empty($rides)){
      $conditions[] = array('rid', $rides, 'IN');
    }
    $conditions[] = array('access', time() - variable_get('session_write_interval', 180), '>');
    $orderbys = array();
    $orderbys[] = array('uid', 'ASC');
    return MiniFieldSql::FilterDatasByTables($tables, $conditions, $orderbys, null, 0, array('fetchCol'));
  }
  /**
   *
   * @access public
   * @static
   * @param array $rides Each element is a role id. Indicates that all role ides if empty.
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
   * @return int The user count online.
   */
  public static function LoadUserCountOnlineByRides($rides = array(), $conditions = array()){
    $tables = array(
      array('users', '', 'uid', array('uid'), array()),
      array('users_roles', 'INNER', 'uid', array('rid'), array())
    );
    if(is_array($rides) && !empty($rides)){
      $conditions[] = array('rid', $rides, 'IN');
    }
    $conditions[] = array('access', time() - variable_get('session_write_interval', 180), '>');
    $orderbys = array();
    $orderbys[] = array('uid', 'ASC');
    return MiniFieldSql::FilterDatasByTables($tables, $conditions, $orderbys, null, 0, array('rowCount'));
  }

  /**
   *
   * @access public
   * @static
   * @param int $rid role id.
   */
  public static function ClearUsersByRid($rid){
    if($rid < 3){
      return;
    }
    $RoleCid = CMSRole::GetRoleCategoryIdByRid($rid);
    if(!is_numeric($RoleCid) || $RoleCid == 0){ return; }
    $uids = self::LoadUserUidesByRides(array($rid));
    if(empty($uids)){ return; }
    MiniFieldData::DeleteDatasByCid($RoleCid, array(array('userid', $uids, 'NOT IN')), true);
    foreach($uids as $uid){
      $ides = MiniFieldData::LoadDataIdesByCid($RoleCid, array(array('userid', $uid)));
      if(!isset($ides[0])){
        $_POST = array('userid' => $uid);
        $mfd = new MiniFieldData($RoleCid, 0);
        $mfd->SaveDataToDB();
      }else if(isset($ides[1])){
        unset($ides[0]);
        MiniFieldData::DeleteDatasByCid($RoleCid, array(array('id', $ides, 'IN')), true);
      }
    }
    MiniFieldStatic::ClearStatics(__CLASS__ . '_LoadUsersByRid');
    MiniFieldStatic::ClearStatics(__CLASS__ . '_LoadUserByUid');
    MiniFieldStatic::ClearStatics(__CLASS__ . '_LoadUserByRidUid');
  }
  /**
   *
   * @access public
   * @static
   * @param int $rid role id.
   * @param int $uid user id.
   */
  public static function ClearUserByRidUid($rid, $uid){
    if($rid < 3){
      return;
    }
    $RoleCid = CMSRole::GetRoleCategoryIdByRid($rid);
    if(!is_numeric($RoleCid) || $RoleCid == 0){ return; }
    $ides = MiniFieldData::LoadDataIdesByCid($RoleCid, array(array('userid', $uid)));
    if(!isset($ides[0])){
      $_POST = array('userid' => $uid);
      $mfd = new MiniFieldData($RoleCid, 0);
      $mfd->SaveDataToDB();
    }else if(isset($ides[1])){
      unset($ides[0]);
      MiniFieldData::DeleteDatasByCid($RoleCid, array(array('id', $ides, 'IN')), true);
    }
    MiniFieldStatic::ClearStatics(__CLASS__ . '_LoadUsersByRid');
    MiniFieldStatic::ClearStatics(__CLASS__ . '_LoadUserByUid_' . $uid);
    MiniFieldStatic::ClearStatics(__CLASS__ . '_LoadUserByRidUid_' . $uid);
  }

  /**
   *
   * @access public
   * @static
   * @param int $rid role id.
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
   * @param bool $format
   * @param int $element An optional integer to distinguish between multiple pagers on one page.
   * @param string $callback Perform necessary alterations to the <b>SelectQuery</b> before it is executed.
   *  - Possible value: a function string, or a class-level static function string(the class string and function string are separated by '|').
   *  - Function argument: A SelectQuery object for this connection.
   *  - Function expected return value: A new SelectQuery object for this connection.
   *  - Function example: function test($queryObj){ return $queryObj; }
   * @return type
   */
  public static function FilterUsersByRid($rid, $conditions = array(), $orderbys = array(), $limit = null, $format = false, $element = 0, $callback = ''){
    if(!is_numeric($rid)){ return array(); }
    $RoleCid = CMSRole::GetRoleCategoryIdByRid($rid);
    if(!is_numeric($RoleCid) || $RoleCid == 0){ return array(); }
    $tablename = minifield_tablename_load($RoleCid);
    if(empty($tablename)){ return array(); }
    $tables = array(
      array('users', '', 'uid', array(), array()),
      array('users_roles', 'INNER', 'uid', array('rid'), array()),
      array($tablename, 'INNER', 'userid', array(), array())
    );
    $conditions[] = array('rid', $rid);
    //$conditions[] = array('users.uid', 1, '>');//exclude drupal administrator
    $arrDatas = MiniFieldSql::FilterDatasByTables($tables, $conditions, $orderbys, $limit, $element, array('fetchAll'), $callback);
    if($format == true){
      $arrFields = MiniField::LoadDatasByCid($RoleCid, null, false);
      $arrDatas = MiniFieldData::FormatDatas($arrDatas, $arrFields);
    }
    foreach($arrDatas as $key => $Data){
      $arrDatas[$key]->data = unserialize($Data->data);
      $arrDatas[$key]->roles = array();
      if($Data->uid){
        $arrDatas[$key]->roles[DRUPAL_AUTHENTICATED_RID] = 'authenticated user';
      }else{
        $arrDatas[$key]->roles[DRUPAL_ANONYMOUS_RID] = 'anonymous user';
      }
      if($Data->rid){
        $arrDatas[$key]->roles[$Data->rid] = CMSRole::GetRoleNameByRid($Data->rid);
      }
      //$account = user_load($Data->uid);
      //$arrDatas[$key] = (object)array_merge((array)$arrDatas[$key], (array)$account);
    }
    return $arrDatas;
  }

  /**
   *
   * @access public
   * @static
   * @param int $rid role id.
   * @param type $limit The page limit or null.
   * @param bool $format
   * @param int $element An optional integer to distinguish between multiple pagers on one page.
   * @return type
   */
  public static function LoadUsersByRid($rid, $limit = null, $format = false, $element = 0){
    $StaticName = __CLASS__ . '_' . __FUNCTION__ . '_' . $rid . '_' . ($format ? 1 : 0);
    if(is_null($limit)){
      $arrDatas = MiniFieldStatic::GetStatic($StaticName);
      if(isset($arrDatas)){
        return $arrDatas;
      }
    }
    $arrDatas = self::FilterUsersByRid($rid, array(array('status', 1)), array(), $limit, $format, $element);
    if(is_null($limit)){
      MiniFieldStatic::SetStatic($StaticName, $arrDatas);
    }
    return $arrDatas;
  }

  /**
   *
   * @access public
   * @static
   * @param int $rid role id.
   * @param int $uid user id.
   * @param bool $format
   * @param boolean $reset to reset the internal cache and load from the database; FALSE (default) to load from the internal cache, if set.
   * @return object
   */
  public static function LoadUserByRidUid($rid, $uid, $format = false, $reset = false){
    if(!is_numeric($rid) || !is_numeric($uid)){ return null; }
    $StaticName = __CLASS__ . '_' . __FUNCTION__ . '_' . $uid . '_' . ($format ? 1 : 0);
    $Data = MiniFieldStatic::GetStatic($StaticName);
    if($reset){
      $Data = MiniFieldStatic::ClearStatic($StaticName);
    }
    if(isset($Data)){
      return $Data;
    }
    $RoleCid = CMSRole::GetRoleCategoryIdByRid($rid);
    if(!is_numeric($RoleCid) || $RoleCid == 0){ return null; }
    $tablename = minifield_tablename_load($RoleCid);
    if(empty($tablename)){ return null; }
    $tables = array(
      array('users', '', 'uid', array(), array()),
      array('users_roles', 'INNER', 'uid', array('rid'), array()),
      array($tablename, 'INNER', 'userid', array(), array())
    );
    $conditions = array();
    $conditions[] = array('rid', $rid);
    $conditions[] = array('users.uid', $uid);
    $Data = MiniFieldSql::FilterDatasByTables($tables, $conditions, array(), null, 0, array('fetchObject'));
    if(isset($Data->uid) && is_numeric($Data->uid)){
      if($format == true){
        $arrFields = MiniField::LoadDatasByCid($RoleCid, null, false);
        $Data = MiniFieldData::FormatData($Data, $arrFields);
      }
      $Data->data = unserialize($Data->data);
      $Data->roles = array();
      if($Data->uid){
        $Data->roles[DRUPAL_AUTHENTICATED_RID] = 'authenticated user';
      }else{
        $Data->roles[DRUPAL_ANONYMOUS_RID] = 'anonymous user';
      }
      if($Data->rid){
        $Data->roles[$Data->rid] = CMSRole::GetRoleNameByRid($Data->rid);
      }
      //$account = user_load($Data->uid);
      //$Data = (object)array_merge((array)$Data, (array)$account);
      MiniFieldStatic::SetStatic($StaticName, $Data);
    }
    return $Data;
  }

  /**
   *
   * @access public
   * @static
   * @param int $uid user id.
   * @param bool $format
   * @param boolean $reset to reset the internal cache and load from the database; FALSE (default) to load from the internal cache, if set.
   * @return object
   */
  public static function LoadUserByUid($uid, $format = false, $reset = false){
    if(!is_numeric($uid)){ return null; }
    $StaticName = __CLASS__ . '_' . __FUNCTION__ . '_' . $uid;
    $rid = MiniFieldStatic::GetStatic($StaticName);
    if($reset){
      $rid = MiniFieldStatic::ClearStatic($StaticName);
    }
    if(isset($rid)){
      return self::LoadUserByRidUid($rid, $uid, $format, $reset);
    }
    //A user has one role only
    $rid = MiniFieldSql::FilterDatasByTable('users_roles', array('rid'), array(), array(array('uid', $uid)), array(), null, 0, array('fetchField'));
    if($rid){
      MiniFieldStatic::SetStatic($StaticName, $rid);
      return self::LoadUserByRidUid($rid, $uid, $format, $reset);
    }
    return null;
  }

  public static function ExistUserByRidUid($rid, $uid){
    $Data = self::LoadUserByRidUid($rid, $uid);
    return isset($Data->uid) ? true : false;
  }
  public static function ExistUserByUid($uid){
    return MiniFieldSql::ExistDatasByTable('users', array(array('uid', $uid)));
  }
  public static function ExistUserByName($name){
    return MiniFieldSql::ExistDatasByTable('users', array(array('name', $name)));
  }
  public static function ExistUserByMail($mail){
    return MiniFieldSql::ExistDatasByTable('users', array(array('mail', $mail)));
  }
  public static function ExistUserByMobile($mobile){
    return MiniFieldSql::ExistDatasByTable('users', array(array('mobile', $mobile)));
  }

  public static function DeleteUserByRidUid($rid, $uid){
    $usercount = self::LoadUserCountByRides(array($rid), array(array('users.uid', $uid)));
    if($usercount <= 0){ return; }
    module_invoke_all('cmsuser_delete', array($uid));
    $RoleCid = CMSRole::GetRoleCategoryIdByRid($rid);
    if(is_numeric($RoleCid) && $RoleCid > 0){
      MiniFieldData::DeleteDatasByCid($RoleCid, array(array('userid', $uid)), true);
    }
    user_delete($uid);
  }

  public static function DeleteUsersByRidUides($rid, $uides){
    $uides = self::LoadUserUidesByRides(array($rid), array(array('users.uid', $uides, 'IN')));
    if(empty($uides)){ return; }
    module_invoke_all('cmsuser_delete', $uides);
    $RoleCid = CMSRole::GetRoleCategoryIdByRid($rid);
    if(is_numeric($RoleCid) && $RoleCid > 0){
      MiniFieldData::DeleteDatasByCid($RoleCid, array(array('userid', $uides, 'IN')), true);
    }
    user_delete_multiple($uides);
  }

  /**
   *
   * @access public
   * @static
   * @param int $uid user id.
   * @param MiniFieldData $mfd defaults to null.
   * @param string $hook_presave Whether to invoke the hook of 'cmsuser_presave', defaults to TRUE.
   * @param string $hook_save Whether to invoke the hook of 'cmsuser_save', defaults to TRUE.
   * @return int/array :
   *   - The last insert/update ID of the query, if successfully.
   *   - An associative array of reasons for failure, where the keys are the field name and the values are the values those field name must have, if failed.
   */
  public static function SaveUser($uid, MiniFieldData $mfd = null, $hook_presave = true, $hook_save = true){
    $account = user_load($uid);
    if(!isset($account->uid) || !$account->uid){
      $account = (object)array('uid' => 0, 'name' => '', 'pass' => '', 'mail' => '', 'mobile' => '', 'status' => 0, 'roles' => array());
    }
    $edit = array('name' => '', 'pass' => '', 'mail' => '', 'mobile' => '', 'status' => 0, 'roles' => array());
    if(isset($_POST['name'])){
      $edit['name'] = trim($_POST['name']);
    }else if(isset($account->name)){
      $edit['name'] = $account->name;
    }
    if(isset($_POST['pass'])){
      $edit['pass'] = $_POST['pass'];
    }
    if(isset($_POST['mail'])){
      $edit['mail'] = trim($_POST['mail']);
    }else if(isset($account->mail)){
      $edit['mail'] = $account->mail;
    }
    if(isset($_POST['mobile'])){
      $edit['mobile'] = trim($_POST['mobile']);
    }else if(isset($account->mobile)){
      $edit['mobile'] = $account->mobile;
    }
    if(isset($_POST['status'])){
      $edit['status'] = $_POST['status'];
    }else if(isset($account->status)){
      $edit['status'] = $account->status;
    }
    if(isset($_POST['roles']) && is_numeric($_POST['roles'])){
      $edit['roles'][$_POST['roles']] = $_POST['roles'];
    }else if(isset($_POST['roles']) && is_array($_POST['roles'])){
      foreach($_POST['roles'] as $rid){
        $edit['roles'][$rid] = $rid;
      }
    }else if(isset($account->roles)){
      $edit['roles'] = $account->roles;
    }
    $errors = array();
    if(empty($edit['name'])){
      $errors['name'] = '用户名必须填写';
    }else if($edit['name'] != $account->name && self::ExistUserByName($edit['name'])){
      $errors['name'] = '用户名 '.$edit['name'].' 已被占用';
    }
    if(isset($account->uid) && $account->uid > 0){
      if(preg_match('/^\s*$/', $edit['pass']) > 0){
        unset($edit['pass']);
      }
    }else if(preg_match('/^\s*$/', $edit['pass']) > 0){
      $errors['pass'] = '密码必须填写';
    }
    if(empty($edit['mail'])){
      //$errors['mail'] = '邮箱必须填写';
    }else if((bool)filter_var($edit['mail'], FILTER_VALIDATE_EMAIL) == false){
      $errors['mail'] = '邮箱 '.$edit['mail'].' 格式错误';
    }else if($edit['mail'] != $account->mail && self::ExistUserByMail($edit['mail'])){
      $errors['mail'] = '邮箱 '.$edit['mail'].' 已被占用';
    }
    if(empty($edit['mobile'])){
      //$errors['mobile'] = '手机号必须填写';
    }else if(!preg_match('/^1\d{10}$/', $edit['mobile'])){
      $errors['mobile'] = '手机号 '.$edit['mobile'].' 格式错误';
    }else if($edit['mobile'] != $account->mobile && self::ExistUserByMobile($edit['mobile'])){
      $errors['mobile'] = '手机号 '.$edit['mobile'].' 已被占用';
    }
    if(empty($edit['mail']) && empty($edit['mobile'])){
      $errors['mail'] = isset($_POST['mobile']) ? '邮箱和手机号至少填写一项' : '邮箱必须填写';
      $errors['mobile'] = isset($_POST['mail']) ? '邮箱和手机号至少填写一项' : '手机号必须填写';
    }
    if(!in_array($edit['status'], array(0, 1, '0', '1'))){
      $errors['status'] = '状态必须选择';
    }
    if(isset($mfd->FieldModel['userid'])){
      $errors = array_merge($errors, $mfd->ProcessPostData());
      if(isset($errors['userid'])){ unset($errors['userid']); }
    }
    if(!isset($account->uid) || !$account->uid){
      $account = null;
      if(is_numeric($uid) && $uid > 0){
        $edit['uid'] = $uid;
      }
    }
    if($hook_presave){
      foreach(module_implements('cmsuser_presave') as $module){
        $function = $module . '_cmsuser_presave';
        $function($account, $edit, $errors);
      }
    }
    if(!empty($errors)){
      return $errors;
    }
    $account = user_save($account, $edit);
    if($account && isset($mfd->FieldModel['userid'])){
      $_POST['userid'] = $account->uid;
      $mfd->FieldModel['userid'] = $_POST['userid'];
      $mfd->SaveDataToDB();
    }else if($account && !empty($account->roles)){
      $rids = array_keys($account->roles);
      foreach($rids as $rid){
        self::ClearUserByRidUid($rid, $account->uid);
      }
    }
    if($account){
      MiniFieldStatic::ClearStatics(__CLASS__ . '_LoadUsersByRid');
      MiniFieldStatic::ClearStatics(__CLASS__ . '_LoadUserByUid_' . $account->uid);
      MiniFieldStatic::ClearStatics(__CLASS__ . '_LoadUserByRidUid_' . $account->uid);
      if($hook_save){
        $user = self::LoadUserByUid($account->uid, true);
        module_invoke_all('cmsuser_save', $user);
      }
      return $account->uid;
    }
    return $errors;
  }

  /**
   *
   * @access public
   * @static
   * @param object/int $user The user object or user id.
   * @param string $hook_login Whether to invoke the hook of 'cmsuser_login', defaults to TRUE.
   * @return bool <b>TRUE</b> If successful login, <b>FALSE</b> otherwise.
   */
  public static function Login($user, $hook_login = true){
    $account = null;
    if (isset($user->uid)) {
      $account = $user;
    }else if(is_numeric($user)){
      $account = user_load($user);
    }
    if(!isset($account->uid) || !$account->uid){
      return false;
    }
    global $user;
    $user = $account;
    watchdog('user', 'Session opened for %name.', array('%name' => $user->name));
    // Update the user table timestamp noting user has logged in.
    // This is also used to invalidate one-time login links.
    $user->login = REQUEST_TIME;
    db_update('users')
      ->fields(array('login' => $user->login))
      ->condition('uid', $user->uid)
      ->execute();
    drupal_session_regenerate();
    if($hook_login){
      module_invoke_all('cmsuser_login', $user);
    }
    return true;
  }
  /**
   *
   * @access public
   * @static
   * @param string $name The user name. In addition, mail or mobile also can login in.
   * @param string $pass The user pass.
   * @param string $hook_prelogin Whether to invoke the hook of 'cmsuser_prelogin', defaults to TRUE.
   * @param string $hook_login Whether to invoke the hook of 'cmsuser_login', defaults to TRUE.
   * @return object/array :
   *   - The logged in user object, if successfully.
   *   - An associative array of reasons for failure, where the keys are the field name and the values are the values those field name must have, if failed.
   */
  public static function LoginUser($name, $pass, $hook_prelogin = true, $hook_login = true){
    $errors = array();
    if(empty($name)){
      $errors['name'] = '账户不能为空';
    }
    if(empty($pass)){
      $errors['pass'] = '密码不能为空';
    }
    if(!empty($errors)){
      return $errors;
    }
    $account = null;
    if(self::ExistUserByName($name)){
      $account = user_load_by_name($name);
    }else if((bool)filter_var($name, FILTER_VALIDATE_EMAIL)){
      $account = user_load_by_mail($name);
    }else if(preg_match('/^1\d{10}$/', $name) && ($uid = self::LoadUserUidByMobile($name))){
      $account = user_load($uid);
    }
    if(!isset($account->uid) || !$account->uid){
      $errors['name'] = '账户不存在';
    }else if($account->status == 0){
      $errors['name'] = '账户已经被锁定';
    }else{
      // Allow alternate password hashing schemes.
      require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
      if (!user_check_password($pass, $account)) {
        $errors['name'] = '账户或密码错误';
      }
    }
    if($hook_prelogin){
      foreach(module_implements('cmsuser_prelogin') as $module){
        $function = $module . '_cmsuser_prelogin';
        $function($name, $pass, $account, $errors);
      }
    }
    if(!empty($errors)){
      return $errors;
    }
    if(self::Login($account, $hook_login)){
      return $account;
    }else{
      $errors['name'] = '账户不存在';
    }
    return $errors;
  }

  /**
   *
   * @access public
   * @static
   * @param bool $autologin Defaults to <b>TRUE</b>.
   * @return object/array :
   *   - The last insert user object, if successfully.
   *   - An associative array of reasons for failure, where the keys are the field name and the values are the values those field name must have, if failed.
   */
  public static function RegisterUser($autologin = true){
    $rid = (isset($_POST['roles']) && is_numeric($_POST['roles'])) ? $_POST['roles'] : 0;
    $rid = (preg_match('/^\d+$/', $rid) == 0) ? 0 : $rid;
    if($rid == 0){ return array('name' => '禁止注册用户'); }
    $Role = CMSRole::LoadRoleByRid($rid, false);
    if(!in_array($Role->allowregister, array(1, 2))){ return array('name' => '禁止注册用户'); }
    if(!isset($Role->categoryid) || !is_numeric($Role->categoryid) || $Role->categoryid <= 0){ return array('name' => '禁止注册用户'); }
    $tablename = minifield_tablename_load($Role->categoryid);
    if(empty($tablename)){ return array('name' => '禁止注册用户'); }
    $_POST['status'] = $Role->allowregister == 2 ? 1 : 0;
    $_POST['roles'] = $rid;
    $_POST['userid'] = 0;
    $_POST['id'] = 0;
    $mfd = new MiniFieldData($Role->categoryid);
    $result = self::SaveUser(0, $mfd);
    if(is_numeric($result)){
      $result = self::LoadUserByUid($result, true);
      module_invoke_all('cmsuser_register', $result);
      if($result->status == 1 && $autologin){
        self::Login($result);
      }
    }
    return $result;
  }

  /**
   * 
   * @access public
   * @static
   * @param int $uid user id.
   * @param int $officialname official name.
   * @return array A array with two elements('status' => '$status', 'text' => '$text'):
   *  - $status: The status value 'error' or 'success'.
   *  - $text: The status text.
   */
  public static function ActivateUserApply($uid, $officialname){
    $account = user_load($uid);
    if(!$account){
      return array('status' => 'error', 'text' => '用户不存在');
    }else if($account->status == 1){
      return array('status' => 'error', 'text' => '无效访问，此账户已经激活');
    }else if((bool)filter_var($account->mail, FILTER_VALIDATE_EMAIL) == false){
      return array('status' => 'error', 'text' => '无效的邮箱，不能激活账户');
    }else{
      $edit = array(
        'data' => array(
          'activatecode' => MiniFieldFile::getRandomString(10),
          'activatetime' => time()
        )
      );
      $account = user_save($account, $edit);
      require_once MINIFIELD_MODULE_PATH . '/minifield.mail.inc';
      $mail = MiniFieldMail::GetMailInstance();
      MiniFieldMail::ProcessParams($mail, array(
        'Mailer' => 'smtp',
        'SMTPAuth' => true,
        'Port' => 25,
        'Host' => 'smtp.163.com',
        'Username' => 'ccmsmail@163.com',
        'Password' => 'ccmsmail.',
        'SetFrom' => array('ccmsmail@163.com' => $officialname),
        'AddAddress' => array($account->mail => $account->name),
        'Subject' => $officialname . ' - 激活账户',
        'AltBody' => '',
        'Body' => '<h1>激活账户</h1>
                  <p>' . $account->name . '，<br>
                  这封信是由 ' . $officialname . ' 发送的。</p>
                  <p>您收到这封邮件，是由于在' . $officialname . '进行了新用户注册使用了这个邮箱地址。如果您并没有访问过' . $officialname . '，或没有进行上述操作，请忽略这封邮件。您不需要退订或进行其他进一步的操作。</p>
                  ----------------------------------------------------------------------<br>
                  <strong>激活账户说明</strong><br>
                  ----------------------------------------------------------------------<br>
                  <p>如果您是 ' . $officialname . ' 的新用户，我们需要对您的地址有效性进行验证以避免垃圾邮件或地址被滥用。</p>
                  <p>激活账户验证码：<b>' . $account->data['activatecode'] . '</b> (有效期：3天)<br>
                  <br>
                  <p>感谢您的访问，祝您使用愉快！</p>
                  <p>
                  此致<br>
                  ' . $officialname . '.<br>
                  <a href="' . $GLOBALS['base_url'] . '" target="_blank">' . $GLOBALS['base_url'] . '</a></p>
                  ',
        'Attachments' => array(),
        'ContentType' => true,
        'CharSet' => 'utf-8'
      ));
      MiniFieldMail::SendAdvanced($mail);
      return array('status' => 'success', 'text' => '激活账户验证码已发出(有效期：3天)');
    }
  }

  /**
   * 
   * @access public
   * @static
   * @param int $uid user id.
   * @param string $activatecode activate apply random code.
   * @return array A array with two elements('status' => '$status', 'text' => '$text'):
   *  - $status: The status value 'error' or 'success'.
   *  - $text: The status text.
   */
  public static function ActivateUserValidate($uid, $activatecode){
    $account = user_load($uid);
    if(!$account){
      return array('status' => 'error', 'text' => '用户不存在');
    }else if($account->status == 1){
      return array('status' => 'error', 'text' => '无效访问，此账户已经激活');
    }else if(!isset($account->data['activatecode']) || !isset($account->data['activatetime'])){
      return array('status' => 'error', 'text' => '请先获取验证码');
    }else if(empty($account->data['activatecode']) || empty($account->data['activatetime'])){
      return array('status' => 'error', 'text' => '请先获取验证码');
    }else if($account->data['activatecode'] != $activatecode){
      return array('status' => 'error', 'text' => '验证码匹配失败');
    }else if(time() - $account->data['activatetime'] >  3 * 24 * 3600){
      return array('status' => 'error', 'text' => '验证码已过期，请重新获取');
    }else{
      $account = user_save($account, array('status' => 1, 'data' => array('activatecode' => '', 'activatetime' => '')));
      if($account){
        return array('status' => 'success', 'text' => '激活成功');
      }else{
        return array('status' => 'error', 'text' => '激活失败');
      }
    }
  }

  /**
   * 
   * @access public
   * @static
   * @param int $uid user id.
   * @param int $officialname official name.
   * @return array A array with two elements('status' => '$status', 'text' => '$text'):
   *  - $status: The status value 'error' or 'success'.
   *  - $text: The status text.
   */
  public static function ResetPassApply($uid, $officialname){
    $account = user_load($uid);
    if(!$account){
      return array('status' => 'error', 'text' => '用户不存在');
    }else if($account->status != 1){
      return array('status' => 'error', 'text' => '无效访问，此为被限制的用户');
    }else if((bool)filter_var($account->mail, FILTER_VALIDATE_EMAIL) == false){
      return array('status' => 'error', 'text' => '无效的邮箱，不能重置密码');
    }else{
      $edit = array(
        'data' => array(
          'resetpasscode' => MiniFieldFile::getRandomString(10),
          'resetpasstime' => time()
        )
      );
      $account = user_save($account, $edit);
      require_once MINIFIELD_MODULE_PATH . '/minifield.mail.inc';
      $mail = MiniFieldMail::GetMailInstance();
      MiniFieldMail::ProcessParams($mail, array(
        'Mailer' => 'smtp',
        'SMTPAuth' => true,
        'Port' => 25,
        'Host' => 'smtp.163.com',
        'Username' => 'ccmsmail@163.com',
        'Password' => 'ccmsmail.',
        'SetFrom' => array('ccmsmail@163.com' => $officialname),
        'AddAddress' => array($account->mail => $account->name),
        'Subject' => $officialname . ' - 重置密码',
        'AltBody' => '',
        'Body' => '<h1>重置密码</h1>
                  <p>' . $account->name . '，<br>
                  这封信是由 ' . $officialname . ' 发送的。</p>
                  <p>您收到这封邮件，是由于在' . $officialname . '的账户使用了这个邮箱地址。如果您并没有访问过' . $officialname . '，或没有进行上述操作，请忽略这封邮件。您不需要退订或进行其他进一步的操作。</p>
                  ----------------------------------------------------------------------<br>
                  <strong>重置密码说明</strong><br>
                  ----------------------------------------------------------------------<br>
                  <p>如果您忘记了在 ' . $officialname . ' 的账户密码，可以通过验证邮箱找回密码。</p>
                  <p>重置密码验证码：<b>' . $account->data['resetpasscode'] . '</b> (有效期：10分钟)<br>
                  <br>
                  <p>感谢您的访问，祝您使用愉快！</p>
                  <p>
                  此致<br>
                  ' . $officialname . '.<br>
                  <a href="' . $GLOBALS['base_url'] . '" target="_blank">' . $GLOBALS['base_url'] . '</a></p>
                  ',
        'Attachments' => array(),
        'ContentType' => true,
        'CharSet' => 'utf-8'
      ));
      MiniFieldMail::SendAdvanced($mail);
      return array('status' => 'success', 'text' => '重置密码验证码已发出(有效期：10分钟)');
    }
  }

  /**
   * 
   * @access public
   * @static
   * @param int $uid user id.
   * @param string $resetpasscode resetpass apply random code.
   * @param string $pass new password.
   * @return array A array with two elements('status' => '$status', 'text' => '$text'):
   *  - $status: The status value 'error' or 'success'.
   *  - $text: The status text.
   */
  public static function ResetPassValidate($uid, $resetpasscode, $pass){
    $account = user_load($uid);
    if(!$account){
      return array('status' => 'error', 'text' => '用户不存在');
    }else if($account->status != 1){
      return array('status' => 'error', 'text' => '无效访问，此为被限制的用户');
    }else if(!isset($account->data['resetpasscode']) || !isset($account->data['resetpasstime'])){
      return array('status' => 'error', 'text' => '请先获取验证码');
    }else if(empty($account->data['resetpasscode']) || empty($account->data['resetpasstime'])){
      return array('status' => 'error', 'text' => '请先获取验证码');
    }else if($account->data['resetpasscode'] != $resetpasscode){
      return array('status' => 'error', 'text' => '验证码匹配失败');
    }else if(time() - $account->data['resetpasstime'] >  10 * 60){
      return array('status' => 'error', 'text' => '验证码已过期，请重新获取');
    }else if(preg_match('/^\s*$/', $pass) > 0){
      return array('status' => 'error', 'text' => '新密码必须填写');
    }else if($pass == $account->name){
      return array('status' => 'error', 'text' => '密码和用户名不能完全相同');
    }else{
      $account = user_save($account, array('pass' => $pass, 'data' => array('resetpasscode' => '', 'resetpasstime' => '')));
      if($account){
        return array('status' => 'success', 'text' => '重置密码成功');
      }else{
        return array('status' => 'success', 'text' => '重置密码失败');
      }
    }
  }
}