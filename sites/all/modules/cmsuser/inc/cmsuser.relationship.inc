<?php
function cmsuser_relationship(){
  if(!isset($_POST['action']) || !isset($_POST['userid'])){
    echo json_encode(array('status' => 'error', 'text' => '缺少参数'));exit;
  }
  if($_POST['action'] == 'apply'){
    $result = CMSUserRelationship::Apply($GLOBALS['user']->uid, $_POST['userid']);
    if($result['status'] == 'success'){
      CMSUserMessage::Send(1, $GLOBALS['user']->uid, 1, 2, '您申请添加用户 ' . MiniFieldCommon::PlaceholderConvert('userid', $_POST['userid']) . ' 为好友。');
      CMSUserMessage::Send(1, $_POST['userid'], 1, 2, '用户 ' . MiniFieldCommon::PlaceholderConvert('userid', $GLOBALS['user']->uid) . ' 申请添加您为好友。');
    }
    echo json_encode($result);exit;
  }else if($_POST['action'] == 'unapply'){
    $result = CMSUserRelationship::Delete($GLOBALS['user']->uid, $_POST['userid']);
    if($result['status'] == 'success'){
      CMSUserMessage::Send(1, $GLOBALS['user']->uid, 1, 2, '您撤销添加用户 ' . MiniFieldCommon::PlaceholderConvert('userid', $_POST['userid']) . ' 为好友。');
      CMSUserMessage::Send(1, $_POST['userid'], 1, 2, '用户 ' . MiniFieldCommon::PlaceholderConvert('userid', $GLOBALS['user']->uid) . ' 撤销添加您为好友。');
    }
    echo json_encode($result);exit;
  }else if($_POST['action'] == 'accept'){
    $result = CMSUserRelationship::Accept($GLOBALS['user']->uid, $_POST['userid']);
    if($result['status'] == 'success'){
      CMSUserMessage::Send(1, $GLOBALS['user']->uid, 1, 2, '您接受了用户 ' . MiniFieldCommon::PlaceholderConvert('userid', $_POST['userid']) . ' 的好友请求。');
      CMSUserMessage::Send(1, $_POST['userid'], 1, 2, '用户 ' . MiniFieldCommon::PlaceholderConvert('userid', $GLOBALS['user']->uid) . ' 接受了您的好友请求。');
    }
    echo json_encode($result);exit;
  }else if($_POST['action'] == 'refuse'){
    $result = CMSUserRelationship::Delete($GLOBALS['user']->uid, $_POST['userid']);
    if($result['status'] == 'success'){
      CMSUserMessage::Send(1, $GLOBALS['user']->uid, 1, 2, '您拒绝了用户 ' . MiniFieldCommon::PlaceholderConvert('userid', $_POST['userid']) . ' 的好友请求。');
      CMSUserMessage::Send(1, $_POST['userid'], 1, 2, '用户 ' . MiniFieldCommon::PlaceholderConvert('userid', $GLOBALS['user']->uid) . ' 拒绝了您的好友请求。');
    }
    echo json_encode($result);exit;
  }else if($_POST['action'] == 'delete'){
    $result = CMSUserRelationship::Delete($GLOBALS['user']->uid, $_POST['userid']);
    if($result['status'] == 'success'){
      CMSUserMessage::Send(1, $GLOBALS['user']->uid, 1, 2, '您解除了与用户 ' . MiniFieldCommon::PlaceholderConvert('userid', $_POST['userid']) . ' 的关系。');
      CMSUserMessage::Send(1, $_POST['userid'], 1, 2, '用户 ' . MiniFieldCommon::PlaceholderConvert('userid', $GLOBALS['user']->uid) . ' 解除了与您的关系。');
    }
    echo json_encode($result);exit;
  }
  exit;
}