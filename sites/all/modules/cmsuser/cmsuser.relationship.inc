<?php
/**
 * @author Changfeng Ji <jichf@qq.com>
 */
final class CMSUserRelationship{
  /**
   * 
   * @access public
   * @static
   * @param int $userid_self
   * @param int $userid_other
   * @return object The relationship object or null.
   */
  public static function GetRelationship($userid_self, $userid_other){
    if(!is_numeric($userid_self) || $userid_self == 0 || !is_numeric($userid_other) || $userid_other == 0  || $userid_self == $userid_other){
      return null;
    }
    $conditions = array();
    $subconditions = array();
    $subconditions[] = array('userid', $userid_self);
    $subconditions[] = array('related_uid', $userid_other);
    $conditions[] = array('conjunction' => 'AND', 'conditions' => $subconditions);
    $subconditions = array();
    $subconditions[] = array('userid', $userid_other);
    $subconditions[] = array('related_uid', $userid_self);
    $conditions[] = array('conjunction' => 'AND', 'conditions' => $subconditions);
    $arrDatas = MiniFieldData::FilterDatasByCid(CMSUSER_CID_RELATIONSHIP, array(), array(), array(array('conjunction' => 'OR', 'conditions' => $conditions)), array(array('mutual', 'DESC')), null, false);
    if(count($arrDatas) > 1){
      MiniFieldData::DeleteDatasByCid(CMSUSER_CID_RELATIONSHIP, array(array('conjunction' => 'OR', 'conditions' => $conditions), array('id', $arrDatas[0]->id, '!=')), true);
    }
    return isset($arrDatas[0]) ? $arrDatas[0] : null;
  }

  /**
   * 
   * @access public
   * @static
   * @param int $userid_self
   * @param int $userid_other
   */
  public static function DeleteRelationship($userid_self, $userid_other){
    if(!is_numeric($userid_self) || $userid_self == 0 || !is_numeric($userid_other) || $userid_other == 0  || $userid_self == $userid_other){
      return;
    }
    $conditions = array();
    $subconditions = array();
    $subconditions[] = array('userid', $userid_self);
    $subconditions[] = array('related_uid', $userid_other);
    $conditions[] = array('conjunction' => 'AND', 'conditions' => $subconditions);
    $subconditions = array();
    $subconditions[] = array('userid', $userid_other);
    $subconditions[] = array('related_uid', $userid_self);
    $conditions[] = array('conjunction' => 'AND', 'conditions' => $subconditions);
    MiniFieldData::DeleteDatasByCid(CMSUSER_CID_RELATIONSHIP, array(array('conjunction' => 'OR', 'conditions' => $conditions)), true);
  }

  /**
   * 
   * @access public
   * @static
   * @param int $userid_self
   * @param int $userid_other
   * @return boolean
   */
  public static function hasRelationship($userid_self, $userid_other){
    $Data = self::GetRelationship($userid_self, $userid_other);
    if(isset($Data->id) && $Data->mutual == 1){
      return true;
    }
    return false;
  }

  /**
   * 
   * @access public
   * @static
   * @param int $userid_self
   * @param int $userid_other
   * @return boolean
   */
  public static function hasApply($userid_self, $userid_other){
    $Data = self::GetRelationship($userid_self, $userid_other);
    if(isset($Data->id) && $Data->mutual == 0 && $Data->userid == $userid_self){
      return true;
    }
    return false;
  }

  /**
   * 
   * @access public
   * @static
   * @param int $userid_self
   * @param int $userid_other
   * @return boolean
   */
  public static function waitAccept($userid_self, $userid_other){
    $Data = self::GetRelationship($userid_self, $userid_other);
    if(isset($Data->id) && $Data->mutual == 0 && $Data->related_uid == $userid_self){
      return true;
    }
    return false;
  }

  /**
   * 
   * @access public
   * @static
   * @param int $userid_self
   * @param int $userid_other
   * @return array A array with two elements('status' => '$status', 'text' => '$text'):
   *  - $status: The status value 'error' or 'success'.
   *  - $text: The status text.
   */
  public static function Apply($userid_self, $userid_other){
    if(!is_numeric($userid_self) || $userid_self == 0 || !is_numeric($userid_other) || $userid_other == 0  || $userid_self == $userid_other){
      return array('status' => 'error', 'text' => '参数不合法');
    }
    if(self::hasRelationship($userid_self, $userid_other)){
      return array('status' => 'error', 'text' => '已经互为好友');
    }else if(self::hasApply($userid_self, $userid_other)){
      return array('status' => 'error', 'text' => '重复发出申请');
    }else if(self::waitAccept($userid_self, $userid_other)){
      return array('status' => 'error', 'text' => '对方已申请加您为好友');
    }
    $tablename = minifield_tablename_load(CMSUSER_CID_RELATIONSHIP);
    if(empty($tablename)){ return array('status' => 'error', 'text' => '未知错误'); }
    $result = db_insert($tablename)
      ->fields(array('userid' => $userid_self, 'related_uid' => $userid_other, 'mutual' => 0))
      ->execute();
    if((is_numeric($result) && preg_match('/^\d+$/', $result) > 0)){
      return array('status' => 'success', 'text' => '申请已发出');
    }else{
      return array('status' => 'error', 'text' => '未知错误');
    }
  }

