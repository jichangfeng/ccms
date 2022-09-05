<?php
/**
 * @author Changfeng Ji <jichf@qq.com>
 */
final class MiniFieldCategory extends MiniFieldData{
  public function __construct($cid, $id = null, $events = array()) {
    parent::__construct($cid, $id);
    if($id == 0 && isset($this->FieldModel['weight'])){
      $this->FieldModel['weight'] = (int)MiniFieldSql::LoadDataExpressionByTableField($this->Category->tablename, 'weight', 'MAX') + 1;
    }
  }
  public function InitStructure($cid, $id) {
    $this->Category = MiniFieldCategoryStructure::Category();
    $this->arrFields = MiniFieldCategoryStructure::arrFields();
  }
  public function AssemblyData($cid, $id) {
    if($id > 0){
      $Data = self::LoadDataByCidId($cid, $id, false);
      if(isset($Data->id)){
        foreach($this->FieldModel as $key=>$value){
          $this->FieldModel[$key] = isset($Data->$key) ? $Data->$key : "";
        }
        $this->IsExists = true;
      }
    }
  }
  public function SaveDataToDB() {
    $Data = new stdClass();
    if($this->FieldModel['id'] > 0){
      $Data = self::LoadDataByCidId(null, $this->FieldModel['id'], false);
    }
    $result = parent::SaveDataToDB();
    if(is_array($result)){
      return $result;
    }
    $table = array(
      'description' => $this->FieldModel['table_description'],
      'fields' => array('id' => array('type' => 'serial', 'unsigned' => TRUE, 'not null' => TRUE)),
      'primary key' => array('id')
    );
    if(!empty($this->FieldModel['table_type'])){ $table['mysql_engine'] = $this->FieldModel['table_type']; }
    if(!empty($this->FieldModel['table_collation'])){
      $collation = $this->FieldModel['table_collation'];
      list($charset) = explode('_', $collation);
      $table['mysql_character_set'] = $charset;
      //$table['collation'] = $charset == $collation ? '' : $collation;
      //$table['mysql_suffix'] = (' ENGINE=' . $this->FieldModel['table_type']) . (' DEFAULT CHARSET=' . $charset) . ($charset == $collation ? '' : ' COLLATE ' . $collation) . ' ';
    }
    if(isset($Data->id)){
      if(db_table_exists($Data->tablename) && $Data->tablename != $this->FieldModel['tablename']){
        db_rename_table($Data->tablename, $this->FieldModel['tablename']);
      }else{
        if(!db_table_exists($this->FieldModel['tablename'])){
          db_create_table($this->FieldModel['tablename'], $table);
        }
      }
    }else{
      if(!db_table_exists($this->FieldModel['tablename'])){
        db_create_table($this->FieldModel['tablename'], $table);
      }
    }
    MiniFieldStatic::ClearStatic('minifield_tablename_load');
    MiniFieldStatic::ClearStatic('minifield_cid_load');
    MiniFieldStatic::ClearStatics(__CLASS__);
    return $result;
  }
  
  /* static start */
  public static function ValidateTableName(MiniFieldData $mfd){
    $errors = array();
    $CurrentTN = $mfd->Category->tablename;
    $rc = MiniFieldSql::LoadDataCountByTableField($CurrentTN, 'tablename', array(array('tablename', $mfd->FieldModel['tablename']), array('id', $mfd->FieldModel['id'] , '!=')));
    if($rc > 0){
      $errors['tablename'] = ts('表名称 %tablename 已存在，不能重复', 'ucfirst', 'minifield', array('%tablename' => $mfd->FieldModel['tablename']));
    }
    return $errors;
  }
  
