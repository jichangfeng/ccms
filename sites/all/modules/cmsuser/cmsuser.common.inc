<?php
/**
 * @author Changfeng Ji <jichf@qq.com>
 */
final class CMSUserField{
  /**
   * 
   * @access public
   * @static
   * @param MiniFieldData $mfd
   */
  public static function AfterConstruct(MiniFieldData $mfd){
  }
  /**
   * 
   * @access public
   * @static
   * @param MiniFieldData $mfd
   * @param array $errors
   * @return array $errors
   */
  public static function AfterProcessPostData(MiniFieldData $mfd, $errors){
    return array();
  }
  /**
   * 
   * @access public
   * @static
   * @param MiniFieldData $mfd
   * @param mixed $result
   */
  public static function AfterSaveDataToDB(MiniFieldData $mfd, $result){
    
  }

  public static function GetValueArraysOfCategoryId($separator = '\0', $group = ''){
    static $arrValueArrays;
    if(!isset($arrValueArrays)){
      $arrValueArrays = array();
      $arrValueArrays[] = array(0, "", "");
      $arrDatas = MiniFieldCategory::FilterDatasByCid(null, array('id', 'name'), array(), array(array('grouptag', 'user')), array(array('weight', 'ASC')), null, false, 0);
      foreach($arrDatas as $Data){
        $arrValueArrays[] = array($Data->id, $Data->name, "");
      }
    }
    return $arrValueArrays;
  }

  public static function ValidateRoleCategoryId(MiniFieldData $mfd){
    $errors = array();
    if($mfd->FieldModel['id'] > 0 && isset($mfd->FieldModel['roleid'])){
      $Role = CMSRole::LoadRoleByRid($mfd->FieldModel['roleid'], false);
      if(!empty($Role->categoryid) && $Role->categoryid != $mfd->FieldModel['categoryid'] && !CMSRole::CanChangeCategoryId($Role->roleid)){
        $errors['categoryid'] = '当前角色下存在用户，不能修改';
      }
    }
    return $errors;
  }

  public static function GetValueArraysOfCategoryPerm($separator = '\0', $group = ''){
    static $arrValueArrays;
    if(!isset($arrValueArrays)){
      $arrValueArrays = array();
      $arrRoles = CMSRole::LoadRoles(null, true);
      foreach($arrRoles as $Role){
        if($Role->rid == 3){ continue; }
        $arrValueArrays[] = array($group . '_begin', $Role->name);
        $arrValueArrays[] = array('add' . $Role->rid, '添加', '勾选以允许角色“' . $Role->name . '”下的用户在当前角色下添加用户');
        $arrValueArrays[] = array('edit' . $Role->rid, '编辑', '勾选以允许角色“' . $Role->name . '”下的用户在当前角色下编辑用户');
        $arrValueArrays[] = array('delete' . $Role->rid, '删除', '勾选以允许角色“' . $Role->name . '”下的用户在当前角色下删除用户');
        $arrValueArrays[] = array('view' . $Role->rid, '查看', '勾选以允许角色“' . $Role->name . '”下的用户在当前角色下查看用户');
        $arrValueArrays[] = array($group . '_end', $Role->name);
      }
    }
    return $arrValueArrays;
  }
}