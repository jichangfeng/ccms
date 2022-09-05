<?php
/**
 * @author Changfeng Ji <jichf@qq.com>
 */
class MiniFieldSql{
  /**
   * Convert query object to sql string.
   * @access public
   * @static
   * @param object $query A query object. such as:
   *  - $query = db_select('users', 'users')->fields('users', array('uid', 'name', 'mail'))->condition('uid', 1, '>')->orderBy('uid', 'ASC')->distinct();
   *  - $query = db_insert('users')->fields(array('name' => 'coco', 'pass' => 'coco', 'mail' => 'jichf@qq.com'));
   *  - $query = db_update('users')->fields(array('name' => 'coco', 'pass' => 'coco', 'mail' => 'jichf@qq.com'))->condition('uid', 1);
   *  - $query = db_truncate('users');
   * @return string The sql string.
   */
  public static function toSqlString($query){
    return $query->__toString();
  }
  /**
   * Executes the TRUNCATE query.
   * @access public
   * @static
   * @param string $table The table from which to delete.
   * @param array $options An array of options to control how the query operates.
   * @return Return value is dependent on the database type.
   */
  public static function Truncate($table, array $options = array()){
    return db_truncate($table, $options)->execute();
  }
  /**
   * @access public
   * @static
   * @param string $table base table for this query. May be a string or another SelectQuery object. If a query object is passed, it will be used as a subselect.
   * @param type $limit The page limit or null.
   * @param int $element An optional integer to distinguish between multiple pagers on one page.
   * @return array The loaded data array.
   */
  public static function LoadDatasByTable($table, $limit = null, $element = 0){
    if(is_numeric($limit) && $limit > 0){
      /*if(!is_numeric($element) || $element < 0){
        $element = 0;
        global $pager_page_array, $pager_limits, $pager_total, $pager_total_items;
        while(isset($pager_page_array[$element])){ $element++; }
      }*/
      $queryObj = db_select($table, $table)->extend('PagerDefault')->limit($limit)->element($element);
    }else{
      $queryObj = db_select($table, $table);
    }
    $queryObj->fields($table);
    return $queryObj->execute()->fetchAll();
  }
  /**
   * @access public
   * @static
   * @param string $table base table for this query. May be a string or another SelectQuery object. If a query object is passed, it will be used as a subselect.
   * @param int $id The data identifier.
   * @return object The loaded data object.
   */
  public static function LoadDataByTableId($table, $id){
    $queryObj = db_select($table, $table)->fields($table)->condition($table . '.id', $id);
    return $queryObj->execute()->fetchObject();
  }
  /**
   * @access public
   * @static
   * @param string $table table from which to delete.
   * @param int $id The data identifier.
   */
  final static function DeleteDataByTableId($table, $id){
    db_delete($table)->condition('id', $id)->execute();
  }