  /**
   * Save data to database table.
   * includes process data, check data, save data.
   * @access public
   * @static
   * @param array $variables
   *   An array of fields and values to save. For example array('name'
   *   => 'My name').
   *   The default variables include
   *     id = 0, name = '', tablename = '', grouptag = '', weight = 1,
   *     table_type = 'MyISAM', table_collation = 'utf8_general_ci', table_description = ''.
   * @return int/array :
   *   - The last insert/update ID of the query, if successfully.
   *   - An associative array of reasons for failure, where the keys are the field name and the values are the values those field name must have, if failed.
   */
  public static function SaveData($variables = array()){
    $backupPOST = $_POST;
    $variables_default = array(
      'id' => 0,
      'name' => '',
      'tablename' => '',
      'grouptag' => '', 'switchofgrouptag' => '',
      'weight' => 1,
      'table_type' => 'MyISAM', 'switchoftable_type' => '',
      'table_collation' => 'utf8_general_ci', 'switchoftable_collation' => '',
      'table_description' => ''
    );
    $_POST = array_merge($variables_default, $variables);
    $mfc = new MiniFieldCategory(null, $_POST['id']);
    if($mfc->IsExists == true){
      foreach($variables as $key => $value){
        if(isset($variables_default['switchof' . $key])){
          $variables['switchof' . $key] = '';
        }
      }
      $_POST = $variables;
    }
    $result = $mfc->SaveDataToDB();
    $_POST = $backupPOST;
    return $result;
  }
  
