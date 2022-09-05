<?php
/**
 * @author Changfeng Ji <jichf@qq.com>
 */
final class CMSUserMessage{
  /**
   * 
   * @access public
   * @static
   * @param int $sender The sender user id.
   * @param int $receiver The receiver user id.
   * @param int $senderstatus The sender status. 1 = show; 0 = hidden.
   * @param int $receiverstatus The receiver status. 2 = unread; 1 = read; 0 = hidden.
   * @param string $message
   * @return array A array with two elements('status' => '$status', 'text' => '$text'):
   *  - $status: The status value 'error' or 'success'.
   *  - $text: The status text.
   */
  public static function Send($sender, $receiver, $senderstatus, $receiverstatus, $message){
    if(!is_numeric($sender)){ return array('status' => 'error', 'text' => '参数不合法'); }
    if(!is_numeric($receiver)){ return array('status' => 'error', 'text' => '参数不合法'); }
    if(!in_array($senderstatus, array(1, 0, '1','0'))){ return array('status' => 'error', 'text' => '参数不合法'); }
    if(!in_array($receiverstatus, array(2, 1, 0, '2', '1','0'))){ return array('status' => 'error', 'text' => '参数不合法'); }
    if(preg_match('/[^\s]+/', $message) == 0){
      return array('status' => 'error', 'text' => '消息必须填写');
    }
    $tablename = minifield_tablename_load(CMSUSER_CID_MESSAGE);
    if(empty($tablename)){ return array('status' => 'error', 'text' => '未知错误'); }
    $result = db_insert($tablename)
      ->fields(array(
        'created' => time(),
        'sender' => $sender,
        'receiver' => $receiver,
        'senderstatus' => $senderstatus,
        'receiverstatus' => $receiverstatus,
        'message' => $message
      ))
      ->execute();
    if((is_numeric($result) && preg_match('/^\d+$/', $result) > 0)){
      return array('status' => 'success', 'text' => '发送成功');
    }else{
      return array('status' => 'error', 'text' => '未知错误');
    }
  }

  /**
   * 
   * @access public
   * @static
   * @param int $id The message id.
   * @param int $senderstatus The sender status. 1 = show; 0 = hidden.
   * @return array A array with two elements('status' => '$status', 'text' => '$text'):
   *  - $status: The status value 'error' or 'success'.
   *  - $text: The status text.
   */
  public static function SenderStatusUpdate($id, $senderstatus){
    if(!is_numeric($id)){ return array('status' => 'error', 'text' => '参数不合法'); }
    if(!in_array($senderstatus, array(1, 0, '1','0'))){ return array('status' => 'error', 'text' => '参数不合法'); }
    $Data = MiniFieldData::LoadDataByCidId(CMSUSER_CID_MESSAGE, $id, false);
    if(!isset($Data->id)){
      return json_encode(array('status' => 'error', 'text' => '消息不存在'));
    }
    $tablename = minifield_tablename_load(CMSUSER_CID_MESSAGE);
    if(empty($tablename)){ return array('status' => 'error', 'text' => '未知错误'); }
    db_update($tablename)
      ->fields(array('senderstatus' => $senderstatus))
      ->condition('id', $id)
      ->execute();
    return array('status' => 'success', 'text' => '执行成功');
  }

  /**
   * 
   * @access public
   * @static
   * @param int $id The message id.
   * @param int $receiverstatus The receiver status. 2 = unread; 1 = read; 0 = hidden.
   * @return array A array with two elements('status' => '$status', 'text' => '$text'):
   *  - $status: The status value 'error' or 'success'.
   *  - $text: The status text.
   */
  public static function ReceiverStatusUpdate($id, $receiverstatus){
    if(!is_numeric($id)){ return array('status' => 'error', 'text' => '参数不合法'); }
    if(!in_array($receiverstatus, array(2, 1, 0, '2', '1','0'))){ return array('status' => 'error', 'text' => '参数不合法'); }
    $Data = MiniFieldData::LoadDataByCidId(CMSUSER_CID_MESSAGE, $id, false);
    if(!isset($Data->id)){
      return json_encode(array('status' => 'error', 'text' => '消息不存在'));
    }
    $tablename = minifield_tablename_load(CMSUSER_CID_MESSAGE);
    if(empty($tablename)){ return array('status' => 'error', 'text' => '未知错误'); }
    db_update($tablename)
      ->fields(array('receiverstatus' => $receiverstatus))
      ->condition('id', $id)
      ->execute();
    return array('status' => 'success', 'text' => '执行成功');
  }

  /**
   * 
   * @access public
   * @static
   * @param int $id The message id.
   */
  public static function Delete($id){
    MiniFieldData::DeleteDataByCidId(CMSUSER_CID_MESSAGE, $id, true);
  }

  /**
   * 
   * @access public
   * @static
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
   *       - The available field: 'created', 'sender', 'receiver', 'senderstatus', 'receiverstatus', 'message'.
   *       - The available value: created(Unix timestamp), sender(user id), receiver(user id), senderstatus(1 = show; 0 = hidden), receiverstatus(2 = unread; 1 = read; 0 = hidden), message(string).
   * @param array $orderbys Orders the result set by a given field.
   *      Each element is still a array with one to two elements($field[, $direction]):
   *       - $field: The field on which to order.
   *       - $direction: The direction to sort. Legal values are "ASC" or "DESC" or "RANDOM"($field is invalid), Default to "ASC".
   * @param type $limit The page limit or null.
   * @param boolean $format whether to format loaded data or not.
   * @param int $element An optional integer to distinguish between multiple pagers on one page.
   * @return array The filtered data array.
   */
  public static function LoadMessages($conditions = array(), $orderbys = array(), $limit = null, $format = false, $element = 0){
    return MiniFieldData::FilterDatasByCid(CMSUSER_CID_MESSAGE, array(), array(), $conditions, $orderbys, $limit, $format, $element);
  }

  /**
   * 
   * @access public
   * @static
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
   *       - The available field: 'created', 'sender', 'receiver', 'senderstatus', 'receiverstatus', 'message'.
   *       - The available value: created(Unix timestamp), sender(user id), receiver(user id), senderstatus(1 = show; 0 = hidden), receiverstatus(2 = unread; 1 = read; 0 = hidden), message(string).
   * @return int The number of data count. -1 if error.
   */
  public static function LoadMessageCount($conditions = array()){
    return MiniFieldData::LoadDataCountByCid(CMSUSER_CID_MESSAGE, $conditions);
  }
}