  /**
   * 
   * @access public
   * @static
   * @param int $userid_self
   * @param int $userid_other
   * @return array A array with two elements('status' => '$status', 'text' => '$text'):
   *  - $status: The status value 'error' or 'success'.
   *  - $text: The status text.
   */
  public static function Accept($userid_self, $userid_other){
    if(!is_numeric($userid_self) || $userid_self == 0 || !is_numeric($userid_other) || $userid_other == 0  || $userid_self == $userid_other){
      return array('status' => 'error', 'text' => '参数不合法');
    }
    if(self::hasRelationship($userid_self, $userid_other)){
      return array('status' => 'error', 'text' => '已经互为好友');
    }else if(self::hasApply($userid_self, $userid_other)){
      return array('status' => 'error', 'text' => '自己发出的申请不能自己处理');
    }else if(self::waitAccept($userid_self, $userid_other)){
      $tablename = minifield_tablename_load(CMSUSER_CID_RELATIONSHIP);
      if(empty($tablename)){ return array('status' => 'error', 'text' => '未知错误'); }
      db_update($tablename)
        ->fields(array('mutual' => 1))
        ->condition('userid', $userid_other)
        ->condition('related_uid', $userid_self)
        ->execute();
      return array('status' => 'success', 'text' => '成功建立好友关系');
    }else{
      return array('status' => 'error', 'text' => '未知错误');
    }
  }

  /**
   * 
   * @access public
   * @static
   * @param int $userid_self
   * @param int $userid_other
   * @return array A array with two elements('status' => '$status', 'text' => '$text'):
   *  - $status: The status value 'error' or 'success'.
   *  - $text: The status text.
   */
  public static function Delete($userid_self, $userid_other){
    if(!is_numeric($userid_self) || $userid_self == 0 || !is_numeric($userid_other) || $userid_other == 0  || $userid_self == $userid_other){
      return array('status' => 'error', 'text' => '参数不合法');
    }
    self::DeleteRelationship($userid_self, $userid_other);
    return array('status' => 'success', 'text' => '关系已解除');
  }

  private static function Conditions_LoadRelationships($uid){
    if(!is_numeric($uid) || $uid == 0){ return array(); }
    $conditions = array();
    $subconditions = array();
    $subconditions[] = array('userid', $uid);
    $subconditions[] = array('related_uid', $uid);
    $conditions[] = array('conjunction' => 'OR', 'conditions' => $subconditions);
    $conditions[] = array('mutual', 1);
    return $conditions;
  }
  private static function Conditions_LoadRelationshipsOfOnline($uid){
    if(!is_numeric($uid) || $uid == 0){ return array(); }
    $conditions = array();
    $subconditions = array();
    $subconditions[] = array('userid', $uid);
    $subconditions[] = array('related_uid', $uid);
    $conditions[] = array('conjunction' => 'OR', 'conditions' => $subconditions);
    $conditions[] = array('mutual', 1);
    $uides = CMSUser::LoadUserUidesOnlineByRides(array(), array(array('uid', $uid, '!=')));
    if(!empty($uides)){
      $subconditions = array();
      $subconditions[] = array('userid', $uides, 'IN');
      $subconditions[] = array('related_uid', $uides, 'IN');
      $conditions[] = array('conjunction' => 'OR', 'conditions' => $subconditions);
    }else{
      return array();
    }
    return $conditions;
  }
  private static function Conditions_LoadRelationshipsOfHasApply($uid){
    if(!is_numeric($uid) || $uid == 0){ return array(); }
    $conditions = array();
    $conditions[] = array('userid', $uid);
    $conditions[] = array('mutual', 0);
    return $conditions;
  }
  private static function Conditions_LoadRelationshipsOfWaitAccept($uid){
    if(!is_numeric($uid) || $uid == 0){ return array(); }
    $conditions = array();
    $conditions[] = array('related_uid', $uid);
    $conditions[] = array('mutual', 0);
    return $conditions;
  }