  /**
   * Insert data by table and fields.
   * @access final
   * @static
   * @param string $table The table into which to insert.
   * @param array $fields associative array of fields to write into the database. The array keys are the field names and the values are the values to which to set them.
   * @return int The last insert ID of the query, if one exists. If no fields are specified, this method will do nothing and return 0.
   */
  final static function InsertDataByTable($table, $fields){
    if(empty($fields)){
      return 0;
    }
    $queryObj = db_insert($table)->fields($fields);
    return $queryObj->execute();
  }
  /**
   * Update datas by table.
   * $fields and $expressions at least one is not empty.
   * @access final
   * @static
   * @param string $table The table to update.
   * @param array $fields associative array of fields to write into the database. The array keys are the field names and the values are the values to which to set them.
   * @param array $expressions The collection of expression.
   *      Each element is still a array with two to three elements($field, $expression[, $arguments]):
   *       - $field: The field to set.
   *       - $expression: The field will be set to the value of this expression. This parameter may include named placeholders.
   *       - $arguments: If specified, this is an array of key/value pairs for named placeholders corresponding to the expression.
   *      Example: $table = 'users', $expressions = array(array("uid", "uid + 1000"), array("name", "concat('prefix_', uid, '_', mail)"), array("pass", "md5(pass)")).
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
   * @return int The number of rows affected by the update.
   */
  final static function UpdateDatasByTable($table, $fields = array(), $expressions = array(), $conditions = array()){
    if(empty($fields) && empty($expressions)){
      return 0;
    }
    $queryObj = db_update($table)->fields($fields);
    self::ProcessExpressions($queryObj, $expressions);
    self::ProcessConditions($queryObj, $conditions);
    return $queryObj->execute();
  }
  /**
   * Merge datas by table(Dynamic query is to execute insert or update command).
   * 
   * $fields and $expressions at least one can not be empty.
   * $conditions can not be empty.
   * 
   * $fields is required if an INSERT query has been executed.
   * $expressions is invalid if an INSERT query has been executed.
   * @access final
   * @static
   * @param string $table The table into which to merge.
   * @param array $fields associative array of fields to write into the database. The array keys are the field names and the values are the values to which to set them.
   * @param array $expressions The collection of expression.
   *      Each element is still a array with two to three elements($field, $expression[, $arguments]):
   *       - $field: The field to set.
   *       - $expression: The field will be set to the value of this expression. This parameter may include named placeholders.
   *       - $arguments: If specified, this is an array of key/value pairs for named placeholders corresponding to the expression.
   *      Example: $table = 'users', $expressions = array(array("uid", "uid + 1000"), array("name", "concat('prefix_', uid, '_', mail)"), array("pass", "md5(pass)")).
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
   * @return int
   *   Return the merge query status on success:
   *    - Return <i>MergeQuery::STATUS_INSERT (int 1)</i> if an INSERT query has been executed.
   *    - Return <i>MergeQuery::STATUS_UPDATE (int 2)</i> if an UPDATE query has been executed.
   */
  public static function MergeDatasByTable($table, $fields = array(), $expressions = array(), $conditions = array()){
    if(empty($fields) && empty($expressions)){
      return 0;
    }
    if(empty($conditions)){
      return 0;
    }
    $queryObj = db_merge($table)->fields($fields);
    self::ProcessExpressions($queryObj, $expressions);
    self::ProcessConditions($queryObj, $conditions);
    return $queryObj->execute();
  }

