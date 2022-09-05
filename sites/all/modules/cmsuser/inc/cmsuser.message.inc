<?php
function cmsuser_message_send(){
  if(!isset($_POST['receiver']) || !isset($_POST['message'])){
    echo json_encode(array('status' => 'error', 'text' => '缺少参数'));exit;
  }
  $result = CMSUserMessage::Send($GLOBALS['user']->uid, $_POST['receiver'], 1, 2, $_POST['message']);
  echo json_encode($result);exit;
}

function cmsuser_message_senderstatus($id){
  if(!isset($_POST['senderstatus']) || !in_array($_POST['senderstatus'], array(1, 0, '1','0'))){
    echo json_encode(array('status' => 'error', 'text' => '缺少参数'));exit;
  }
  $Data = MiniFieldData::LoadDataByCidId(CMSUSER_CID_MESSAGE, $id, false);
  if(!isset($Data->id)){
    echo json_encode(array('status' => 'error', 'text' => '消息不存在'));exit;
  }else if($Data->sender != $GLOBALS['user']->uid){
    echo json_encode(array('status' => 'error', 'text' => '拒绝访问'));exit;
  }
  $result = CMSUserMessage::SenderStatusUpdate($id, $_POST['senderstatus']);
  echo json_encode($result);exit;
}

function cmsuser_message_receiverstatus($id){
  if(!isset($_POST['receiverstatus']) || !in_array($_POST['receiverstatus'], array(2, 1, 0, '2', '1','0'))){
    echo json_encode(array('status' => 'error', 'text' => '缺少参数'));exit;
  }
  $Data = MiniFieldData::LoadDataByCidId(CMSUSER_CID_MESSAGE, $id, false);
  if(!isset($Data->id)){
    echo json_encode(array('status' => 'error', 'text' => '消息不存在'));exit;
  }else if($Data->receiver != $GLOBALS['user']->uid){
    echo json_encode(array('status' => 'error', 'text' => '拒绝访问'));exit;
  }
  $result = CMSUserMessage::ReceiverStatusUpdate($id, $_POST['receiverstatus']);
  echo json_encode($result);exit;
}