  /**
   * @access public
   * @static
   * @param int $cid The invalid parameter(such as null).
   * @param type $limit The page limit or null.
   * @param boolean $format whether to format loaded data or not.
   * @param int $element An optional integer to distinguish between multiple pagers on one page.
   * @param boolean $invokehook whether to invoke hook hook_minifielddata_load_$cid($arrDatas = array()).
   * @return array The loaded data array. 
   */
  public static function LoadDatasByCid($cid, $limit = null, $format = false, $element = 0, $invokehook = true) {
    $StaticName = __CLASS__ . '_' . __FUNCTION__ . '_' . ($format ? 1 : 0);
    if(is_null($limit)){
      $arrDatas = MiniFieldStatic::GetStatic($StaticName, true);
      if(isset($arrDatas)){
        return $arrDatas;
      }
    }
    $Category = MiniFieldCategoryStructure::Category();
    if(empty($Category->tablename)){ return array(); }
    $arrDatas = MiniFieldSql::FilterDatasByTable($Category->tablename, array(), array(), array(), array(array('weight', 'ASC'), array('id', 'ASC')), $limit, $element);
    if($format == true){
      $arrFields = MiniFieldCategoryStructure::arrFields();
      $arrDatas = MiniFieldData::FormatDatas($arrDatas, $arrFields);
    }
    if(is_null($limit)){
      MiniFieldStatic::SetStatic($StaticName, $arrDatas, true);
    }
    //if($invokehook == true){ module_invoke_all('minifielddata_load_' . $cid, $arrDatas); }
    return $arrDatas;
  }
  /**
   * @access public
   * @static
   * @param int $cid The invalid parameter(such as null).
   * @param int $id The minifield data identifier.
   * @param boolean $format whether to format loaded data or not.
   * @param boolean $invokehook whether to invoke hook hook_minifielddata_load_$cid($arrDatas = array()).
   * @return object The loaded data object.
   */
  public static function LoadDataByCidId($cid, $id, $format = false, $invokehook = true) {
    $StaticName = __CLASS__ . '_' . __FUNCTION__ . '_' . $id . '_' . ($format ? 1 : 0);
    $Data = MiniFieldStatic::GetStatic($StaticName, true);
    if(isset($Data)){
      return $Data;
    }
    $Category = MiniFieldCategoryStructure::Category();
    if(empty($Category->tablename)){ return null; }
    $Data = MiniFieldSql::LoadDataByTableId($Category->tablename, $id);
    if($format == true){
      $arrFields = MiniFieldCategoryStructure::arrFields();
      $Data = self::FormatData($Data, $arrFields);
    }
    MiniFieldStatic::SetStatic($StaticName, $Data, true);
    //if($invokehook == true){ module_invoke_all('minifielddata_load_' . $cid, array($Data)); }
    return $Data;
  }
  /**
   * @access public
   * @static
   * @param int $cid The invalid parameter(such as null).
   * @param int $id The minifield data identifier.
   * @param boolean $includefiles whether to delete related files or not.
   * @param boolean $invokehook whether to invoke hook hook_minifielddata_delete_$cid($ides = array()).
   */
  public static function DeleteDataByCidId($cid, $id, $includefiles = false, $invokehook = true) {
    $Category = MiniFieldCategoryStructure::Category();
    if(empty($Category->tablename)){ return; }
    //if($invokehook == true){ module_invoke_all('minifielddata_delete_' . $cid, array($id)); }
    $includetable = true;
    if($includetable == true){
      $Data = self::LoadDataByCidId($cid, $id, false);
      if(isset($Data->id) && db_table_exists($Data->tablename)){
        db_drop_table($Data->tablename);
      }
    }
    if($includefiles == true){
      $arrFields = MiniFieldCategoryStructure::arrFields();
      $Data = self::LoadDataByCidId($cid, $id, false);
      self::DeleteDataFiles($Data, $arrFields);
    }
    MiniFieldSql::DeleteDataByTableId($Category->tablename, $id);
    $Category = MiniFieldStructure::Category();
    if(empty($Category->tablename)){ return; }
    MiniFieldSql::DeleteDatasByTable($Category->tablename, array(array('cid', $id)));
    MiniFieldStatic::ClearStatic('minifield_tablename_load');
    MiniFieldStatic::ClearStatic('minifield_cid_load');
    MiniFieldStatic::ClearStatics(__CLASS__);
  }
  /**
   * Filter datas by minifield category identifier
   * @access public
   * @static
   * @param int $cid The invalid parameter(such as null).
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
   * @param array $fetchmode The fetch mode. Possible value:
   *  - array('fetchAll'): Returns an array containing all of the result set rows.
   *  - array('fetchAllAssoc', $key): Returns the result set as an associative array keyed by the given field.
   *      If the given key appears multiple times, later records will overwrite earlier ones.
   *      $key is a name of the field on which to index the array.
   *  - array('fetchAllKeyed'[, $key_index, $value_index]): Returns the entire result set as a single associative array.
   *      This method is only useful for two-column result sets.
   *      It will return an associative array where the key is one column from the result set and the value is another field.
   *      In most cases, the default of the first two columns is appropriate.
   *      $key_index is a numeric index of the field to use as the array key, default to 0.
   *      $value_index is a numeric index of the field to use as the array value, default to 1.
   *  - array('fetchCol'[, $index]): Returns an entire single column of a result set as an indexed array.
   *      $index a index of the column number to fetch, default to 0.
   *  - array('fetchObject'): Fetches the next row and returns it as an object.
   *  - array('fetchField'[, $index]): Returns a single field from the next record of a result set.
   *      $index The numeric index of the field to return. Defaults to the first field.
   *  - array('rowCount'): Returns the number of rows affected by the last SQL statement.
   * @param boolean $invokehook whether to invoke hook hook_minifielddata_load_$cid($arrDatas = array()).
   * @param string $callback Perform necessary alterations to the <b>SelectQuery</b> before it is executed.
   *  - Possible value: a function string, or a class-level static function string(the class string and function string are separated by '|').
   *  - Function argument: A SelectQuery object for this connection.
   *  - Function expected return value: A new SelectQuery object for this connection.
   *  - Function example: function test($queryObj){ return $queryObj; }
   * @return mixed The return value of this function on success depends on the fetch mode($fetchmode):
   *  - fetchAll: An array containing all of the remaining rows in the result set.
   *  - fetchAllAssoc: An associative array, or an empty array if there is no result set.
   *  - fetchAllKeyed: An associative array, or an empty array if there is no result set.
   *  - fetchCol: An indexed array, or an empty array if there is no result set.
   *  - fetchObject: An instance of the required class with property names that correspond to the column names or <b>FALSE</b> on failure.
   *  - fetchField: A single field from the next record, or <b>FALSE</b> if there is no next record.
   *  - rowCount: The number of rows.
   */
  public static function FilterDatasByCid($cid, $fields = array(), $fields_alias = array(), $conditions = array(), $orderbys = array(), $limit = null, $format = false, $element = 0, $fetchmode = array('fetchAll'), $invokehook = true, $callback = '') {
    $Category = MiniFieldCategoryStructure::Category();
    if(empty($Category->tablename)){ return array(); }
    $arrDatas = MiniFieldSql::FilterDatasByTable($Category->tablename, $fields, $fields_alias, $conditions, $orderbys, $limit, $element, $fetchmode, $callback);
    if($format == true && isset($fetchmode[0]) && in_array($fetchmode[0], array('fetchAll', 'fetchAllAssoc'))){
      $arrFields = MiniFieldCategoryStructure::arrFields();
      $arrDatas = MiniFieldData::FormatDatas($arrDatas, $arrFields, $fields_alias);
    }else if($format == true && isset($fetchmode[0]) && $fetchmode[0] == 'fetchObject'){
      $arrFields = MiniFieldCategoryStructure::arrFields();
      $arrDatas = self::FormatData($arrDatas, $arrFields, $fields_alias);
    }
    /* if($invokehook == true && isset($fetchmode[0]) && in_array($fetchmode[0], array('fetchAll', 'fetchAllAssoc'))){
      module_invoke_all('minifielddata_load_' . $cid, $arrDatas);
    }else if($invokehook == true && isset($fetchmode[0]) && $fetchmode[0] == 'fetchObject'){
      module_invoke_all('minifielddata_load_' . $cid, array($arrDatas));
    } */
    return $arrDatas;
  }
  /* static end */
}
final class MiniFieldCategoryStructure{
  public static function Category(){
    $Category = array(
      'id' => 0,
      'name' => '字段类别',
      'tablename' => 'minifieldcategory',
      'grouptag' => '',
      'weight' => 0,
      'table_type' => 'MyISAM',
      'table_collation' => '',
      'table_description' => ''
    );
    return (object)$Category;
  }
  public static function arrFields(){
    $Category = self::Category();
    $arrFields = array(
      array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '显示名称',
        'fieldname' => 'name',
        'editor' => 'text',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 1,
        'weight' => 1,
        'field_type' => 'varchar',
        'field_size' => 'normal',
        'field_notnull' => 1,
        'field_default' => '',
        'field_length' => 50,
        'field_unsigned' => 0,
        'field_description' => '',
        'editor_shown' => 1,
        'editor_style' => 'width: 300px;',
        'editor_help' => '',
        'editor_values_way' => 'enumerate',
        'editor_values_separator' => '|',
        'editor_values_group' => '',
        'editor_values' => '',
        'editor_validate' => '',
        'editor_filevalidate' => '',
        'editor_regexp' => '/[^\s]+/',
        'editor_regexp_error' => '显示名称为必填项，不能为空',
        'tablename' => $Category->tablename,
        'catename' => $Category->name
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '表名称',
        'fieldname' => 'tablename',
        'editor' => 'text',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 1,
        'weight' => 2,
        'field_type' => 'varchar',
        'field_size' => 'normal',
        'field_notnull' => 1,
        'field_default' => '',
        'field_length' => 50,
        'field_unsigned' => 0,
        'field_description' => '唯一表名称',
        'editor_shown' => 1,
        'editor_style' => 'width: 300px;',
        'editor_help' => '',
        'editor_values_way' => 'enumerate',
        'editor_values_separator' => '|',
        'editor_values_group' => '',
        'editor_values' => '',
        'editor_validate' => 'MiniFieldCategory|ValidateTableName',
        'editor_filevalidate' => '',
        'editor_regexp' => '/[^\s]+/',
        'editor_regexp_error' => '表名称为必填项，不能为空',
        'tablename' => $Category->tablename,
        'catename' => $Category->name
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '分组标记',
        'fieldname' => 'grouptag',
        'editor' => 'radio',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 1,
        'weight' => 2,
        'field_type' => 'varchar',
        'field_size' => 'normal',
        'field_notnull' => 0,
        'field_default' => '',
        'field_length' => 50,
        'field_unsigned' => 0,
        'field_description' => '分组标记',
        'editor_shown' => 1,
        'editor_style' => '',
        'editor_help' => '',
        'editor_values_way' => 'dynamic',
        'editor_values_separator' => '|',
        'editor_values_group' => '',
        'editor_values' => 'MiniFieldRepository|GroupTags',
        'editor_validate' => '',
        'editor_filevalidate' => '',
        'editor_regexp' => '',
        'editor_regexp_error' => '',
        'tablename' => $Category->tablename,
        'catename' => $Category->name
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '权重',
        'fieldname' => 'weight',
        'editor' => 'textselect',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 1,
        'weight' => 3,
        'field_type' => 'int',
        'field_size' => 'normal',
        'field_notnull' => 1,
        'field_default' => 0,
        'field_length' => 50,
        'field_unsigned' => 0,
        'field_description' => '',
        'editor_shown' => 1,
        'editor_style' => 'width: 100px;',
        'editor_help' => '整数值.',
        'editor_values_way' => 'increase',
        'editor_values_separator' => '|',
        'editor_values_group' => '',
        'editor_values' => '1|50',
        'editor_validate' => '',
        'editor_filevalidate' => '',
        'editor_regexp' => '/^\d+$/',
        'editor_regexp_error' => '权重必须为非负整数',
        'tablename' => $Category->tablename,
        'catename' => $Category->name
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '表存储引擎',
        'fieldname' => 'table_type',
        'editor' => 'select',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 1,
        'weight' => 4,
        'field_type' => 'varchar',
        'field_size' => 'normal',
        'field_notnull' => 0,
        'field_default' => '',
        'field_length' => 50,
        'field_unsigned' => 0,
        'field_description' => '',
        'editor_shown' => 1,
        'editor_style' => '',
        'editor_help' => '',
        'editor_values_way' => 'dynamic',
        'editor_values_separator' => '|',
        'editor_values_group' => '',
        'editor_values' => 'MiniFieldRepository|TableTypes',
        'editor_validate' => '',
        'editor_filevalidate' => '',
        'editor_regexp' => '',
        'editor_regexp_error' => '',
        'tablename' => $Category->tablename,
        'catename' => $Category->name
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '表整理',
        'fieldname' => 'table_collation',
        'editor' => 'select',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 1,
        'weight' => 5,
        'field_type' => 'varchar',
        'field_size' => 'normal',
        'field_notnull' => 0,
        'field_default' => '',
        'field_length' => 50,
        'field_unsigned' => 0,
        'field_description' => '',
        'editor_shown' => 1,
        'editor_style' => '',
        'editor_help' => '',
        'editor_values_way' => 'dynamic',
        'editor_values_separator' => '|',
        'editor_values_group' => 'gptag',
        'editor_values' => 'MiniFieldRepository|TableCollations',
        'editor_validate' => '',
        'editor_filevalidate' => '',
        'editor_regexp' => '',
        'editor_regexp_error' => '',
        'tablename' => $Category->tablename,
        'catename' => $Category->name
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '表描述',
        'fieldname' => 'table_description',
        'editor' => 'textarea',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 0,
        'weight' => 6,
        'field_type' => 'text',
        'field_size' => 'normal',
        'field_notnull' => 0,
        'field_default' => '',
        'field_length' => 255,
        'field_unsigned' => 0,
        'field_description' => '',
        'editor_shown' => 1,
        'editor_style' => 'width: 300px;',
        'editor_help' => '',
        'editor_values_way' => 'enumerate',
        'editor_values_separator' => '|',
        'editor_values_group' => '',
        'editor_values' => '',
        'editor_validate' => '',
        'editor_filevalidate' => '',
        'editor_regexp' => '',
        'editor_regexp_error' => '',
        'tablename' => $Category->tablename,
        'catename' => $Category->name
      )
    );
    foreach($arrFields as $key=>$value){
      $arrFields[$key] = (object)($arrFields[$key]);
    }
    return $arrFields;
  }
}