  public static function LoadRelationships($uid, $limit = null, $element = 0){
    if(!is_numeric($uid) || $uid == 0){ return array(); }
    $conditions = self::Conditions_LoadRelationships($uid);
    $arrDatas = MiniFieldData::FilterDatasByCid(CMSUSER_CID_RELATIONSHIP, array(), array(), $conditions, array(), $limit, false, $element);
    return $arrDatas;
  }
  public static function LoadRelationships_Uides($uid){
    $arrDatas = self::LoadRelationships($uid);
    $arrs = array();
    foreach($arrDatas as $Data){ $arrs[] = $Data->userid == $uid ? $Data->related_uid : $Data->userid; }
    return $arrs;
  }
  public static function LoadRelationships_Count($uid){
    if(!is_numeric($uid) || $uid == 0){ return array(); }
    $conditions = self::Conditions_LoadRelationships($uid);
    return MiniFieldData::LoadDataCountByCid(CMSUSER_CID_RELATIONSHIP, $conditions);
  }

  public static function LoadRelationshipsOfOnline($uid, $limit = null, $element = 0){
    if(!is_numeric($uid) || $uid == 0){ return array(); }
    $conditions = self::Conditions_LoadRelationshipsOfOnline($uid);
    $arrDatas = MiniFieldData::FilterDatasByCid(CMSUSER_CID_RELATIONSHIP, array(), array(), $conditions, array(), $limit, false, $element);
    return $arrDatas;
  }
  public static function LoadRelationshipsOfOnline_Uides($uid){
    $arrDatas = self::LoadRelationshipsOfOnline($uid);
    $arrs = array();
    foreach($arrDatas as $Data){ $arrs[] = $Data->userid == $uid ? $Data->related_uid : $Data->userid; }
    return $arrs;
  }
  public static function LoadRelationshipsOfOnline_Count($uid){
    if(!is_numeric($uid) || $uid == 0){ return array(); }
    $conditions = self::Conditions_LoadRelationshipsOfOnline($uid);
    return MiniFieldData::LoadDataCountByCid(CMSUSER_CID_RELATIONSHIP, $conditions);
  }

  public static function LoadRelationshipsOfHasApply($uid, $limit = null, $element = 0){
    if(!is_numeric($uid) || $uid == 0){ return array(); }
    $conditions = self::Conditions_LoadRelationshipsOfHasApply($uid);
    $arrDatas = MiniFieldData::FilterDatasByCid(CMSUSER_CID_RELATIONSHIP, array(), array(), $conditions, array(), $limit, false, $element);
    return $arrDatas;
  }
  public static function LoadRelationshipsOfHasApply_Uides($uid){
    $arrDatas = self::LoadRelationshipsOfHasApply($uid);
    $arrs = array();
    foreach($arrDatas as $Data){ $arrs[] = $Data->userid == $uid ? $Data->related_uid : $Data->userid; }
    return $arrs;
  }
  public static function LoadRelationshipsOfHasApply_Count($uid){
    if(!is_numeric($uid) || $uid == 0){ return array(); }
    $conditions = self::Conditions_LoadRelationshipsOfHasApply($uid);
    return MiniFieldData::LoadDataCountByCid(CMSUSER_CID_RELATIONSHIP, $conditions);
  }

  public static function LoadRelationshipsOfWaitAccept($uid, $limit = null, $element = 0){
    if(!is_numeric($uid) || $uid == 0){ return array(); }
    $conditions = self::Conditions_LoadRelationshipsOfWaitAccept($uid);
    $arrDatas = MiniFieldData::FilterDatasByCid(CMSUSER_CID_RELATIONSHIP, array(), array(), $conditions, array(), $limit, false, $element);
    return $arrDatas;
  }
  public static function LoadRelationshipsOfWaitAccept_Uides($uid){
    $arrDatas = self::LoadRelationshipsOfWaitAccept($uid);
    $arrs = array();
    foreach($arrDatas as $Data){ $arrs[] = $Data->userid == $uid ? $Data->related_uid : $Data->userid; }
    return $arrs;
  }
  public static function LoadRelationshipsOfWaitAccept_Count($uid){
    if(!is_numeric($uid) || $uid == 0){ return array(); }
    $conditions = self::Conditions_LoadRelationshipsOfWaitAccept($uid);
    return MiniFieldData::LoadDataCountByCid(CMSUSER_CID_RELATIONSHIP, $conditions);
  }
}