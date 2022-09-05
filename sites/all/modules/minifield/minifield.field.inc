<?php
/**
 * @author Changfeng Ji <jichf@qq.com>
 */
final class MiniField extends MiniFieldData{
  public function __construct($cid, $id = null, $events = array()) {
    parent::__construct($cid, $id);
    $this->FieldModel['cid'] = $cid;
    if($id == 0 && isset($this->FieldModel['weight'])){
      $this->FieldModel['weight'] = (int)MiniFieldSql::LoadDataExpressionByTableField($this->Category->tablename, 'weight', 'MAX', array(array('cid', $cid))) + 1;
    }
  }
  public function InitStructure($cid, $id) {
    $this->Category = MiniFieldStructure::Category();
    $this->arrFields = MiniFieldStructure::arrFields();
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
      $Data = self::LoadDataByCidId($this->FieldModel['cid'], $this->FieldModel['id'], false);
    }
    $result = parent::SaveDataToDB();
    if(is_array($result)){
      return $result;
    }
    $table = minifield_tablename_load($this->FieldModel['cid'], true);
    if(!empty($table) && db_table_exists($table)){
      $field = $this->FieldModel['fieldname'];
      $spec = array(
        'type' => $this->FieldModel['field_type'],
        'size' => $this->FieldModel['field_size'],
        'not null' => $this->FieldModel['field_notnull'] == 1 ? TRUE : FALSE,
        'default' => $this->FieldModel['field_default'],
        'length' => $this->FieldModel['field_length'],
        'unsigned' => $this->FieldModel['field_unsigned'] == 1 ? TRUE : FALSE);
      if($spec['type'] == 'serial' || $spec['type'] == 'text' || $spec['type'] == 'blob'){ unset($spec['default']); }
      if($spec['type'] == 'float' && !is_numeric($spec['default'])){ unset($spec['default']); }
      if(($spec['type'] == 'int' || $spec['type'] == 'numeric') && preg_match('/^-?\d+$/', $spec['default']) == 0){ unset($spec['default']); }
      if(!in_array(MiniFieldStructure::$DatatypeMaps[$spec['type'].':'.$spec['size']], array('CHAR', 'VARCHAR', 'TEXT'))){
        unset($spec['length']);
      }
      if(isset($Data->id)){
        if(db_field_exists($table, $Data->fieldname)){
          db_change_field($table, $Data->fieldname, $field, $spec);
        }else{
          if(!db_field_exists($table, $field)){
            db_add_field($table, $field, $spec);
          }
        }
      }else{
        if(!db_field_exists($table, $field)){
          db_add_field($table, $field, $spec);
        }
      }
    }
    MiniFieldStatic::ClearStatic('minifield_fieldname_load');
    MiniFieldStatic::ClearStatic('minifield_id_load');
    MiniFieldStatic::ClearStatics(__CLASS__);
    return $result;
  }
  
  /* static start */
  public static function ValidateCategoryId(MiniFieldData $mfd){
    $errors = array();
    $tablename = minifield_tablename_load($mfd->FieldModel['cid'], true);
    if(empty($tablename)){
      $errors['cid'] = ts('所属字段类别标识 %cid 不存在', 'ucfirst', 'minifield', array('%cid' => $mfd->FieldModel['cid']));
    }
    return $errors;
  }
  public static function ValidateFieldName(MiniFieldData $mfd){
    $errors = array();
    $CurrentTN = $mfd->Category->tablename;
    $rc = MiniFieldSql::LoadDataCountByTableField($CurrentTN, 'fieldname', array(array('fieldname', $mfd->FieldModel['fieldname']), array('cid', $mfd->FieldModel['cid']), array('id', $mfd->FieldModel['id'] , '!=')));
    if($rc > 0){
      $errors['fieldname'] = ts('字段名称 %fieldname 已存在，不能重复', 'ucfirst', 'minifield', array('%fieldname' => $mfd->FieldModel['fieldname']));
    }
    return $errors;
  }
  public static function ValidateFieldSize(MiniFieldData $mfd){
    $errors = array();
    if(!isset(MiniFieldStructure::$DatatypeMaps[$mfd->FieldModel['field_type'].':'.$mfd->FieldModel['field_size']])){
      $errors['field_size'] = ts('字段类型 %fieldtype 与字段大小 %fieldsize 不匹配', 'ucfirst', 'minifield', array('%fieldtype' => $mfd->FieldModel['field_type'], '%fieldsize' => $mfd->FieldModel['field_size'])) . '[<a target="_blank" href="http://drupal.org/node/159605">Data Types and Sizes are explained here</a>]';
    }
    return $errors;
  }
  public static function ValidateFieldDefault(MiniFieldData $mfd){
    $errors = array();
    if(($mfd->FieldModel['field_type'] == 'int' || $mfd->FieldModel['field_type'] == 'numeric') && preg_match('/^-?\d+$/', $mfd->FieldModel['field_default']) == 0){
      $errors['field_default'] = ts('当字段类型为 int、numeric 时，字段默认值必须为整数', 'ucfirst', 'minifield');
    }else if($mfd->FieldModel['field_type'] == 'float' && !is_numeric($mfd->FieldModel['field_default'])){
      $errors['field_default'] = ts('当字段类型为 float 时，字段默认值必须为数值', 'ucfirst', 'minifield');
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
   *     id = 0, cid = 0, name = '', fieldname = '', editor = 'text', enabled = 1, required = 0, spotlight = 0, weight = 1,
   *     field_type = 'varchar', field_size = 'normal', field_notnull = 0, field_default = '', field_length = 50, field_unsigned = 0, field_description = '',
   *     editor_shown = 1, editor_style = '', editor_help = '', editor_values_way = 'enumerate', editor_values_separator = '|', editor_values_group = '', editor_values = '',
   *     editor_validate = '', editor_filevalidate = '', editor_regexp = '', editor_regexp_error = ''.
   * @return int/array :
   *   - The last insert/update ID of the query, if successfully.
   *   - An associative array of reasons for failure, where the keys are the field name and the values are the values those field name must have, if failed.
   */
  public static function SaveData($variables = array()){
    $backupPOST = $_POST;
    $variables_default = array(
      'id' => 0,
      'cid' => 0,
      'name' => '',
      'fieldname' => '',
      'editor' => 'text', 'switchofeditor' => '',
      'enabled' => 1, 'switchofenabled' => '',
      'required' => 0, 'switchofrequired' => '',
      'spotlight' => 0, 'switchofspotlight' => '',
      'weight' => 1,
      'field_type' => 'varchar', 'switchoffield_type' => '',
      'field_size' => 'normal', 'switchoffield_size' => '',
      'field_notnull' => 0, 'switchoffield_notnull' => '',
      'field_default' => '',
      'field_length' => 50,
      'field_unsigned' => 0, 'switchoffield_unsigned' => '',
      'field_description' => '',
      'editor_shown' => 1, 'switchofeditor_shown' => '',
      'editor_style' => '',
      'editor_help' => '',
      'editor_values_way' => 'enumerate', 'switchofeditor_values_way' => '',
      'editor_values_separator' => '|',
      'editor_values_group' => '',
      'editor_values' => '',
      'editor_validate' => '',
      'editor_filevalidate' => '',
      'editor_regexp' => '', 'switchofeditor_regexp' => '',
      'editor_regexp_error' => ''
    );
    $_POST = array_merge($variables_default, $variables);
    $mf = new MiniField($_POST['cid'], $_POST['id']);
    if($mf->IsExists == true){
      foreach($variables as $key => $value){
        if(isset($variables_default['switchof' . $key])){
          $variables['switchof' . $key] = '';
        }
      }
      $_POST = $variables;
    }
    $result = $mf->SaveDataToDB();
    $_POST = $backupPOST;
    return $result;
  }
  
  /**
   * @access public
   * @static
   * @param int $cid The minifield category identifier.
   * @param type $limit The page limit or null.
   * @param boolean $format whether to format loaded data or not.
   * @param int $element An optional integer to distinguish between multiple pagers on one page.
   * @param boolean $invokehook whether to invoke hook hook_minifielddata_load_$cid($arrDatas = array()).
   * @return array The loaded data array. 
   */
  public static function LoadDatasByCid($cid, $limit = null, $format = false, $element = 0, $invokehook = true) {
    $StaticName = __CLASS__ . '_' . __FUNCTION__ . '_' . $cid . '_' . ($format ? 1 : 0);
    if(is_null($limit)){
      $arrDatas = MiniFieldStatic::GetStatic($StaticName, true);
      if(isset($arrDatas)){
        return $arrDatas;
      }
    }
    $Category1 = MiniFieldStructure::Category();
    if(empty($Category1->tablename)){ return array(); }
    $Category2 = MiniFieldCategoryStructure::Category();
    if(empty($Category2->tablename)){ return array(); }
    $tables = array(
      array($Category1->tablename, '', 'cid', array(), array()),
      array($Category2->tablename, 'INNER', 'id', array('tablename', 'table_type'), array('id' => 'cateid', 'name' => 'catename'))
    );
    $conditions = array();
    $conditions[] = array('cid', $cid);
    $orderbys = array();
    $orderbys[] = array($Category1->tablename . '.weight', 'ASC');
    $orderbys[] = array($Category1->tablename . '.id', 'ASC');
    $arrDatas = MiniFieldSql::FilterDatasByTables($tables, $conditions, $orderbys, $limit, $element);
    if($format == true){
      $arrFields = MiniFieldStructure::arrFields();
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
   * @param int $cid The minifield category identifier.
   * @param int $id The minifield data identifier.
   * @param boolean $format whether to format loaded data or not.
   * @param boolean $invokehook whether to invoke hook hook_minifielddata_load_$cid($arrDatas = array()).
   * @return object The loaded data object.
   */
  public static function LoadDataByCidId($cid, $id, $format = false, $invokehook = true) {
    $StaticName = __CLASS__ . '_' . __FUNCTION__ . '_' . $cid . '_' . $id . '_' . ($format ? 1 : 0);
    $Data = MiniFieldStatic::GetStatic($StaticName, true);
    if(isset($Data)){
      return $Data;
    }
    $Category1 = MiniFieldStructure::Category();
    if(empty($Category1->tablename)){ return null; }
    $Category2 = MiniFieldCategoryStructure::Category();
    if(empty($Category2->tablename)){ return null; }
    $tables = array(
      array($Category1->tablename, '', 'cid', array(), array()),
      array($Category2->tablename, 'INNER', 'id', array('tablename', 'table_type'), array('id' => 'cateid', 'name' => 'catename'))
    );
    $conditions = array();
    $conditions[] = array('cid', $cid);
    $conditions[] = array($Category1->tablename . '.id', $id);
    $Data = MiniFieldSql::FilterDatasByTables($tables, $conditions, array(), null, 0, array('fetchObject'));
    if($format == true){
      $arrFields = MiniFieldStructure::arrFields();
      $Data = self::FormatData($Data, $arrFields);
    }
    MiniFieldStatic::SetStatic($StaticName, $Data, true);
    //if($invokehook == true){ module_invoke_all('minifielddata_load_' . $cid, array($Data)); }
    return $Data;
  }
  /**
   * @access public
   * @static
   * @param int $cid The minifield category identifier.
   * @param int $id The minifield data identifier.
   * @param boolean $includefiles whether to delete related files or not.
   * @param boolean $invokehook whether to invoke hook hook_minifielddata_delete_$cid($ides = array()).
   */
  public static function DeleteDataByCidId($cid, $id, $includefiles = false, $invokehook = true) {
    $Category = MiniFieldStructure::Category();
    if(empty($Category->tablename)){ return; }
    //if($invokehook == true){ module_invoke_all('minifielddata_delete_' . $cid, array($id)); }
    $includefield = true;
    if($includefield == true){
      $Data = self::LoadDataByCidId($cid, $id, false);
      if(isset($Data->id) && db_field_exists($Data->tablename, $Data->fieldname)){
        db_drop_field($Data->tablename, $Data->fieldname);
      }
    }
    if($includefiles == true){
      $arrFields = MiniFieldStructure::arrFields();
      $Data = self::LoadDataByCidId($cid, $id, false);
      self::DeleteDataFiles($Data, $arrFields);
    }
    MiniFieldSql::DeleteDataByTableId($Category->tablename, $id);
    MiniFieldStatic::ClearStatic('minifield_fieldname_load');
    MiniFieldStatic::ClearStatic('minifield_id_load');
    MiniFieldStatic::ClearStatics(__CLASS__);
  }
  /**
   * Filter datas by minifield category identifier
   * @access public
   * @static
   * @param int $cid The minifield category identifier.
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
    $Category1 = MiniFieldStructure::Category();
    if(empty($Category1->tablename)){ return array(); }
    $Category2 = MiniFieldCategoryStructure::Category();
    if(empty($Category2->tablename)){ return array(); }
    $tables = array(
      array($Category1->tablename, '', 'cid', $fields, array()),
      array($Category2->tablename, 'INNER', 'id', array('tablename', 'table_type'), array('id' => 'cateid', 'name' => 'catename'))
    );
    $conditions[] = array('cid', $cid);
    $orderbys[] = array($Category1->tablename . '.weight', 'ASC');
    $orderbys[] = array($Category1->tablename . '.id', 'ASC');
    $arrDatas = MiniFieldSql::FilterDatasByTables($tables, $conditions, $orderbys, $limit, $element, $fetchmode, $callback);
    if($format == true && isset($fetchmode[0]) && in_array($fetchmode[0], array('fetchAll', 'fetchAllAssoc'))){
      $arrFields = MiniFieldStructure::arrFields();
      $arrDatas = MiniFieldData::FormatDatas($arrDatas, $arrFields, $fields_alias);
    }else if($format == true && isset($fetchmode[0]) && $fetchmode[0] == 'fetchObject'){
      $arrFields = MiniFieldStructure::arrFields();
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

final class MiniFieldStructure{
  public static $DatatypeMaps = array(
    'varchar:normal'  => 'VARCHAR',
    'char:normal'     => 'CHAR',

    'text:tiny'       => 'TINYTEXT',
    'text:small'      => 'TINYTEXT',
    'text:medium'     => 'MEDIUMTEXT',
    'text:big'        => 'LONGTEXT',
    'text:normal'     => 'TEXT',

    'serial:tiny'     => 'TINYINT',
    'serial:small'    => 'SMALLINT',
    'serial:medium'   => 'MEDIUMINT',
    'serial:big'      => 'BIGINT',
    'serial:normal'   => 'INT',

    'int:tiny'        => 'TINYINT',
    'int:small'       => 'SMALLINT',
    'int:medium'      => 'MEDIUMINT',
    'int:big'         => 'BIGINT',
    'int:normal'      => 'INT',

    'float:tiny'      => 'FLOAT',
    'float:small'     => 'FLOAT',
    'float:medium'    => 'FLOAT',
    'float:big'       => 'DOUBLE',
    'float:normal'    => 'FLOAT',

    'numeric:normal'  => 'DECIMAL',

    'blob:big'        => 'LONGBLOB',
    'blob:normal'     => 'BLOB',
  );
  public static function Category(){
    $Category = array(
      'id' => 0,
      'name' => '字段',
      'tablename' => 'minifield',
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
        'name' => '所属字段类别',
        'fieldname' => 'cid',
        'editor' => 'hidden',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 0,
        'weight' => 1,
        'field_type' => 'int',
        'field_size' => 'normal',
        'field_notnull' => 1,
        'field_default' => 0,
        'field_length' => 50,
        'field_unsigned' => 0,
        'field_description' => '',
        'editor_shown' => 0,
        'editor_style' => '',
        'editor_help' => '所属字段类别标识',
        'editor_values_way' => 'enumerate',
        'editor_values_separator' => '|',
        'editor_values_group' => '',
        'editor_values' => '',
        'editor_validate' => 'MiniField|ValidateCategoryId',
        'editor_filevalidate' => '',
        'editor_regexp' => '/^[0-9]*[1-9][0-9]*$/',
        'editor_regexp_error' => '字段类别标识必须为正整数',
        'tablename' => $Category->tablename,
        'catename' => $Category->name
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '显示名称',
        'fieldname' => 'name',
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
        'field_description' => '显示名称',
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
        'name' => '字段名称',
        'fieldname' => 'fieldname',
        'editor' => 'text',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 1,
        'weight' => 3,
        'field_type' => 'varchar',
        'field_size' => 'normal',
        'field_notnull' => 1,
        'field_default' => '',
        'field_length' => 50,
        'field_unsigned' => 0,
        'field_description' => '唯一字段名称',
        'editor_shown' => 1,
        'editor_style' => 'width: 300px;',
        'editor_help' => '唯一字段名称',
        'editor_values_way' => 'enumerate',
        'editor_values_separator' => '|',
        'editor_values_group' => '',
        'editor_values' => '',
        'editor_validate' => 'MiniField|ValidateFieldName',
        'editor_filevalidate' => '',
        'editor_regexp' => '/[^\s]+/',
        'editor_regexp_error' => '字段名称为必填项，不能为空',
        'tablename' => $Category->tablename,
        'catename' => $Category->name
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '编辑控件类型',
        'fieldname' => 'editor',
        'editor' => 'select',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 1,
        'weight' => 4,
        'field_type' => 'varchar',
        'field_size' => 'normal',
        'field_notnull' => 1,
        'field_default' => '',
        'field_length' => 50,
        'field_unsigned' => 0,
        'field_description' => '编辑控件类型',
        'editor_shown' => 1,
        'editor_style' => '',
        'editor_help' => '',
        'editor_values_way' => 'dynamic',
        'editor_values_separator' => '|',
        'editor_values_group' => 'gptag',
        'editor_values' => 'MiniFieldRepository|HtmlEditors',
        'editor_validate' => '',
        'editor_filevalidate' => '',
        'editor_regexp' => '/[^\s]+/',
        'editor_regexp_error' => '编辑控件类型为必选项，不能为空',
        'tablename' => $Category->tablename,
        'catename' => $Category->name
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '是否启用',
        'fieldname' => 'enabled',
        'editor' => 'radio',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 1,
        'weight' => 5,
        'field_type' => 'int',
        'field_size' => 'normal',
        'field_notnull' => 1,
        'field_default' => 0,
        'field_length' => 50,
        'field_unsigned' => 0,
        'field_description' => '是否启用',
        'editor_shown' => 1,
        'editor_style' => '',
        'editor_help' => '',
        'editor_values_way' => 'enumerate',
        'editor_values_separator' => '|',
        'editor_values_group' => '',
        'editor_values' => MiniFieldCommon::JoinedByNewline(array('1|<span style="color: green;">是</span>||1', '0|<span style="color: red;">否</span>|')),
        'editor_validate' => '',
        'editor_filevalidate' => '',
        'editor_regexp' => '/[^\s]+/',
        'editor_regexp_error' => '是否启用为必选项，不能为空',
        'tablename' => $Category->tablename,
        'catename' => $Category->name
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '是否必须',
        'fieldname' => 'required',
        'editor' => 'radio',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 1,
        'weight' => 6,
        'field_type' => 'int',
        'field_size' => 'normal',
        'field_notnull' => 1,
        'field_default' => 0,
        'field_length' => 50,
        'field_unsigned' => 0,
        'field_description' => '是否必须',
        'editor_shown' => 1,
        'editor_style' => '',
        'editor_help' => '',
        'editor_values_way' => 'enumerate',
        'editor_values_separator' => '|',
        'editor_values_group' => '',
        'editor_values' => MiniFieldCommon::JoinedByNewline(array('1|<span style="color: green;">是</span>|', '0|<span style="color: red;">否</span>||1')),
        'editor_validate' => '',
        'editor_filevalidate' => '',
        'editor_regexp' => '/[^\s]+/',
        'editor_regexp_error' => '是否必须为必选项，不能为空',
        'tablename' => $Category->tablename,
        'catename' => $Category->name
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '是否醒目',
        'fieldname' => 'spotlight',
        'editor' => 'radio',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 1,
        'weight' => 7,
        'field_type' => 'int',
        'field_size' => 'normal',
        'field_notnull' => 1,
        'field_default' => 0,
        'field_length' => 50,
        'field_unsigned' => 0,
        'field_description' => '是否醒目',
        'editor_shown' => 1,
        'editor_style' => '',
        'editor_help' => '',
        'editor_values_way' => 'enumerate',
        'editor_values_separator' => '|',
        'editor_values_group' => '',
        'editor_values' => MiniFieldCommon::JoinedByNewline(array('1|<span style="color: green;">是</span>|', '0|<span style="color: red;">否</span>||1')),
        'editor_validate' => '',
        'editor_filevalidate' => '',
        'editor_regexp' => '/[^\s]+/',
        'editor_regexp_error' => '是否醒目为必选项，不能为空',
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
        'weight' => 8,
        'field_type' => 'int',
        'field_size' => 'normal',
        'field_notnull' => 1,
        'field_default' => 0,
        'field_length' => 50,
        'field_unsigned' => 0,
        'field_description' => '权重',
        'editor_shown' => 1,
        'editor_style' => 'width: 100px;',
        'editor_help' => '整数值.',
        'editor_values_way' => 'increase',
        'editor_values_separator' => '|',
        'editor_values_group' => '',
        'editor_values' => '1|100',
        'editor_validate' => '',
        'editor_filevalidate' => '',
        'editor_regexp' => '/^\d+$/',
        'editor_regexp_error' => '权重必须为非负整数',
        'tablename' => $Category->tablename,
        'catename' => $Category->name
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '字段类型',
        'fieldname' => 'field_type',
        'editor' => 'select',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 1,
        'weight' => 9,
        'field_type' => 'varchar',
        'field_size' => 'normal',
        'field_notnull' => 0,
        'field_default' => '',
        'field_length' => 50,
        'field_unsigned' => 0,
        'field_description' => '字段类型',
        'editor_shown' => 1,
        'editor_style' => '',
        'editor_help' => '',
        'editor_values_way' => 'dynamic',
        'editor_values_separator' => '|',
        'editor_values_group' => 'gptag',
        'editor_values' => 'MiniFieldRepository|FieldTypes',
        'editor_validate' => '',
        'editor_filevalidate' => '',
        'editor_regexp' => '/[^\s]+/',
        'editor_regexp_error' => '字段类型为必选项，不能为空',
        'tablename' => $Category->tablename,
        'catename' => $Category->name
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '字段大小',
        'fieldname' => 'field_size',
        'editor' => 'select',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 0,
        'weight' => 10,
        'field_type' => 'varchar',
        'field_size' => 'normal',
        'field_notnull' => 0,
        'field_default' => '',
        'field_length' => 50,
        'field_unsigned' => 0,
        'field_description' => '字段大小',
        'editor_shown' => 1,
        'editor_style' => '',
        'editor_help' => '<a target="_blank" href="http://drupal.org/node/159605">Data Types and Sizes are explained here</a>',
        'editor_values_way' => 'dynamic',
        'editor_values_separator' => '|',
        'editor_values_group' => '',
        'editor_values' => 'MiniFieldRepository|FieldSizes',
        'editor_validate' => 'MiniField|ValidateFieldSize',
        'editor_filevalidate' => '',
        'editor_regexp' => '/[^\s]+/',
        'editor_regexp_error' => '字段大小为必选项，不能为空',
        'tablename' => $Category->tablename,
        'catename' => $Category->name
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '字段不为空',
        'fieldname' => 'field_notnull',
        'editor' => 'radio',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 0,
        'weight' => 11,
        'field_type' => 'int',
        'field_size' => 'normal',
        'field_notnull' => 1,
        'field_default' => 0,
        'field_length' => 50,
        'field_unsigned' => 0,
        'field_description' => '字段不为空',
        'editor_shown' => 1,
        'editor_style' => '',
        'editor_help' => '',
        'editor_values_way' => 'enumerate',
        'editor_values_separator' => '|',
        'editor_values_group' => '',
        'editor_values' => MiniFieldCommon::JoinedByNewline(array('1|是|', '0|否||1')),
        'editor_validate' => '',
        'editor_filevalidate' => '',
        'editor_regexp' => '',
        'editor_regexp_error' => '',
        'tablename' => $Category->tablename,
        'catename' => $Category->name
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '字段默认值',
        'fieldname' => 'field_default',
        'editor' => 'text',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 0,
        'weight' => 12,
        'field_type' => 'varchar',
        'field_size' => 'normal',
        'field_notnull' => 0,
        'field_default' => '',
        'field_length' => 50,
        'field_unsigned' => 0,
        'field_description' => '字段默认值',
        'editor_shown' => 1,
        'editor_style' => 'width: 300px;',
        'editor_help' => "The field's default value. Note that type 'text' and 'blob' fields cannot have default values.",
        'editor_values_way' => 'enumerate',
        'editor_values_separator' => '|',
        'editor_values_group' => '',
        'editor_values' => '',
        'editor_validate' => 'MiniField|ValidateFieldDefault',
        'editor_filevalidate' => '',
        'editor_regexp' => '',
        'editor_regexp_error' => '',
        'tablename' => $Category->tablename,
        'catename' => $Category->name
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '字段长度',
        'fieldname' => 'field_length',
        'editor' => 'text',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 0,
        'weight' => 13,
        'field_type' => 'int',
        'field_size' => 'normal',
        'field_notnull' => 0,
        'field_default' => 50,
        'field_length' => 50,
        'field_unsigned' => 0,
        'field_description' => '字段长度',
        'editor_shown' => 1,
        'editor_style' => 'width: 300px;',
        'editor_help' => "The maximal length of a type 'char', 'varchar' or 'text' field. Ignored for other field types.",
        'editor_values_way' => 'enumerate',
        'editor_values_separator' => '|',
        'editor_values_group' => '',
        'editor_values' => '',
        'editor_validate' => '',
        'editor_filevalidate' => '',
        'editor_regexp' => '/^\d+$/',
        'editor_regexp_error' => '字段长度必须为非负整数',
        'tablename' => $Category->tablename,
        'catename' => $Category->name
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '字段无符号',
        'fieldname' => 'field_unsigned',
        'editor' => 'radio',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 0,
        'weight' => 14,
        'field_type' => 'int',
        'field_size' => 'normal',
        'field_notnull' => 1,
        'field_default' => 0,
        'field_length' => 50,
        'field_unsigned' => 0,
        'field_description' => '字段无符号',
        'editor_shown' => 1,
        'editor_style' => '',
        'editor_help' => "A boolean indicating whether a type 'int', 'float' and 'numeric' only is signed or unsigned. Defaults to FALSE. Ignored for other field types.",
        'editor_values_way' => 'enumerate',
        'editor_values_separator' => '|',
        'editor_values_group' => '',
        'editor_values' => MiniFieldCommon::JoinedByNewline(array('1|是|', '0|否||1')),
        'editor_validate' => '',
        'editor_filevalidate' => '',
        'editor_regexp' => '/[^\s]+/',
        'editor_regexp_error' => '字段无符号为必选项，不能为空',
        'tablename' => $Category->tablename,
        'catename' => $Category->name
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '字段描述',
        'fieldname' => 'field_description',
        'editor' => 'textarea',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 0,
        'weight' => 15,
        'field_type' => 'text',
        'field_size' => 'normal',
        'field_notnull' => 0,
        'field_default' => '',
        'field_length' => 255,
        'field_unsigned' => 0,
        'field_description' => '字段描述',
        'editor_shown' => 1,
        'editor_style' => 'width: 300px;',
        'editor_help' => "A string describing this field and its purpose.",
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
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '显示状态',
        'fieldname' => 'editor_shown',
        'editor' => 'radio',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 1,
        'weight' => 16,
        'field_type' => 'int',
        'field_size' => 'normal',
        'field_notnull' => 1,
        'field_default' => 0,
        'field_length' => 50,
        'field_unsigned' => 0,
        'field_description' => '编辑控件显示状态',
        'editor_shown' => 1,
        'editor_style' => '',
        'editor_help' => "是否显示编辑控件",
        'editor_values_way' => 'enumerate',
        'editor_values_separator' => '|',
        'editor_values_group' => '',
        'editor_values' => MiniFieldCommon::JoinedByNewline(array('1|显示||1', '0|隐藏|')),
        'editor_validate' => '',
        'editor_filevalidate' => '',
        'editor_regexp' => '/[^\s]+/',
        'editor_regexp_error' => '显示状态为必选项，不能为空',
        'tablename' => $Category->tablename,
        'catename' => $Category->name
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '样式',
        'fieldname' => 'editor_style',
        'editor' => 'text',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 0,
        'weight' => 17,
        'field_type' => 'varchar',
        'field_size' => 'normal',
        'field_notnull' => 0,
        'field_default' => '',
        'field_length' => 255,
        'field_unsigned' => 0,
        'field_description' => '编辑控件样式',
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
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '帮助',
        'fieldname' => 'editor_help',
        'editor' => 'text',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 0,
        'weight' => 18,
        'field_type' => 'varchar',
        'field_size' => 'normal',
        'field_notnull' => 0,
        'field_default' => '',
        'field_length' => 255,
        'field_unsigned' => 0,
        'field_description' => '编辑控件帮助',
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
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '可选值录入方式',
        'fieldname' => 'editor_values_way',
        'editor' => 'radio',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 0,
        'weight' => 19,
        'field_type' => 'varchar',
        'field_size' => 'normal',
        'field_notnull' => 0,
        'field_default' => '',
        'field_length' => 50,
        'field_unsigned' => 0,
        'field_description' => '编辑控件可选值录入方式',
        'editor_shown' => 1,
        'editor_style' => '',
        'editor_help' => '',
        'editor_values_way' => 'enumerate',
        'editor_values_separator' => '|',
        'editor_values_group' => '',
        'editor_values' => MiniFieldCommon::JoinedByNewline(array('enumerate|枚举||1', 'increase|递增|', 'dynamic|动态|')),
        'editor_validate' => '',
        'editor_filevalidate' => '',
        'editor_regexp' => '/[^\s]+/',
        'editor_regexp_error' => '编辑控件可选值录入方式为必选项，不能为空',
        'tablename' => $Category->tablename,
        'catename' => $Category->name
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '可选值分隔符',
        'fieldname' => 'editor_values_separator',
        'editor' => 'text',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 0,
        'weight' => 20,
        'field_type' => 'varchar',
        'field_size' => 'normal',
        'field_notnull' => 0,
        'field_default' => '|',
        'field_length' => 50,
        'field_unsigned' => 0,
        'field_description' => '编辑控件可选值分隔符',
        'editor_shown' => 1,
        'editor_style' => 'width: 200px;',
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
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '可选值分组标记',
        'fieldname' => 'editor_values_group',
        'editor' => 'text',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 0,
        'weight' => 21,
        'field_type' => 'varchar',
        'field_size' => 'normal',
        'field_notnull' => 0,
        'field_default' => '',
        'field_length' => 50,
        'field_unsigned' => 0,
        'field_description' => '编辑控件可选值分组标记',
        'editor_shown' => 1,
        'editor_style' => 'width: 200px;',
        'editor_help' => '开始分组标记：{分组标记}_begin；结束分组标记：{分组标记}_end',
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
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '可选值',
        'fieldname' => 'editor_values',
        'editor' => 'textarea',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 0,
        'weight' => 22,
        'field_type' => 'text',
        'field_size' => 'normal',
        'field_notnull' => 0,
        'field_default' => '',
        'field_length' => 255,
        'field_unsigned' => 0,
        'field_description' => '编辑控件可选值',
        'editor_shown' => 1,
        'editor_style' => 'width: 300px;',
        'editor_help' => '一行代表一个可选值（录入方式为递增或动态时，仅第一行有效）<br />录入方式为枚举时：{值}/{开始分组标记}/{结束分组标记}[{分隔符}{别名}{分隔符}{说明}{分隔符}{默认值标记(1/0)}]<br />录入方式为递增时：{开始数值}{分隔符}{结束数值}<br />录入方式为动态时：{class}{分隔符}{function}[{分隔符}{arguments1}……{分隔符}{arguments*}]',
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
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '验证',
        'fieldname' => 'editor_validate',
        'editor' => 'text',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 0,
        'weight' => 23,
        'field_type' => 'varchar',
        'field_size' => 'normal',
        'field_notnull' => 0,
        'field_default' => '',
        'field_length' => 255,
        'field_unsigned' => 0,
        'field_description' => '编辑控件验证',
        'editor_shown' => 1,
        'editor_style' => 'width: 300px;',
        'editor_help' => '验证：{class}|{function}[|{arguments1}……|{arguments*}]',
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
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '文件验证',
        'fieldname' => 'editor_filevalidate',
        'editor' => 'textarea',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 0,
        'weight' => 24,
        'field_type' => 'text',
        'field_size' => 'normal',
        'field_notnull' => 0,
        'field_default' => '',
        'field_length' => 255,
        'field_unsigned' => 0,
        'field_description' => '编辑控件文件验证',
        'editor_shown' => 1,
        'editor_style' => 'width: 300px;',
        'editor_help' => '一行代表一个验证：{class}|{function}[|{arguments1}……|{arguments*}]<br />验证文件名最大长度：MiniFieldFile|FileValidateNameLength|{最大长度(数值)}<br />验证文件名允许扩展类型：MiniFieldFile|FileValidateExtensions|{允许扩展类型(空格间隔)}<br />验证文件最大大小：MiniFieldFile|FileValidateSize|{文件尺寸(数值g|G|m|M|k|K)}<br />验证文件是否是图片：MiniFieldFile|FileValidateIsImage<br />验证文件分辨率：MiniFieldFile|FileValidateImageResolution|{最大尺寸(宽x高)}|{最小尺寸(宽x高)}|{裁切方式(image_scale/image_scale_and_crop/image_crop/)}|{裁切坐标(横坐标x纵坐标)}',
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
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '正则表达式匹配',
        'fieldname' => 'editor_regexp',
        'editor' => 'select',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 0,
        'weight' => 25,
        'field_type' => 'varchar',
        'field_size' => 'normal',
        'field_notnull' => 0,
        'field_default' => '',
        'field_length' => 255,
        'field_unsigned' => 0,
        'field_description' => '编辑控件正则表达式匹配',
        'editor_shown' => 1,
        'editor_style' => '',
        'editor_help' => '',
        'editor_values_way' => 'dynamic',
        'editor_values_separator' => '|',
        'editor_values_group' => 'gptag',
        'editor_values' => 'MiniFieldRepository|RegularExpressions',
        'editor_validate' => '',
        'editor_filevalidate' => '',
        'editor_regexp' => '',
        'editor_regexp_error' => '',
        'tablename' => $Category->tablename,
        'catename' => $Category->name
      ),array(
        'id' => 0,
        'cid' => $Category->id,
        'name' => '正则表达式匹配失败提醒语句',
        'fieldname' => 'editor_regexp_error',
        'editor' => 'text',
        'enabled' => 1,
        'required' => 1,
        'spotlight' => 0,
        'weight' => 26,
        'field_type' => 'varchar',
        'field_size' => 'normal',
        'field_notnull' => 0,
        'field_default' => '',
        'field_length' => 255,
        'field_unsigned' => 0,
        'field_description' => '编辑控件正则表达式匹配失败提醒语句',
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