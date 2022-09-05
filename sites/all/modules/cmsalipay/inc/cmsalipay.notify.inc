<?php
function cmsalipay_notify(){
  include_once CMSALIPAY_MODULE_PATH . "/cmsalipay.config.inc";
  include_once CMSALIPAY_MODULE_PATH . "/lib/cmsalipay.notify.inc";
  $alipayNotify = new CmsAlipayNotify($aliapy_config);
  $verify_result = $alipayNotify->VerifyNotify(CMSALIPAY_MODULE_PATH . "/log-notify.txt");
  if($verify_result){
    //获取支付宝的通知返回参数，可参考技术文档中服务器异步通知参数列表
    $out_trade_no = $_POST['out_trade_no'];//商户网站唯一订单号
    $trade_no = $_POST['trade_no'];//创建订单生成的交易号
    $price = $_POST['price'];//商品单价
    $total_fee = $_POST['total_fee'];//交易金额
    if($_POST['trade_status'] == 'WAIT_BUYER_PAY'){
      //该判断表示买家已在支付宝交易管理中产生了交易记录，但没有付款
      echo "success";//required
    }else if($_POST['trade_status'] == 'WAIT_SELLER_SEND_GOODS'){
      //该判断表示买家已在支付宝交易管理中产生了交易记录且付款成功，但卖家没有发货
      echo "success";//required
    }else if($_POST['trade_status'] == 'WAIT_BUYER_CONFIRM_GOODS'){
      //该判断表示卖家已经发了货，但买家还没有做确认收货的操作
      echo "success";//required
    }else if($_POST['trade_status'] == 'TRADE_FINISHED' || $_POST['trade_status'] == 'TRADE_SUCCESS'){
      //该判断表示买家已经确认收货，这笔交易完成
      echo "success";//required
    }else if($_POST['trade_status'] == 'TRADE_CLOSED'){
      echo "success";//required
    }else{
      //其他状态判断
      echo "success";
    }
  }else{
    //验证失败
    echo "fail";
  }
  exit;
}