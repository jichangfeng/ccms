<?php
/**
 * @author Changfeng Ji <jichf@qq.com>
 */
final class CMSRole{
  public static function ClearRoles(){
    $rids = MiniFieldSql::LoadDataColByTableField('role', 'rid', array(array('rid', 2, '>')), array(array('weight', 'ASC'), array('name', 'ASC')));
    if(empty($rids)){ return; }
    MiniFieldData::DeleteDatasByCid(CMSUSER_CID_ROLE, array(array('roleid', $rids, 'NOT IN')), true);
    foreach($rids as $rid){
      $ides = MiniFieldData::LoadDataIdesByCid(CMSUSER_CID_ROLE, array(array('roleid', $rid)));
      if(!isset($ides[0])){
        $_POST = array('roleid' => $rid);
        $mfd = new MiniFieldData(CMSUSER_CID_ROLE, 0);
        $mfd->SaveDataToDB();
      }else if(isset($ides[1])){
        unset($ides[0]);
        MiniFieldData::DeleteDatasByCid(CMSUSER_CID_ROLE, array(array('id', $ides, 'IN')), true);
      }
    }
  }

  /**
   *
   * @access public
   * @static
   * @param type $limit The page limit or null.
   * @param bool $format
   * @param int $element An optional integer to distinguish between multiple pagers on one page.
   * @return type
   */
  public static function LoadRoles($limit = null, $format = false, $element = 0){
    $StaticName = __CLASS__ . '_' . __FUNCTION__ . '_' . ($format ? 1 : 0);
    if(is_null($limit)){
      $arrDatas = MiniFieldStatic::GetStatic($StaticName, true);
      if(isset($arrDatas)){
        return $arrDatas;
      }
    }
    $tables = array();
    $tables[] = array('role', '', 'rid', array(), array());
    $tablename = minifield_tablename_load(CMSUSER_CID_ROLE);
    if(!empty($tablename)){
      $tables[] = array($tablename, 'LEFT OUTER', 'roleid', array(), array());
    }
    $conditions = array();
    $conditions[] = array('rid', 2, '>');//exclude DRUPAL_ANONYMOUS_RID, DRUPAL_AUTHENTICATED_RID
    $orderbys = array();
    $orderbys[] = array('weight', 'ASC');
    $orderbys[] = array('name', 'ASC');
    $arrDatas = MiniFieldSql::FilterDatasByTables($tables, $conditions, $orderbys, $limit, $element);
    if($format == true){
      $arrFields = MiniField::LoadDatasByCid(CMSUSER_CID_ROLE, null, false);
      $arrDatas = MiniFieldData::FormatDatas($arrDatas, $arrFields);
    }
    if(is_null($limit)){
      MiniFieldStatic::SetStatic($StaticName, $arrDatas, true);
    }
    return $arrDatas;
  }

  public static function LoadRoleByRid($rid, $format = false){
    $StaticName = __CLASS__ . '_' . __FUNCTION__ . '_' . $rid . '_' . ($format ? 1 : 0);
    $Data = MiniFieldStatic::GetStatic($StaticName, true);
    if(isset($Data)){
      return $Data;
    }
    $tables = array();
    $tables[] = array('role', '', 'rid', array(), array());
    $tablename = minifield_tablename_load(CMSUSER_CID_ROLE);
    if(!empty($tablename)){
      $tables[] = array($tablename, 'LEFT OUTER', 'roleid', array(), array());
    }
    $conditions = array();
    $conditions[] = array('rid', $rid);
    $Data = MiniFieldSql::FilterDatasByTables($tables, $conditions, array(), null, 0, array('fetchObject'));
    if($format == true){
      $arrFields = MiniField::LoadDatasByCid(CMSUSER_CID_ROLE, null, false);
      $Data = MiniFieldData::FormatData($Data, $arrFields);
    }
    MiniFieldStatic::SetStatic($StaticName, $Data, true);
    return $Data;
  }

  public static function ExistRoleByRid($rid){
    $Data = self::LoadRoleByRid($rid);
    return isset($Data->rid) ? true : false;
  }

  public static function CanDeleted($rid){
    $usercount = CMSUser::LoadUserCountByRides(array($rid));
    return $usercount > 0 ? false : true;
  }

  public static function CanChangeCategoryId($rid){
    $usercount = CMSUser::LoadUserCountByRides(array($rid));
    return $usercount > 0 ? false : true;
  }

  /**
   *
   * @access public
   * @static
   * @param string $Permission The permission type, include view add edit delete.
   * @param object/int $User The user object or user id.
   * @param object/int $Role The formatted role object or $Role id.
   * @return int 0 = can not manage user; 1 = can manage user.
   */
  public static function CanManageUser($Permission, $User, $Role){
    if(!in_array($Permission, array('view', 'add', 'edit', 'delete'))){ return 0; }
    if(!isset($User->uid) || !isset($User->roles)){
      $User = user_load($User);
      if(!isset($User->uid) || !isset($User->roles)){ return 0; }
    }
    if($User->uid == 1 || isset($User->roles[3])){ return 1; }
    if(!isset($Role->rid)){
      $Role = self::LoadRoleByRid($Role, true);
      if(!isset($Role->rid) || !isset($Role->categoryperm)){ return 0; }
    }
    $Permissions = array();
    foreach($Role->categoryperm as $ValueArray){
      $Permissions[] = $ValueArray[0];
    }
    $rides = array_keys($User->roles);
    $result = 0;
    foreach($rides as $rid){
      if(in_array($Permission . $rid, $Permissions)){
        $result = 1;
        break;
      }
    }
    return $result;
  }

