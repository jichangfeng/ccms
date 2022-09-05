<?php
/**
 * @author Changfeng Ji <jichf@qq.com>
 */
final class CMSTranslation{
  /**
   * The custom of translation.
   * @access public
   * @static
   * @return array
   */
  public static function Translations(){
    $StaticName = __CLASS__ . '_' . __FUNCTION__;
    $translations = MiniFieldStatic::GetStatic($StaticName, true);
    if(isset($translations)){
      return $translations;
    }
    $translations = array('id' => array(), 'all' => array());
    foreach(MiniFieldTranslation::GroupBies() as $Groupby){
      $translations[$Groupby[0]] = array();
    }
    $arrDatas = MiniFieldData::LoadDatasByCid(CMSDATA_CID_TRANSLATION, null, false);
    foreach ($arrDatas as $Data) {
      $translations['id'][$Data->id] = $Data;
      $arrs = array();
      foreach(MiniFieldTranslation::$Languages as $Language){
        $arrs[$Data->$Language] = $Data;
      }
      $translations['all'] = array_merge($translations['all'], $arrs);
      if(isset($translations[$Data->groupby])){
        $translations[$Data->groupby] = array_merge($translations[$Data->groupby], $arrs);
      }
    }
    MiniFieldStatic::SetStatic($StaticName, $translations, true);
    return $translations;
  }
}