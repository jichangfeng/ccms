<?php
/**
 * @author Changfeng Ji <jichf@qq.com>
 */
final class CMSNodeField{
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

  /**
   *
   * @access public
   * @static
   * @param string $separator Defaults to an string '\0'.
   * @param string $group Defaults to an string ''.
   * @return array Each element is still a array with one to four elements($value[, $name, $title, $selected])
   */
  public static function GetValueArraysOfStatus($separator = '\0', $group = ''){
    $arrValueArrays = array();
    $arrValueArrays[] = array(1, '<span style="color: green;">'.ts('已发布', 'ucwords', 'node').'</span>', '', 1);
    $arrValueArrays[] = array(0, '<span style="color: red;">'.ts('未发布', 'ucwords', 'node').'</span>', '');
    return $arrValueArrays;
  }
}