  public static function DeleteRoleByRid($rid){
    if(self::CanDeleted($rid)){
      $Data = self::LoadRoleByRid($rid);
      user_role_delete((int)$Data->rid);
      if(isset($Data->id)){
        MiniFieldData::DeleteDataByCidId(CMSUSER_CID_ROLE, $Data->id, true);
      }
      MiniFieldStatic::ClearStatics(__CLASS__);
    }
  }

  public static function DeleteRolesByRides($rides){
    $ides = array();
    foreach($rides as $rid){
      if(self::CanDeleted($rid)){
        $Data = self::LoadRoleByRid($rid);
        if(isset($Data->id)){ $ides[] = $Data->id; }
        user_role_delete((int)$Data->rid);
      }
    }
    if(!empty($ides)){
      MiniFieldData::DeleteDatasByCid(CMSUSER_CID_ROLE, array(array('id', $ides, 'IN')), true);
    }
    MiniFieldStatic::ClearStatics(__CLASS__);
  }

  /**
   *
   * @access public
   * @static
   * @param array $rid The role id.
   * @param boolean $reset to reset the internal cache and load from the database; FALSE (default) to load from the internal cache, if set.
   * @return string The role name, or empty string if not found.
   */
  public static function GetRoleNameByRid($rid, $reset = false){
    $StaticName = __CLASS__ . '_' . __FUNCTION__;
    $arrs = MiniFieldStatic::GetStatic($StaticName, true);
    if($reset){
      $arrs = MiniFieldStatic::ClearStatic($StaticName);
    }
    if(!isset($arrs)){
      $arrs = MiniFieldSql::FilterDatasByTable('role', array('rid', 'name'), array(), array(), array(array('weight', 'ASC'), array('name', 'ASC')), null, 0, array('fetchAllKeyed'));
      MiniFieldStatic::SetStatic($StaticName, $arrs, true);
    }
    return isset($arrs[$rid]) ? $arrs[$rid] : '';
  }
  /**
   *
   * @access public
   * @static
   * @param array $rid The role id.
   * @param boolean $reset to reset the internal cache and load from the database; FALSE (default) to load from the internal cache, if set.
   * @return int The role category id, or 0 if not found.
   */
  public static function GetRoleCategoryIdByRid($rid, $reset = false){
    $StaticName = __CLASS__ . '_' . __FUNCTION__;
    $arrs = MiniFieldStatic::GetStatic($StaticName, true);
    if($reset){
      $arrs = MiniFieldStatic::ClearStatic($StaticName);
    }
    if(!isset($arrs)){
      $arrs = MiniFieldData::FilterDatasByCid(CMSUSER_CID_ROLE, array('roleid', 'categoryid'), array(), array(), array(), null, false, 0, array('fetchAllKeyed'));
      MiniFieldStatic::SetStatic($StaticName, $arrs, true);
    }
    return isset($arrs[$rid]) ? $arrs[$rid] : 0;
  }

  /**
   *
   * @access public
   * @static
   * @param int $rid role id.
   * @param MiniFieldData $mfd
   * @return int/array :
   *   - The last insert/update ID of the query, if successfully.
   *   - An associative array of reasons for failure, where the keys are the field name and the values are the values those field name must have, if failed.
   */
  public static function SaveRole($rid, MiniFieldData $mfd){
    //$mfd = new MiniFieldData(CMSUSER_CID_ROLE);
    $role = user_role_load($rid);
    if(!isset($role->rid) || !is_numeric($role->rid)){
      $role = new stdClass();
      $role->name = '';
      $role->weight = 0;
    }
    if(!isset($_POST['name']) && isset($role->name)){
      $_POST['name'] = $role->name;
    }
    if(!isset($_POST['weight']) && isset($role->weight)){
      $_POST['weight'] = $role->weight;
    }
    $errors = array();
    if(empty($_POST['name'])){
      $errors['name'] = '角色名必须填写';
    }else if($_POST['name'] != $role->name && user_role_load_by_name($_POST['name']) != FALSE){
      $errors['name'] = '角色名 '.$_POST['name'].' 已存在';
    }
    if(preg_match('/^\d+$/', $_POST['weight']) == 0){
      $errors['weight'] = '权重必须选择';
    }
    $errors = array_merge($errors, $mfd->ProcessPostData());
    if(isset($errors['roleid'])){ unset($errors['roleid']); }
    if(empty($errors)){
      $role->name = $_POST['name'];
      $role->weight = $_POST['weight'];
      user_role_save($role);
      $_POST['roleid'] = $role->rid;
      $mfd->FieldModel['roleid'] = $_POST['roleid'];
      $errors = $mfd->SaveDataToDB();
    }
    MiniFieldStatic::ClearStatics(__CLASS__);
    return $errors;
  }
}