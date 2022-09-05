<?php
/**
 * @author Changfeng Ji <jichf@qq.com>
 */
final class CMSDataField{
  /**
   *
   * @access public
   * @static
   * @var array The allowed minifield category id collection of cmsdata module.
   */
  public static $AllowedCides = array(
    CMSDATA_CID_TRANSLATION => '网站翻译',
    CMSDATA_CID_SITEINFO => '网站信息',
    CMSDATA_CID_NAVIGATION => '网站栏目'
  );

  public static function ValidateSiteInfoThemePath(MiniFieldData $mfd){
    $errors = array();
    if(!empty($mfd->FieldModel['themepath']) && !is_dir($mfd->FieldModel['themepath'])){
      $errors['themepath'] = '主题路径不存在';
    }
    return $errors;
  }

  public static function ValidateSiteInfoStaticPath(MiniFieldData $mfd){
    $errors = array();
    if(!empty($mfd->FieldModel['staticpath']) && !is_dir($mfd->FieldModel['staticpath'])){
      $errors['staticpath'] = '静态化路径不存在';
    }
    return $errors;
  }

  public static function GetValueArraysOfParentId($separator = '\0', $group = ''){
    static $arrValueArrays;
    if(!isset($arrValueArrays)){
      $arrValueArrays = array();
      $arrValueArrays[] = array(0, '请选择', '顶级栏目');
      $arrNavigations = CMSNavigation::LoadNavigations(0, 1, false);
      $arrNavigationTrees = CMSNavigation::LoadNavigationTrees($arrNavigations);
      foreach($arrNavigationTrees as $Navigation){
        $arrValueArrays[] = array($Navigation->id, $Navigation->level . $Navigation->name, $Navigation->name);
      }
    }
    return $arrValueArrays;
  }

  public static function ValidateNavigationParentId(MiniFieldData $mfd){
    $errors = array();
    if($mfd->FieldModel['id'] > 0){
      if($mfd->FieldModel['parentid'] == $mfd->FieldModel['id']){
        $errors['parentid'] = '父级栏目不能是自己';
      }else{
        $arrNavigations = CMSNavigation::LoadNavigations($mfd->FieldModel['id'], 1, false);
        $ides = CMSNavigation::LoadNavigationNavides($arrNavigations);
        if(in_array($mfd->FieldModel['parentid'], $ides)){
          $errors['parentid'] = '父级栏目不能是其子栏目';
        }
      }
    }
    return $errors;
  }

  public static function GetValueArraysOfCategoryId($separator = '\0', $group = ''){
    static $arrValueArrays;
    if(!isset($arrValueArrays)){
      $arrValueArrays = array();
      $arrValueArrays[] = array(0, "", "");
      $arrDatas = MiniFieldCategory::FilterDatasByCid(null, array('id', 'name'), array(), array(array('grouptag', 'content')), array(array('weight', 'ASC')), null, false, 0);
      foreach($arrDatas as $Data){
        $arrValueArrays[] = array($Data->id, $Data->name, "");
      }
    }
    return $arrValueArrays;
  }

  public static function ValidateNavigationCategoryId(MiniFieldData $mfd){
    $errors = array();
    if($mfd->FieldModel['id'] > 0){
      $Navigation = CMSNavigation::LoadNavigationById($mfd->FieldModel['id'], false);
      if(!empty($Navigation->categoryid) && $Navigation->categoryid != $mfd->FieldModel['categoryid'] && !CMSNavigation::CanChangeCategoryId($Navigation->id)){
        $errors['categoryid'] = '当前栏目下存在内容，不能修改';
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
        $arrValueArrays[] = array('addall' . $Role->rid, '添加', '勾选以允许角色“' . $Role->name . '”下的用户在当前栏目下添加内容');
        $arrValueArrays[] = array('editall' . $Role->rid, '编辑全部', '勾选以允许角色“' . $Role->name . '”下的用户在当前栏目下编辑全部内容');
        $arrValueArrays[] = array('editself' . $Role->rid, '编辑自身', '勾选以允许角色“' . $Role->name . '”下的用户在当前栏目下编辑自己添加的内容');
        $arrValueArrays[] = array('deleteall' . $Role->rid, '删除全部', '勾选以允许角色“' . $Role->name . '”下的用户在当前栏目下删除全部内容');
        $arrValueArrays[] = array('deleteself' . $Role->rid, '删除自身', '勾选以允许角色“' . $Role->name . '”下的用户在当前栏目下删除自己添加的内容');
        $arrValueArrays[] = array('viewall' . $Role->rid, '查看全部', '勾选以允许角色“' . $Role->name . '”下的用户在当前栏目下查看全部内容');
        $arrValueArrays[] = array('viewself' . $Role->rid, '查看自身', '勾选以允许角色“' . $Role->name . '”下的用户在当前栏目下查看自己添加的内容');
        $arrValueArrays[] = array($group . '_end', $Role->name);
      }
    }
    return $arrValueArrays;
  }

  public static function ValidateNavigationUrlAliasUnique(MiniFieldData $mfd){
    $errors = array();
    if(empty($mfd->FieldModel['urlalias'])){ return $errors; }
    if(preg_match('/^[a-z]*$/', $mfd->FieldModel['urlalias']) == 0){
      $errors['urlalias'] = '地址别名只能是小写英文字母或不填';
      return $errors;
    }
    $Category = $mfd->Category;
    if(empty($Category->tablename)){
      $errors['urlalias'] = '服务器出错，无法检查';
      return $errors;
    }
    $rc = MiniFieldSql::LoadDataCountByTableField($Category->tablename, 'urlalias', array(array('urlalias', $mfd->FieldModel['urlalias']), array('id', $mfd->FieldModel['id'] , '!=')));
    if($rc > 0){
      $errors['urlalias'] = '已存在的地址别名';
    }
    return $errors;
  }
}