  /**
   * Filter datas by table
   * @access public
   * @static
   * @param string $table base table for this query. May be a string or another SelectQuery object. If a query object is passed, it will be used as a subselect.
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
  public static function FilterDatasByTable($table, $fields = array(), $fields_alias = array(), $conditions = array(), $orderbys = array(), $limit = null, $element = 0, $fetchmode = array('fetchAll'), $callback = ''){
    if(is_numeric($limit) && $limit > 0){
      /*if(!is_numeric($element) || $element < 0){
        $element = 0;
        global $pager_page_array, $pager_limits, $pager_total, $pager_total_items;
        while(isset($pager_page_array[$element])){ $element++; }
      }*/
      $queryObj = db_select($table, $table)->extend('PagerDefault')->limit($limit)->element($element);
    }else{
      $queryObj = db_select($table, $table);
    }
    if(empty($fields)){
      $queryObj->fields($table);
    }else{
      $queryObj->fields($table, $fields);
    }
    foreach($fields_alias as $key => $value){
      $queryObj->addField($table, $key, $value);
    }
    self::ProcessConditions($queryObj, $conditions);
    self::ProcessOrderbys($queryObj, $orderbys);
    if(!empty($callback) && is_string($callback)){
      $callback_info = explode('|', $callback);
      $callback_count = count($callback_info);
      $queryObj_new = null;
      if($callback_count == 1 && function_exists($callback_info[0])){
        $queryObj_new = call_user_func_array($callback_info[0], array($queryObj));
      }else if($callback_count >= 2 && method_exists($callback_info[0], $callback_info[1])){
        $queryObj_new = call_user_func_array(array($callback_info[0], $callback_info[1]), array($queryObj));
      }
      if($queryObj_new){ $queryObj = $queryObj_new; }
    }
    $arrDatas = array();
    if(isset($fetchmode[0]) && $fetchmode[0] == 'fetchAll'){
      $arrDatas = $queryObj->execute()->fetchAll();
    }else if(isset($fetchmode[1]) && $fetchmode[0] == 'fetchAllAssoc' && is_string($fetchmode[1])){
      $arrDatas = $queryObj->execute()->fetchAllAssoc($fetchmode[1]);
    }else if(isset($fetchmode[0]) && $fetchmode[0] == 'fetchAllKeyed'){
      $key_index = (isset($fetchmode[1]) && is_numeric($fetchmode[1])) ? $fetchmode[1] : 0;
      $value_index = (isset($fetchmode[2]) && is_numeric($fetchmode[2])) ? $fetchmode[2] : 1;
      $arrDatas = $queryObj->execute()->fetchAllKeyed($key_index, $value_index);
    }else if(isset($fetchmode[0]) && $fetchmode[0] == 'fetchCol'){
      $index = (isset($fetchmode[1]) && is_numeric($fetchmode[1])) ? $fetchmode[1] : 0;
      $arrDatas = $queryObj->execute()->fetchCol($index);
    }else if(isset($fetchmode[0]) && $fetchmode[0] == 'fetchObject'){
      return $queryObj->execute()->fetchObject();
    }else if(isset($fetchmode[0]) && $fetchmode[0] == 'fetchField'){
      $index = (isset($fetchmode[1]) && is_numeric($fetchmode[1])) ? $fetchmode[1] : 0;
      return $queryObj->execute()->fetchField($index);
    }else if(isset($fetchmode[0]) && $fetchmode[0] == 'rowCount'){
      return $queryObj->execute()->rowCount();
    }else{
      $arrDatas = $queryObj->execute()->fetchAll();
    }
    return $arrDatas;
  }
  /**
   * Filter datas join by tables
   * @access final
   * @static
   * @param array $tables The collection of table.
   *      Each element is a array with table information($table, $join, $relatedkey, $fields, $fields_alias).
   *       - $table: base table for this query. May be a string or another SelectQuery object. If a query object is passed, it will be used as a subselect.
   *       - $join: The join way, such as INNER, LEFT OUTER, RIGHT OUTER. Ignored for the first category-related information.
   *       - $relatedkey: The associated key.
   *       - $fields: The selected field list. Each element is a field name. Defaults to select all field(*) if $fields is empty.
   *       - $fields_alias: The selected field alias list. Each element key is a field name; each element value is a field alias.
   *       - sample: $tables = array(array('minifield', '', 'cid', array(), array()), array('minifieldcategory', 'INNER', 'id', array('tablename', 'table_type'), array('id' => 'cateid', 'name' => 'catename')));
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
  final static function FilterDatasByTables($tables = array(), $conditions = array(), $orderbys = array(), $limit = null, $element = 0, $fetchmode = array('fetchAll'), $callback = ''){
    if(!is_array($tables)){ return array(); }
    $table_first = $tables[0];
    if(is_numeric($limit) && $limit > 0){
      /*if(!is_numeric($element) || $element < 0){
        $element = 0;
        global $pager_page_array, $pager_limits, $pager_total, $pager_total_items;
        while(isset($pager_page_array[$element])){ $element++; }
      }*/
      $queryObj = db_select($table_first[0], $table_first[0])->extend('PagerDefault')->limit($limit)->element($element);
    }else{
      $queryObj = db_select($table_first[0], $table_first[0]);
    }
    if(empty($table_first[3])){
      $queryObj->fields($table_first[0]);
    }else{
      $queryObj->fields($table_first[0], $table_first[3]);
    }
    foreach($table_first[4] as $key => $value){
      $queryObj->addField($table_first[0], $key, $value);
    }
    foreach($tables as $key => $table){
      if($key == 0){ continue; }
      $alias = $queryObj->addJoin($table[1], $table[0], $table[0], $table[0].'.'.$table[2].' = '.$table_first[0].'.'.$table_first[2]);
      if(empty($table[3])){
        $queryObj->fields($alias);
      }else{
        $queryObj->fields($alias, $table[3]);
      }
      foreach($table[4] as $key => $value){
        $queryObj->addField($alias, $key, $value);
      }
    }
    self::ProcessConditions($queryObj, $conditions);
    self::ProcessOrderbys($queryObj, $orderbys);
    if(!empty($callback) && is_string($callback)){
      $callback_info = explode('|', $callback);
      $callback_count = count($callback_info);
      $queryObj_new = null;
      if($callback_count == 1 && function_exists($callback_info[0])){
        $queryObj_new = call_user_func_array($callback_info[0], array($queryObj));
      }else if($callback_count >= 2 && method_exists($callback_info[0], $callback_info[1])){
        $queryObj_new = call_user_func_array(array($callback_info[0], $callback_info[1]), array($queryObj));
      }
      if($queryObj_new){ $queryObj = $queryObj_new; }
    }
    $arrDatas = array();
    if(isset($fetchmode[0]) && $fetchmode[0] == 'fetchAll'){
      $arrDatas = $queryObj->execute()->fetchAll();
    }else if(isset($fetchmode[1]) && $fetchmode[0] == 'fetchAllAssoc' && is_string($fetchmode[1])){
      $arrDatas = $queryObj->execute()->fetchAllAssoc($fetchmode[1]);
    }else if(isset($fetchmode[0]) && $fetchmode[0] == 'fetchAllKeyed'){
      $key_index = (isset($fetchmode[1]) && is_numeric($fetchmode[1])) ? $fetchmode[1] : 0;
      $value_index = (isset($fetchmode[2]) && is_numeric($fetchmode[2])) ? $fetchmode[2] : 1;
      $arrDatas = $queryObj->execute()->fetchAllKeyed($key_index, $value_index);
    }else if(isset($fetchmode[0]) && $fetchmode[0] == 'fetchCol'){
      $index = (isset($fetchmode[1]) && is_numeric($fetchmode[1])) ? $fetchmode[1] : 0;
      $arrDatas = $queryObj->execute()->fetchCol($index);
    }else if(isset($fetchmode[0]) && $fetchmode[0] == 'fetchObject'){
      return $queryObj->execute()->fetchObject();
    }else if(isset($fetchmode[0]) && $fetchmode[0] == 'fetchField'){
      $index = (isset($fetchmode[1]) && is_numeric($fetchmode[1])) ? $fetchmode[1] : 0;
      return $queryObj->execute()->fetchField($index);
    }else if(isset($fetchmode[0]) && $fetchmode[0] == 'rowCount'){
      return $queryObj->execute()->rowCount();
    }else{
      $arrDatas = $queryObj->execute()->fetchAll();
    }
    return $arrDatas;
  }
  /**
   * Delete datas by table
   * @access final
   * @static
   * @param string $table table from which to delete.
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
   */
  final static function DeleteDatasByTable($table, $conditions){
    $queryObj = db_delete($table);
    self::ProcessConditions($queryObj, $conditions);
    $queryObj->execute();
  }
  /**
   * Checks whether a data exists.
   * @access public
   * @static
   * @param string $table The table from which to check.
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
   * @return bool <b>TRUE</b> if the data specified by conditions exists; <b>FALSE</b> otherwise. 
   */
  public static function ExistDatasByTable($table, $conditions = array()){
    $queryObj = db_select($table, $table);
    self::ProcessConditions($queryObj, $conditions);
    $queryObj->addExpression(1);
    if($queryObj->execute()->fetchField()){
      return true;
    }
    return false;
  }

  /**
   * Load data column by table and name of a field.
   * @access public
   * @static
   * @param string $table base table for this query. May be a string or another SelectQuery object. If a query object is passed, it will be used as a subselect.
   * @param string $field The name of the field.
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
   * @param boolean $distinct TRUE to flag this query DISTINCT, FALSE to disable it.
   * @param type $limit The page limit or null.
   * @param int $element An optional integer to distinguish between multiple pagers on one page.
   * @return array An indexed array, or an empty array if there is no result set.
   */
  final static function LoadDataColByTableField($table, $field, $conditions = array(), $orderbys = array(), $distinct = false, $limit = null, $element = 0){
    if(is_numeric($limit) && $limit > 0){
      /*if(!is_numeric($element) || $element < 0){
        $element = 0;
        global $pager_page_array, $pager_limits, $pager_total, $pager_total_items;
        while(isset($pager_page_array[$element])){ $element++; }
      }*/
      $queryObj = db_select($table, $table)->extend('PagerDefault')->limit($limit)->element($element);
    }else{
      $queryObj = db_select($table, $table);
    }
    $queryObj->fields($table, array($field));
    self::ProcessConditions($queryObj, $conditions);
    self::ProcessOrderbys($queryObj, $orderbys);
    if($distinct == true){ $queryObj->distinct(); }
    return $queryObj->execute()->fetchCol();
  }
  final static function LoadDataIdesByTable($table, $conditions = array(), $orderbys = array()){
    return self::LoadDataColByTableField($table, 'id', $conditions, $orderbys);
  }
  /**
   * Load data count by table and name of a field.
   * @access public
   * @static
   * @param string $table base table for this query. May be a string or another SelectQuery object. If a query object is passed, it will be used as a subselect.
   * @param string $field The name of the field.
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
   * @param boolean $distinct TRUE to flag this query DISTINCT, FALSE to disable it.
   * @return int The number of data count.
   */
  final static function LoadDataCountByTableField($table, $field, $conditions = array(), $distinct = false){
    $queryObj = db_select($table, $table);
    $queryObj->fields($table, array($field));
    self::ProcessConditions($queryObj, $conditions);
    if($distinct == true){ $queryObj->distinct(); }
    return $queryObj->execute()->rowCount();
  }
  final static function LoadDataCountByTable($table, $conditions = array()){
    return self::LoadDataCountByTableField($table, 'id', $conditions);
  }

  /**
   * Load data expression value by table and name of a field.
   * @access public
   * @static
   * @param string $table base table for this query. May be a string or another SelectQuery object. If a query object is passed, it will be used as a subselect.
   * @param string $field The name of the field.
   * @param $expression The expression string. May contain placeholders. The expressions include COUNT, MAX, MIN, SUM, AVG.
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
   * @param boolean $distinct TRUE to flag this query DISTINCT, FALSE to disable it.
   * @return int/boolean A single field from the next record, or FALSE if there is no next record.
   */
  final static function LoadDataExpressionByTableField($table, $field, $expression, $conditions = array()){
    $queryObj = db_select($table, $table);
    $queryObj->addExpression($expression . '(' . $field . ')', $field . strtolower($expression));
    self::ProcessConditions($queryObj, $conditions);
    return $queryObj->execute()->fetchField();
  }

  /**
   *
   * @access public
   * @static
   * @param object $queryObj
   * @param array $expressions The collection of expression.
   *      Each element is still a array with two to three elements($field, $expression[, $arguments]):
   *       - $field: The field to set.
   *       - $expression: The field will be set to the value of this expression. This parameter may include named placeholders.
   *       - $arguments: If specified, this is an array of key/value pairs for named placeholders corresponding to the expression.
   *      Example: $table = 'users', $expressions = array(array("uid", "uid + 1000"), array("name", "concat('prefix_', uid, '_', mail)"), array("pass", "md5(pass)")).
   */
  public static function ProcessExpressions($queryObj, $expressions){
    foreach($expressions as $expression){
      if(isset($expression[2])){
        $queryObj->expression($expression[0], $expression[1], $expression[2]);
      }else if(isset($expression[1])){
        $queryObj->expression($expression[0], $expression[1]);
      }else if(isset($expression[0])){
        $queryObj->expression($expression[0]);
      }
    }
    return $queryObj;
  }
  /**
   *
   * @access public
   * @static
   * @param object $queryObj
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
   */
  public static function ProcessConditions($queryObj, $conditions){
    foreach($conditions as $condition){
      if(isset($condition['conjunction']) && isset($condition['conditions']) && !empty($condition['conditions'])){
        $conjunction = strtolower($condition['conjunction']);
        if(!in_array($conjunction, array('and', 'or', 'xor'))){ continue; }
        $dbcondition = 'db_' . $conjunction;
        $conjunctionObj = $dbcondition();
        self::ProcessConditions($conjunctionObj, $condition['conditions']);
        $queryObj->condition($conjunctionObj);
      }else{
        if(!isset($condition[0])){
          continue;
        }else if(!isset($condition[1])){
          $queryObj->condition($condition[0]);
        }else if($condition[1] === ''){
          if(isset($condition[2]) && $condition[2] == '!='){
            $queryObj->condition(db_and()->condition($condition[0], '', '!=')->isNotNull($condition[0]));
          }else{
            $queryObj->condition(db_or()->condition($condition[0], '')->isNull($condition[0]));
          }
        }else if(!isset($condition[2])){
          if(is_array($condition[1]) && empty($condition[1])){ continue; }
          $queryObj->condition($condition[0], $condition[1]);
        }else if(strtolower($condition[2]) === 'where'){
          if(empty($condition[0])){ continue; }
          $queryObj->where($condition[0], $condition[1]);
        }else{
          if(is_array($condition[1]) && empty($condition[1])){ continue; }
          $queryObj->condition($condition[0], $condition[1], $condition[2]);
        }
      }
    }
    return $queryObj;
  }
  /**
   *
   * @access public
   * @static
   * @param object $queryObj
   * @param array $orderbys Orders the result set by a given field.
   *      Each element is still a array with one to two elements($field[, $direction]):
   *       - $field: The field on which to order.
   *       - $direction: The direction to sort. Legal values are "ASC" or "DESC" or "RANDOM"($field is invalid), Default to "ASC".
   */
  public static function ProcessOrderbys($queryObj, $orderbys){
    foreach($orderbys as $orderby){
      if(isset($orderby[1]) && $orderby[1] == 'RANDOM'){
        $queryObj->orderRandom();
      }else if(isset($orderby[1])){
        $queryObj->orderBy($orderby[0], $orderby[1]);
      }else{
        $queryObj->orderBy($orderby[0]);
      }
    }
    return $queryObj;
  }
}