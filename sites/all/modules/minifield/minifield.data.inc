<?php 
/**
 * Available hooks:
 *  - hook_minifielddata_save_$cid($id = 0)
 *  - hook_minifielddata_delete_$cid($ides = array())
 *  - hook_minifielddata_load_$cid($arrDatas = array())
 * @author Changfeng Ji <jichf@qq.com>
 */
class MiniFieldData{
  /**
   * Post data has been processed
   * @var boolean
   */
  protected $PostDataProcessed = false;
  /**
   * category structure
   * @var object
   */
  public $Category = null;
  /**
   * field structure
   * @var array
   */
  public $arrFields = array();
  /**
   * Whether the data exists. The changes are effective before SaveDataToDB() is called
   * @var boolean
   */
  public $IsExists = false;
  /**
   * table field model
   * @var array
   */
  public $FieldModel = array();
  /**
   * table field data check
   * @var array
   */
  public $FieldCheck = array();
  /**
   * Occurs after construct the instance.
   * 
   * Possible value:
   *  - a function string.
   *  - a class-level static function string. the class string and function string are separated by '|'.
   * 
   * Function arguments:
   *  - MiniFieldData $mfd.
   * @var string
   */
  protected $Event_AfterConstruct = "";
  /**
   * Occurs after $this->ProcessPostData() is executed.
   * 
   * Possible value:
   *  - a function string.
   *  - a class-level static function string. the class string and function string are separated by '|'.
   * 
   * Function arguments:
   *  - MiniFieldData $mfd.
   *  - array $errors.
   *
   * Function expected return value: array $errors.
   * @var string
   */
  public $Event_AfterProcessPostData = "";
  /**
   * Occurs after $this->SaveDataToDB() is executed.
   *
   * Possible value:
   *  - a function string.
   *  - a class-level static function string. the class string and function string are separated by '|'.
   * 
   * Function arguments:
   *  - MiniFieldData $mfd.
   *  - mixed $result.
   * @var string
   */
  public $Event_AfterSaveDataToDB = "";
  protected function CheckFieldData($field){
    $errors = array();
    if(!empty($this->FieldCheck[$field]['validate'])){
      $args = explode('|', $this->FieldCheck[$field]['validate']);
      if(is_array($args) && count($args) >= 2){
        $class = array_shift($args);
        $function = array_shift($args);
        array_unshift($args, $this);
        if(method_exists($class, $function)){
          $errors = array_merge(call_user_func_array(array($class, $function), $args), $errors);
        }
      }
    }
    if(!empty($this->FieldCheck[$field]['regexp'])){
      $pattern = $this->FieldCheck[$field]['regexp'];
      $subject = $this->FieldModel[$field];
      if(preg_match($pattern, $subject) == 0){
        $errors[$field] = $this->FieldCheck[$field]['regexp_error'];
      }
    }
    return $errors;
  }
  protected function TriggerEvent($evt, $args = array()){
    $result = false;
    if(!empty($evt) && is_string($evt)){
      $dynamicinfo = explode('|', $evt);
      if(count($dynamicinfo) == 1){
        $function = array_shift($dynamicinfo);
        if(function_exists($function)){
          $result = call_user_func_array($function, $args);
        }
      }else if(count($dynamicinfo) >= 2){
        $class = array_shift($dynamicinfo);
        $method = array_shift($dynamicinfo);
        if(method_exists($class, $method)){
          $result = call_user_func_array(array($class, $method), $args);
        }
      }
    }
    return $result;
  }
  /**
   * 
   * @param int $cid
   * @param int $id
   * @param array $events The array keys are the event names and the values are the function strings to which to execute them.
   * 
   *   The key include: 
   *     - AfterConstruct: Occurs after construct the instance. Function arguments: (MiniFieldData $mfd).
   *     - AfterProcessPostData: Occurs after $this->SaveDataToDB() is executed. Function arguments: (MiniFieldData $mfd, array $errors). Function expected return value: array $errors.
   *     - AfterSaveDataToDB：Occurs after $this->SaveDataToDB() is executed. Function arguments: (MiniFieldData $mfd, mixed $result).
   * 
   *   The possible value include:
   *     - a function string.
   *     - a class-level static function string. the class string and function string are separated by '|'.
   */
  public function __construct($cid, $id = null, $events = array()) {
    $id = (preg_match('/^\d+$/', $id) == 0) ? 0 : $id;
    if($id == 0 && isset($_POST['id']) && preg_match('/^\d+$/', $_POST['id']) > 0){
      $id = $_POST['id'];
    }
    $this->InitStructure($cid, $id);
    foreach($this->arrFields as $key=>$Field){
      if($Field->enabled != 1){
        unset($this->arrFields[$key]);
      }
    }
    $this->FieldModel = array('id' => $id);
    $this->FieldCheck = array();
    foreach($this->arrFields as $key=>$Field){
      $this->FieldCheck[$Field->fieldname] = array();
      $this->FieldCheck[$Field->fieldname]['validate'] = $Field->editor_validate;
      $this->FieldCheck[$Field->fieldname]['regexp'] = $Field->editor_regexp;
      $this->FieldCheck[$Field->fieldname]['regexp_error'] = $Field->editor_regexp_error;
      $this->FieldModel[$Field->fieldname] = $Field->field_default;
      if(in_array($Field->editor, array('radio', 'checkbox', 'select', 'checkboxes', 'selects', 'textselect', 'texttags'))){
        $this->arrFields[$key]->arrValueArrays = self::GetValueArrays($Field);
        if(in_array($Field->editor, array('radio', 'checkbox', 'select', 'textselect'))){
          foreach($this->arrFields[$key]->arrValueArrays as $ValueArray){
            if(isset($ValueArray[3]) && $ValueArray[3] == 1 && $ValueArray[0] != $Field->editor_values_group . '_begin' && $ValueArray[0] != $Field->editor_values_group . '_end'){
              $this->FieldModel[$Field->fieldname] = $ValueArray[0];
              break;
            }
          }
        }else if($Field->editor == 'texttags'){
          $selectedValues = array();
          foreach($this->arrFields[$key]->arrValueArrays as $ValueArray){
            if(isset($ValueArray[3]) && $ValueArray[3] == 1 && $ValueArray[0] != $Field->editor_values_group . '_begin' && $ValueArray[0] != $Field->editor_values_group . '_end'){
              $selectedValues[] = $ValueArray[0];
            }
          }
          $this->FieldModel[$Field->fieldname] = implode(',', $selectedValues);
        }else if(in_array($Field->editor, array('checkboxes', 'selects'))){
          $selectedValues = array();
          foreach($this->arrFields[$key]->arrValueArrays as $ValueArray){
            if(isset($ValueArray[3]) && $ValueArray[3] == 1 && $ValueArray[0] != $Field->editor_values_group . '_begin' && $ValueArray[0] != $Field->editor_values_group . '_end'){
              $selectedValues[] = $ValueArray[0];
            }
          }
          $this->FieldModel[$Field->fieldname] = MiniFieldCommon::JoinedByNewline($selectedValues);
        }
      }
    }
    $this->AssemblyData($cid, $id);
    if(!empty($events)){
      if(isset($events['AfterConstruct']) && is_string($events['AfterConstruct'])){
        $this->Event_AfterConstruct = $events['AfterConstruct'];
      }
      if(isset($events['AfterProcessPostData']) && is_string($events['AfterProcessPostData'])){
        $this->AfterProcessPostData = $events['AfterProcessPostData'];
      }
      if(isset($events['AfterSaveDataToDB']) && is_string($events['AfterSaveDataToDB'])){
        $this->AfterSaveDataToDB = $events['AfterSaveDataToDB'];
      }
    }
    $this->TriggerEvent($this->Event_AfterConstruct, array($this));
  }
  public function InitStructure($cid, $id){
    $this->Category = MiniFieldCategory::LoadDataByCidId(null, $cid, false);
    $this->arrFields = MiniField::LoadDatasByCid($cid, null, false);
  }
  public function AssemblyData($cid, $id){
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
  /**
   * Process post data.
   * includes process post data, check data.
   * @return array :
   *   - An empty array, if successfully.
   *   - An associative array of reasons for failure, where the keys are the field name and the values are the values those field name must have, if failed.
   */
  public function ProcessPostData(){
    $errors = array();
    $newProcessedFields = array();
    $newDeletedFileIdes = array();
    $newUploadedFileIdes = array();
    foreach($this->arrFields as $key=>$Field){
      $fieldTag = $Field->fieldname;
      if(isset($_POST[$fieldTag]) || isset($_FILES['files']['name'][$fieldTag])){ $newProcessedFields[] = $fieldTag; }
      switch ($Field->editor) {
        /******* basic single editors *******/
        case 'text':
        case 'hidden':
        case 'textarea':
          if(isset($_POST[$fieldTag])){
            $this->FieldModel[$fieldTag] = $_POST[$fieldTag];
            $errors = array_merge($errors, $this->CheckFieldData($fieldTag));
          }
          break;
        case 'radio':
        case 'checkbox':
        case 'select':
          if(isset($_POST['switchof'.$fieldTag])){
            $this->FieldModel[$fieldTag] = isset($_POST[$fieldTag]) ? $_POST[$fieldTag] : '';
            $errors = array_merge($errors, $this->CheckFieldData($fieldTag));
          }
          break;
        case 'password':
          if(isset($_POST[$fieldTag]) && !empty($_POST[$fieldTag])){
            $this->FieldModel[$fieldTag] = md5(trim($_POST[$fieldTag]));
            $errors = array_merge($errors, $this->CheckFieldData($fieldTag));
          }
          break;
        case 'file':
          if(isset($_FILES['files']['name'][$fieldTag])){
            $validators = array();
            $arrValues = MiniFieldCommon::SplittedByNewline($Field->editor_filevalidate);
            foreach($arrValues as $Value){
              $args = explode('|', $Value);
              if(is_array($args)){
                $validators[] = $args;
              }
            }
            $result = MiniFieldFile::FileSaveUpload($fieldTag, 0, 'public://', $validators, 1);
            if(is_object($result) && $result->fid){
              $attach = $result;
              $newDeletedFileIdes[] = $this->FieldModel[$fieldTag];
              $newUploadedFileIdes[] = $attach->fid;
              $this->FieldModel[$fieldTag] = $attach->fid;
            }else if(is_array($result) && !empty($result)){
              $errors[$fieldTag] = $result[0];
            }
            if(!isset($errors[$fieldTag])){
              $errors = array_merge($errors, $this->CheckFieldData($fieldTag));
            }
          }
          break;
        /******* basic multiple editors *******/
        case 'texts':
          if(isset($_POST[$fieldTag])){
            $this->FieldModel[$fieldTag] = is_array($_POST[$fieldTag]) ? MiniFieldCommon::JoinedByNewline($_POST[$fieldTag]) : '';
            $errors = array_merge($errors, $this->CheckFieldData($fieldTag));
          }
          break;
        case 'checkboxes':
        case 'selects':
          if(isset($_POST['switchof'.$fieldTag])){
            if(isset($_POST[$fieldTag]) && is_array($_POST[$fieldTag])){
              $this->FieldModel[$fieldTag] = MiniFieldCommon::JoinedByNewline($_POST[$fieldTag]);
            }else{
              $this->FieldModel[$fieldTag] = '';
            }
            $errors = array_merge($errors, $this->CheckFieldData($fieldTag));
          }
          break;
        case 'files':
          if(isset($_FILES['files']['name'][$fieldTag])){
            $fides = MiniFieldCommon::SplittedByNewline($this->FieldModel[$fieldTag]);
            $validedfides = array();
            foreach($fides as $fid){
              if(preg_match('/^\d+$/', $fid) > 0 && $fid > 0){
                $file = file_load($fid);
                if($file && $file->fid){ $validedfides[] = $fid; }
              }
            }
            $validators = array();
            $arrValues = MiniFieldCommon::SplittedByNewline($Field->editor_filevalidate);
            foreach($arrValues as $Value){
              $args = explode('|', $Value);
              if(is_array($args)){
                $validators[] = $args;
              }
            }
            if(is_array($_FILES['files']['name'][$fieldTag])){
              foreach($_FILES['files']['name'][$fieldTag] as $key=>$value){
                $result = MiniFieldFile::FileSaveUpload($fieldTag, $key, 'public://', $validators, 1);
                if(is_object($result) && $result->fid){
                  $attach = $result;
                  $newUploadedFileIdes[] = $attach->fid;
                  $validedfides[] = $attach->fid;
                }else if(is_array($result) && !empty($result)){
                  $errors[$fieldTag] = $result[0];
                  break;
                }
              }
            }
            $this->FieldModel[$fieldTag] = MiniFieldCommon::JoinedByNewline($validedfides);
            if(!isset($errors[$fieldTag])){
              $errors = array_merge($errors, $this->CheckFieldData($fieldTag));
            }
          }
          break;
        /******* extend single editors *******/
        case 'pcas':
          if(isset($_POST[$fieldTag])){
            $this->FieldModel[$fieldTag] = is_array($_POST[$fieldTag]) ? MiniFieldCommon::JoinedByNewline($_POST[$fieldTag]) : '';
            $errors = array_merge($errors, $this->CheckFieldData($fieldTag));
          }
          break;
        case 'pcasdict':
          if(isset($_POST[$fieldTag])){
            $this->FieldModel[$fieldTag] = $_POST[$fieldTag];
            $errors = array_merge($errors, $this->CheckFieldData($fieldTag));
          }
          break;
        case 'textselect':
        case 'farbtastic':
        case 'baidumappoint':
        case 'browserfile':
          if(isset($_POST[$fieldTag])){
            $this->FieldModel[$fieldTag] = $_POST[$fieldTag];
            $errors = array_merge($errors, $this->CheckFieldData($fieldTag));
          }
          break;
        case 'texttags':
          if(isset($_POST[$fieldTag])){
            $this->FieldModel[$fieldTag] = trim($_POST[$fieldTag], ' \t\n\r\0\x0B,，　');
            $this->FieldModel[$fieldTag] = preg_replace('/\s|　/', '', $this->FieldModel[$fieldTag]);
            $this->FieldModel[$fieldTag] = preg_replace('/，/', ',', $this->FieldModel[$fieldTag]);
            $this->FieldModel[$fieldTag] = array_unique(explode(',', $this->FieldModel[$fieldTag]));
            $this->FieldModel[$fieldTag] = implode(',', $this->FieldModel[$fieldTag]);
            $errors = array_merge($errors, $this->CheckFieldData($fieldTag));
          }
          break;
        case 'datepicker':
          if(isset($_POST[$fieldTag])){
            $time = array();
            $time[0] = isset($_POST[$fieldTag.'hour'])&&is_numeric($_POST[$fieldTag.'hour']) ? $_POST[$fieldTag.'hour'] : 0;
            $time[1] = isset($_POST[$fieldTag.'minute'])&&is_numeric($_POST[$fieldTag.'minute']) ? $_POST[$fieldTag.'minute'] : 0;
            $time[2] = isset($_POST[$fieldTag.'second'])&&is_numeric($_POST[$fieldTag.'second']) ? $_POST[$fieldTag.'second'] : 0;
            $datetime = trim($_POST[$fieldTag]) . ' ' . implode(':', $time);
            $correctDateFormat = date_parse($datetime);
            if(preg_match('/[^\s]+/', $_POST[$fieldTag]) > 0 && isset($correctDateFormat['error_count']) && $correctDateFormat['error_count'] == 0){
              $this->FieldModel[$fieldTag] = strtotime($datetime);
            }else{
              $this->FieldModel[$fieldTag] = '';
            }
            $errors = array_merge($errors, $this->CheckFieldData($fieldTag));
          }
          break;
        case 'ckeditor':
        case 'ueditor':
          if(isset($_POST[$fieldTag])){
            $this->FieldModel[$fieldTag] = get_magic_quotes_gpc() ? htmlspecialchars($_POST[$fieldTag]) : htmlspecialchars(addslashes($_POST[$fieldTag]));
            $errors = array_merge($errors, $this->CheckFieldData($fieldTag));
          }
          break;
        /******* extend multiple editors *******/
      }
    }
    $result = $this->TriggerEvent($this->Event_AfterProcessPostData, array($this, $errors));
    if(is_array($result) && !empty($result)){
      $errors = array_merge($errors, $result);
    }
    if(empty($newProcessedFields)){
      $errors = array_merge($errors, array(ts('没有添加字段数据', 'ucfirst', 'minifield')));
    }
    if(empty($errors)){//delete new deleted files when validate succeed
      foreach($newDeletedFileIdes as $newDeletedFilesId){
        if(preg_match('/^\d+$/', $newDeletedFilesId) > 0 && $newDeletedFilesId > 0){
          $file = file_load($newDeletedFilesId);
          if($file && $file->fid){ file_delete($file, true); }
        }
      }
    }else{//delete new uploaded files when validate failed
      foreach($newUploadedFileIdes as $newUploadedFilesId){
        if(preg_match('/^\d+$/', $newUploadedFilesId) > 0 && $newUploadedFilesId > 0){
          $file = file_load($newUploadedFilesId);
          if($file && $file->fid){ file_delete($file, true); }
        }
      }
    }
    $this->PostDataProcessed = true;
    return $errors;
  }
  /**
   * Save data to database table.
   * includes process post data, check data, save data.
   * @return int/array :
   *   - The last insert/update ID of the query, if successfully.
   *   - An associative array of reasons for failure, where the keys are the field name and the values are the values those field name must have, if failed.
   */
  public function SaveDataToDB(){
    if($this->PostDataProcessed == false){
      $errors = $this->ProcessPostData();
      if(!empty($errors)){
        return $errors;
      }
    }
    $CurrentTN = $this->Category->tablename;
    $CurrentFM = $this->FieldModel;
    if(!isset($CurrentFM['id']) || preg_match('/^\d+$/', $CurrentFM['id']) == 0 || $CurrentFM['id'] < 0){
      $CurrentFM['id'] = 0;
      $this->IsExists = false;
    }
    if($CurrentFM['id'] > 0 && $this->IsExists == false){
      $datacount = MiniFieldSql::LoadDataCountByTable($CurrentTN, array(array('id', $CurrentFM['id'])));
      if($datacount > 0){
        $this->IsExists = true;
      }
    }
    if($CurrentFM['id'] > 0 && $this->IsExists == true){
      $id = $CurrentFM['id'];
      unset($CurrentFM['id']);
      $condition = db_update($CurrentTN)->fields($CurrentFM)->condition('id', $id);
      $condition->execute();//The number of rows affected by the update.
      $this->TriggerEvent($this->Event_AfterSaveDataToDB, array($this, $id));
      if($this->Category->id > 0){ module_invoke_all('minifielddata_save_' . $this->Category->id, $id); }
      return $id;
    }else{
      if($CurrentFM['id'] == 0){ unset($CurrentFM['id']); }
      $condition = db_insert($CurrentTN)->fields($CurrentFM);
      $result = $condition->execute();
      $this->FieldModel['id'] = is_numeric($result) ? $result : $this->FieldModel['id'];
      $this->TriggerEvent($this->Event_AfterSaveDataToDB, array($this, $result));
      if($this->Category->id > 0){ module_invoke_all('minifielddata_save_' . $this->Category->id, $result); }
      return $result;
    }
  }
  
  /* static start */
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
  public static function LoadDatasByCid($cid, $limit = null, $format = false, $element = 0, $invokehook = true){
    $tablename = minifield_tablename_load($cid);
    if(empty($tablename)){ return array(); }
    $arrDatas = MiniFieldSql::LoadDatasByTable($tablename, $limit, $element);
    if($format == true){
      $arrFields = MiniField::LoadDatasByCid($cid, null, false);
      $arrDatas = MiniFieldData::FormatDatas($arrDatas, $arrFields);
    }
    if($invokehook == true){ module_invoke_all('minifielddata_load_' . $cid, $arrDatas); }
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
  public static function LoadDataByCidId($cid, $id, $format = false, $invokehook = true){
    $tablename = minifield_tablename_load($cid);
    if(empty($tablename)){ return null; }
    $Data = MiniFieldSql::LoadDataByTableId($tablename, $id);
    if($format == true){
      $arrFields = MiniField::LoadDatasByCid($cid, null, false);
      $Data = self::FormatData($Data, $arrFields);
    }
    if($invokehook == true){ module_invoke_all('minifielddata_load_' . $cid, array($Data)); }
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
  public static function DeleteDataByCidId($cid, $id, $includefiles = false, $invokehook = true){
    $tablename = minifield_tablename_load($cid);
    if(empty($tablename)){ return; }
    if($invokehook == true){ module_invoke_all('minifielddata_delete_' . $cid, array($id)); }
    if($includefiles == true){
      $arrFields = MiniField::LoadDatasByCid($cid, null, false);
      $Data = self::LoadDataByCidId($cid, $id, false);
      self::DeleteDataFiles($Data, $arrFields);
    }
    MiniFieldSql::DeleteDataByTableId($tablename, $id);
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
  public static function FilterDatasByCid($cid, $fields = array(), $fields_alias = array(), $conditions = array(), $orderbys = array(), $limit = null, $format = false, $element = 0, $fetchmode = array('fetchAll'), $invokehook = true, $callback = ''){
    $tablename = minifield_tablename_load($cid);
    if(empty($tablename)){ return array(); }
    $arrDatas = MiniFieldSql::FilterDatasByTable($tablename, $fields, $fields_alias, $conditions, $orderbys, $limit, $element, $fetchmode, $callback);
    if($format == true && isset($fetchmode[0]) && in_array($fetchmode[0], array('fetchAll', 'fetchAllAssoc'))){
      $arrFields = MiniField::LoadDatasByCid($cid, null, false);
      $arrDatas = MiniFieldData::FormatDatas($arrDatas, $arrFields, $fields_alias);
    }else if($format == true && isset($fetchmode[0]) && $fetchmode[0] == 'fetchObject'){
      $arrFields = MiniField::LoadDatasByCid($cid, null, false);
      $arrDatas = self::FormatData($arrDatas, $arrFields, $fields_alias);
    }
    if($invokehook == true && isset($fetchmode[0]) && in_array($fetchmode[0], array('fetchAll', 'fetchAllAssoc'))){
      module_invoke_all('minifielddata_load_' . $cid, $arrDatas);
    }else if($invokehook == true && isset($fetchmode[0]) && $fetchmode[0] == 'fetchObject'){
      module_invoke_all('minifielddata_load_' . $cid, array($arrDatas));
    }
    return $arrDatas;
  }
  /**
   * Delete datas by minifield category identifier
   * @access final
   * @static
   * @param int $cid The minifield category identifier.
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
   * @param boolean $includefiles whether to delete related files or not.
   * @param boolean $invokehook whether to invoke hook hook_minifielddata_delete_$cid($ides = array()).
   */
  final static function DeleteDatasByCid($cid, $conditions, $includefiles = false, $invokehook = true){
    $tablename = minifield_tablename_load($cid);
    if(empty($tablename)){ return; }
    if($invokehook == true){
      $ides = self::LoadDataIdesByCid($cid, $conditions);
      module_invoke_all('minifielddata_delete_' . $cid, $ides);
    }
    if($includefiles == true){
      $arrFields = MiniField::LoadDatasByCid($cid, null, false);
      $arrDatas = self::FilterDatasByCid($cid, array(), array(), $conditions, array(), null, false);
      foreach($arrDatas as $Data){
        self::DeleteDataFiles($Data, $arrFields);
      }
    }
    MiniFieldSql::DeleteDatasByTable($tablename, $conditions);
  }
  /**
   * Load data column by minifield category identifier and name of a field.
   * @access public
   * @static
   * @param int $cid The minifield category identifier.
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
  final static function LoadDataColByCidField($cid, $field, $conditions = array(), $orderbys = array(), $distinct = false, $limit = null, $element = 0){
    $tablename = minifield_tablename_load($cid);
    if(empty($tablename)){ return array(); }
    return MiniFieldSql::LoadDataColByTableField($tablename, $field, $conditions, $orderbys, $distinct, $limit, $element);
  }
  final static function LoadDataIdesByCid($cid, $conditions = array(), $orderbys = array()){
    return self::LoadDataColByCidField($cid, 'id', $conditions, $orderbys);
  }
  /**
   * Load data count by minifield category identifier and name of a field.
   * @access public
   * @static
   * @param int $cid The minifield category identifier.
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
   * @return int The number of data count. -1 if $cid error.
   */
  final static function LoadDataCountByCidField($cid, $field, $conditions = array(), $distinct = false){
    $tablename = minifield_tablename_load($cid);
    if(empty($tablename)){ return -1; }
    return MiniFieldSql::LoadDataCountByTableField($tablename, $field, $conditions, $distinct);
  }
  final static function LoadDataCountByCid($cid, $conditions = array()){
    return self::LoadDataCountByCidField($cid, 'id', $conditions);
  }
  /**
   * Load data expression value by table and name of a field.
   * @access public
   * @static
   * @param int $cid The minifield category identifier.
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
  final static function LoadDataExpressionByCidField($cid, $field, $expression, $conditions = array()){
    $tablename = minifield_tablename_load($cid);
    if(empty($tablename)){ return false; }
    return MiniFieldSql::LoadDataExpressionByTableField($tablename, $field, $expression, $conditions);
  }
  /**
   * Format data by a collection of fields.
   * @access public
   * @static
   * @param object $Data The unformatted datas will to be formatted.
   * @param array $arrFields A unformatted collection of fields for format data.
   * @param array $fields_alias The selected field alias list. Each element key is a field name; each element value is a field alias.
   * @return object The formatted datas.
   */
  public static function FormatDatas($arrDatas, $arrFields, $fields_alias = array()){
    if(empty($arrDatas)){
      return $arrDatas;
    }
    if(!empty($fields_alias)){
      foreach($arrFields as $key=>$Field){
        $fieldTag = $Field->fieldname;
        if(isset($fields_alias[$fieldTag])){
          $newField = clone $Field;
          $newField->fieldname = $fields_alias[$fieldTag];
          $arrFields[] = $newField;
          unset($fields_alias[$fieldTag]);
        }
      }
    }
    foreach($arrDatas as $key => $Data){
      $arrDatas[$key] = MiniFieldData::FormatData($Data, $arrFields, $fields_alias);
    }
    return $arrDatas;
  }
  /**
   * Format data by a collection of fields.
   * @access public
   * @static
   * @param object $Data The unformatted data will to be formatted.
   * @param array $arrFields A unformatted collection of fields for format data.
   * @param array $fields_alias The selected field alias list. Each element key is a field name; each element value is a field alias.
   * @return object The formatted data.
   */
  public static function FormatData($Data, $arrFields, $fields_alias = array()){
    if(!empty($fields_alias)){
      foreach($arrFields as $key=>$Field){
        $fieldTag = $Field->fieldname;
        if(isset($fields_alias[$fieldTag])){
          $newField = clone $Field;
          $newField->fieldname = $fields_alias[$fieldTag];
          $arrFields[] = $newField;
          unset($fields_alias[$fieldTag]);
        }
      }
    }
    foreach($arrFields as $key=>$Field){
      $fieldTag = $Field->fieldname;
      if($Field->enabled == 1 && isset($Data->$fieldTag)){
        $Data->$fieldTag = self::FormatDataValue($Data->$fieldTag, $Field);
      }else{
        if(isset($Data->$fieldTag)){ unset($Data->$fieldTag); }
        unset($arrFields[$key]);
      }
    }
    return $Data;
  }
  /**
   * Format data value by a field object.
   * @access public
   * @static
   * @param mixed $DataValue The unformatted data value will to be formatted.
   * @param object $Field The unformatted field object.
   * @return mixed The formatted data value.
   */
  public static function FormatDataValue($DataValue, $Field){
    switch ($Field->editor) {
      /******* basic single editors *******/
      case 'text':
      case 'hidden':
      case 'password':
      case 'textarea':
        $DataValue = $DataValue;
        break;
      case 'radio':
      case 'checkbox':
      case 'select':
        $selectedValue = $DataValue;
        $DataValue = array();
        $arrValueArrays = self::GetValueArrays($Field);
        foreach($arrValueArrays as $ValueArray){
          if($ValueArray[0] == $selectedValue){
            array_push($DataValue, $ValueArray);
            break;
          }
        }
        if(empty($DataValue)){
          array_push($DataValue, array($selectedValue, $selectedValue, $selectedValue));
        }
        break;
      case 'file':
        $fid = $DataValue;
        $DataValue = array();
        $file = MiniFieldFile::loadFile($fid);
        if($file && $file->fid){
          array_push($DataValue, $file);
        }
        break;
      /******* basic multiple editors *******/
      case 'texts':
        $DataValue = MiniFieldCommon::SplittedByNewline($DataValue, true, false);
        if(!is_array($DataValue)){ $DataValue = array(); }
        break;
      case 'checkboxes':
      case 'selects':
        $selectedValues = MiniFieldCommon::SplittedByNewline($DataValue);
        $DataValue = array();
        $arrValueArrays = self::GetValueArrays($Field);
        foreach($selectedValues as $selectedValue){
          $searched = false;
          foreach($arrValueArrays as $ValueArray){
            if($ValueArray[0] == $selectedValue){
              array_push($DataValue, $ValueArray);
              $searched = true;
              break;
            }
          }
          if($searched == false){
            array_push($DataValue, array($selectedValue, $selectedValue, $selectedValue));
          }
        }
        break;
      case 'files':
        $fides = MiniFieldCommon::SplittedByNewline($DataValue);
        $DataValue = array();
        foreach($fides as $fid){
          $file = MiniFieldFile::loadFile($fid);
          if($file && $file->fid){
            array_push($DataValue, $file);
          }
        }
        break;
      /******* extend single editors *******/
      case 'pcas':
        $DataValue = MiniFieldCommon::SplittedByNewline($DataValue);
        if(!is_array($DataValue)){ $DataValue = array(); }
        break;
      case 'pcasdict':
        $DataValue = $DataValue;
        break;
      case 'textselect':
        $selectedValue = $DataValue;
        $DataValue = array();
        $arrValueArrays = self::GetValueArrays($Field);
        foreach($arrValueArrays as $ValueArray){
          if($ValueArray[0] == $selectedValue){
            array_push($DataValue, $ValueArray);
            break;
          }
        }
        if(empty($DataValue)){
          array_push($DataValue, array($selectedValue, $selectedValue, $selectedValue));
        }
        break;
      case 'texttags':
        $DataValue = $DataValue;
        break;
      case 'datepicker':
        if(is_numeric($DataValue)){
          $datetime = new stdClass();
          $datetime->timestamp = $DataValue;
          $datetime->date = @date("Y-m-d", $DataValue);
          $datetime->date = $datetime->date ? $datetime->date : '';
          $datetime->hour = @date("H", $DataValue);
          $datetime->minute = @date("i", $DataValue);
          $datetime->second = @date("s", $DataValue);
        }else{
          $datetime = new stdClass();
          $datetime->timestamp = '';
          $datetime->date = '';
          $datetime->hour = 0;
          $datetime->minute = 0;
          $datetime->second = 0;
        }
        $DataValue = $datetime;
        break;
      case 'farbtastic':
      case 'baidumappoint':
      case 'browserfile':
        $DataValue = $DataValue;
        break;
      case 'ckeditor':
      case 'ueditor':
        $DataValue = get_magic_quotes_gpc() ? htmlspecialchars_decode($DataValue) : stripslashes(htmlspecialchars_decode($DataValue));
        break;
      /******* extend multiple editors *******/
    }
    return $DataValue;
  }
  /**
   * Delete data-related files by a collection of fields
   * @access public
   * @static
   * @param object $Data The unformatted data.
   * @param array $arrFields A collection of fields for delete data-related files.
   * @param array $fields_alias The selected field alias list. Each element key is a field name; each element value is a field alias.
   */
  public static function DeleteDataFiles($Data, $arrFields, $fields_alias = array()){
    if($Data == false || is_null($Data)){ return; }
    if(!empty($fields_alias)){
      foreach($arrFields as $key=>$Field){
        $fieldTag = $Field->fieldname;
        if(isset($fields_alias[$fieldTag])){
          $newField = clone $Field;
          $newField->fieldname = $fields_alias[$fieldTag];
          $arrFields[] = $newField;
        }
      }
    }
    foreach($arrFields as $key=>$Field){
      $fieldTag = $Field->fieldname;
      if(!isset($Data->$fieldTag)){ continue; }
      switch ($Field->editor) {
        /******* basic single editors *******/
        case 'file':
          $fid = $Data->$fieldTag;
          if(preg_match('/^\d+$/', $fid) > 0 && $fid > 0){
            $file = file_load($fid);
            if($file && $file->fid){ file_delete($file, true); }
          }
          break;
        /******* basic multiple editors *******/
        case 'files':
          $fides = MiniFieldCommon::SplittedByNewline($Data->$fieldTag);
          foreach($fides as $fid){
            if(preg_match('/^\d+$/', $fid) > 0 && $fid > 0){
              $file = file_load($fid);
              if($file && $file->fid){ file_delete($file, true); }
            }
          }
          break;
      }
    }
  }
  /**
   *
   * @access final
   * @static
   * @param object $Field The unformatted field object.
   *      Required attributes:
   *       - $Field->editor_values: The values.
   *       - $Field->editor_values_way: The assembly method, includes 'enumerate', 'increase', 'dynamic'. default to 'enumerate'.
   *       - $Field->editor_values_separator: Defaults to an string '\0'.
   *       - $Field->editor_values_group: Defaults to an string ''.
   * @return array
   */
  final static function GetValueArrays($Field){
    $values = isset($Field->editor_values) ? $Field->editor_values : '';
    $way = isset($Field->editor_values_way) ? $Field->editor_values_way : 'enumerate';
    $separator = isset($Field->editor_values_separator) ? $Field->editor_values_separator : '\0';
    $group = isset($Field->editor_values_group) ? $Field->editor_values_group : '';
    $arrValues = MiniFieldCommon::SplittedByNewline($values);
    $arrValueArrays = array();
    if($way == 'dynamic'){//dynamic way
      $Value = isset($arrValues[0]) ? $arrValues[0] : null;
      $arrValues = array();
      if(!is_null($Value)){
        $dynamicinfo = explode($separator, $Value);
        if(count($dynamicinfo) >= 2){
          $class = array_shift($dynamicinfo);
          $method = array_shift($dynamicinfo);
          array_unshift($dynamicinfo, $separator, $group);
          if(method_exists($class, $method)){
            $arrValueArrays = call_user_func_array(array($class, $method), $dynamicinfo);
          }
        }
      }
      return $arrValueArrays;
    }else if($way == 'increase'){//increase way
      $Value = isset($arrValues[0]) ? $arrValues[0] : null;
      $arrValues = array();
      if(!is_null($Value)){
        $increaseinfo = explode($separator, $Value);
        if(count($increaseinfo) >= 2 && preg_match('/^-?\d+$/', $increaseinfo[0]) > 0 && preg_match('/^-?\d+$/', $increaseinfo[1]) > 0){
          $start = (int)$increaseinfo[0];
          $end = (int)$increaseinfo[1];
          if($start <= $end){
            for($i = $start; $i <= $end; $i++){
              $arrValueArrays[] = array($i, $i, $i);
            }
          }
        }
      }
      return $arrValueArrays;
    }else{//enumerate way
      foreach($arrValues as $Value){
        $info = explode($separator, $Value);
        if(!is_array($info)){
          $info = array($Value);
        }
        if(!isset($info[1])){
          $info = array($info[0], $info[0]);
        }
        if(!isset($info[2])){
          $info = array($info[0], $info[1], $info[1]);
        }
        $arrValueArrays[] = $info;
      }
      return $arrValueArrays;
    }
  }

  /**
   * Checks if a value array exists in an value arrays
   * @access final
   * @static
   * @param array $ValueArray
   * @param array $arrValueArrays
   * @return boolean true if $ValueArray is found in $arrValueArrays, false otherwise.
   */
  final static function InValueArrays($ValueArray, $arrValueArrays){
    if(is_array($arrValueArrays) && isset($ValueArray[0])){
      foreach($arrValueArrays as $Value){
        if(isset($Value[0]) && (string)$Value[0] == (string)$ValueArray[0]){
          return true;
        }
      }
    }
    return false;
  }
  /**
   * 
   * @access final
   * @static
   * @param object $Data The formatted data object.
   * @param object $Field The unformatted field object.
   *      Required attributes:
   *       - $Field->fieldname: The field name.
   *       - $Field->editor: The editor.
   * @param array $Options array of options to control how the data output.
   *      Available keys:
   *       - image_width: The image width(css value).
   *       - image_height: The image height(css value).
   * @return string
   */
  final static function GetDataOutput($Data, $Field, $Options = array()){
    $fieldTag = $Field->fieldname;
    if(!isset($Data->$fieldTag)){ return ''; }
    $output = '';
    switch ($Field->editor) {
      /******* basic single editors *******/
      case 'text':
      case 'hidden':
      case 'password':
      case 'textarea':
        $output = $Data->$fieldTag;
        break;
      case 'radio':
      case 'checkbox':
      case 'select':
        $aliases = array();
        foreach($Data->$fieldTag as $ValueArray){
          $aliases[] = $ValueArray[1];
        }
        $output = MiniFieldCommon::JoinedByNewline($aliases);
        break;
      case 'file':
        foreach($Data->$fieldTag as $file){
          if($file->isimage){
            $imagestyle = array();
            isset($Options['image_width']) ? $imagestyle[] = 'width: ' . $Options['image_width'] . ';' : null;
            isset($Options['image_height']) ? $imagestyle[] = 'height: ' . $Options['image_height'] . ';' : null;
            $imagestyle = !empty($imagestyle) ? ' style="' .  implode('', $imagestyle) . '"' : '';
            $output = '<a target="_blank" href="' . $file->filepath . '" title="' . $file->filename . '"><img' . $imagestyle . ' src="' . $file->filepath . '" alt="' . $file->filename . '" /></a>';
          }else{
            $output = '<a target="_blank" href="' . $file->filepath . '">' . $file->filename . '</a>';
          }
        }
        break;
      /******* basic multiple editors *******/
      case 'texts':
        $output = MiniFieldCommon::JoinedByNewline($Data->$fieldTag);
        break;
      case 'checkboxes':
      case 'selects':
        $aliases = array();
        foreach($Data->$fieldTag as $ValueArray){
          $aliases[] = $ValueArray[1];
        }
        $output = MiniFieldCommon::JoinedByNewline($aliases);
        break;
      case 'files':
        foreach($Data->$fieldTag as $file){
          if($file->isimage){
            $imagestyle = array();
            isset($Options['image_width']) ? $imagestyle[] = 'width: ' . $Options['image_width'] . ';' : null;
            isset($Options['image_height']) ? $imagestyle[] = 'height: ' . $Options['image_height'] . ';' : null;
            $imagestyle = !empty($imagestyle) ? ' style="' .  implode('', $imagestyle) . '"' : '';
            $output = '<a target="_blank" href="' . $file->filepath . '" title="' . $file->filename . '"><img' . $imagestyle . ' src="' . $file->filepath . '" alt="' . $file->filename . '" /></a>';
          }else{
            $output = '<a target="_blank" href="' . $file->filepath . '">' . $file->filename . '</a>';
          }
        }
        break;
      /******* extend single editors *******/
      case 'textselect':
        $aliases = array();
        foreach($Data->$fieldTag as $ValueArray){
          $aliases[] = $ValueArray[1];
        }
        $output = MiniFieldCommon::JoinedByNewline($aliases);
        break;
      case 'texttags':
        $output = $Data->$fieldTag;
        break;
      case 'pcas':
        $output = MiniFieldCommon::JoinedByNewline($Data->$fieldTag);
        break;
      case 'pcasdict':
        $output = $Data->$fieldTag;
        /* $output = '<script type="text/javascript" src="' . base_path() . 'misc/resources/jquery/pcasdict/pcasdict.js"></script>';
        $output .= '<script type="text/javascript">document.write(jQuery.pcasAddress(' . $Data->$fieldTag . '));</script>'; */
        break;
      case 'datepicker':
        $output = $Data->$fieldTag->date . ' ' . $Data->$fieldTag->hour . ':' . $Data->$fieldTag->minute . ':' . $Data->$fieldTag->second;
        break;
      case 'farbtastic':
        $output = '<span style="color: ' . $Data->$fieldTag . ';">' . $Data->$fieldTag . '</span>';
        break;
      case 'baidumappoint':
      case 'browserfile':
        $output = $Data->$fieldTag;
        break;
      case 'ckeditor':
      case 'ueditor':
        //waiting to do
        break;
      /******* extend multiple editors *******/
    }
    return $output;
  }
  /* static end */
}