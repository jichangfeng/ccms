<?php
/**
 * 日期：2012-09
 * 说明：
 *  - 总金额计算方式是：总金额 = price * quantity + logistics_fee + discount。
 *  - 建议把 price 看作为总金额，是物流运费、折扣、购物车中购买商品总额等计算后的最终订单的应付总额。
 *  - 建议物流参数只使用一组，根据买家在商户网站中下单时选择的物流类型（快递、平邮、EMS），程序自动识别 logistics_type 被赋予三个中的一个值。
 *  - 各家快递公司都属于EXPRESS（快递）的范畴。
 *
 * 注意，如果在接口集成过程中遇到问题，可以按照下面的途径来解决：
 *  - 1、商户服务中心（https://b.alipay.com/support/helperApply.htm?action=consultationApply），提交申请集成协助，我们会有专业的技术工程师主动联系您协助解决
 *  - 2、商户帮助中心（http://help.alipay.com/support/232511-16307/0-16307.htm?sh=Y&info_type=9）
 *  - 3、支付宝论坛（http://club.alipay.com/read-htm-tid-8681712.html）
 *  - 如果不想使用扩展功能请把扩展功能参数赋空值。
 * @author Changfeng Ji <jichf@qq.com>
 */
final class CMSAlipay{
  /**
   * 标准双接口接入.
   * @access public
   * @static
   * @param array $parameters 请求参数数组。
   *
   * 必需传递的参数包括：
   *  - out_trade_no、subject、logistics_type、logistics_fee、logistics_payment、price
   * 
   * 可选传递的参数包括：
   *  - body、show_url、receive_name、receive_address、receive_zip、receive_phone、receive_mobile
   *
   * 部分数组元素说明：
   *  - service: 接口名称，不可空，这里为标准双接口 trade_create_by_buyer（不需要传递）
   *  - partner: 合作身份者ID，不可空（不需要传递）
   *  - _input_charset: 参数编码字符集，不可空，这里为 utf-8（不需要传递）
   *  - sign_type: 签名方式，不可空，这里为 MD5（不需要传递）
   *  - sign: 密钥，不可空（不需要传递）
   *  - notify_url: 合服务器异步通知页面路径，可空，建议使用异步通知（不需要传递）
   *  - return_url: 页面跳转同步通知页面路径，可空，不建议使用同步通知（不需要传递）
   *  -
   *  - out_trade_no: 商户网站唯一订单号，不可空（必须传递）
   *  - subject: 商品名称，不可空（必须传递）
   *  - payment_type: 收款类型，不可空，这里为值 1，代表商品购买（不需要传递）
   *  - logistics_type: 物流类型，不可空，POST、EXPRESS、EMS 三个值之一（必须传递）
   *  - logistics_fee: 物流费用，不可空，精确到小数点后两位（必须传递）
   *  - logistics_payment: 物流支付类型，不可空，BUYER_PAY、SELLER_PAY、BUYER_PAY_AFTER_RECEIVE 三个值之一（必须传递）
   *  - seller_email: 卖家支付宝账号，不可空（不需要传递）
   *  - price: 商品单价，不可空，这里作为订单商品总金额（必须传递）
   *  - quantity: 商品数量，不可空，这里为 1，把一次交易看成是一次下订单而非购买一件商品（不需要传递）
   *  - body: 商品描述，可空（可选传递）
   *  - discount: 折扣，可空，不使用此参数（不需要传递）
   *  - total_fee: 交易金额，可空，不使用此参数（不需要传递）
   *  - show_url: 商品展示URL，可空，收银台页面上，商品展示的超链接（可选传递）
   *  - seller_id: 卖家支付宝账号对应的支付宝唯一用户号，可空（不需要传递）
   *  - receive_name: 收货人姓名，可空（可选传递）
   *  - receive_address: 收货人地址，可空（可选传递）
   *  - receive_zip: 收货人邮编，可空（可选传递）
   *  - receive_phone: 收货人电话，可空（可选传递）
   *  - receive_mobile: 收货人手机，可空（可选传递）
   * @return string 提交表单HTML文本
   */
  public static function Payment_dualfun($parameters){
    include_once CMSALIPAY_MODULE_PATH . "/cmsalipay.config.inc";
    include_once CMSALIPAY_MODULE_PATH . "/lib/cmsalipay.service.inc";
    $parameters_new = array(
      "service" => "trade_create_by_buyer",
      "partner" => trim($aliapy_config['partner']),
      "_input_charset" => trim(strtolower($aliapy_config['input_charset'])),
      //"sign_type" => "",不需要传递，后续组装
      //"sign" => "",不需要传递，后续生成
      "notify_url" => trim($aliapy_config['notify_url']),
      "return_url" => trim($aliapy_config['return_url']),

      "payment_type" => 1,
      "seller_email" => trim($aliapy_config['seller_email']),
      "quantity" => 1,
      //"discount" => 0,
      //"total_fee" => 0,
      //"seller_id" => "",
      "subject" => "",
    );
    $parameters_new['out_trade_no'] = $parameters['out_trade_no'];
    $parameters_new['subject'] = $parameters['subject'];
    $parameters_new['logistics_type'] = $parameters['logistics_type'];
    $parameters_new['logistics_fee'] = $parameters['logistics_fee'];
    $parameters_new['logistics_payment'] = $parameters['logistics_payment'];
    $parameters_new['price'] = $parameters['price'];
    if(isset($parameters['body'])){ $parameters_new['body'] = $parameters['body']; }
    if(isset($parameters['show_url'])){ $parameters_new['show_url'] = $parameters['show_url']; }
    if(isset($parameters['receive_name'])){ $parameters_new['receive_name'] = $parameters['receive_name']; }
    if(isset($parameters['receive_name'])){ $parameters_new['receive_address'] = $parameters['receive_address']; }
    if(isset($parameters['receive_zip'])){ $parameters_new['receive_zip'] = $parameters['receive_zip']; }
    if(isset($parameters['receive_phone'])){ $parameters_new['receive_phone'] = $parameters['receive_phone']; }
    if(isset($parameters['receive_mobile'])){ $parameters_new['receive_mobile'] = $parameters['receive_mobile']; }
    $alipayService = new CmsAlipayService($aliapy_config);
    $html_text = $alipayService->trade_create_by_buyer($parameters_new);
    return $html_text;
  }
  
  /**
   * 确认发货接口.
   * @access public
   * @static
   * @param array $parameters 请求参数数组。
   *
   * 必需传递的参数包括：
   *  - trade_no、logistics_name
   *
   * 可选传递的参数包括：
   *  - invoice_no、transport_type、create_transport_type、seller_ip
   *
   * 部分数组元素说明：
   *  - service: 接口名称，不可空，这里为标准双接口 trade_create_by_buyer（不需要传递）
   *  - partner: 合作身份者ID，不可空（不需要传递）
   *  - _input_charset: 参数编码字符集，不可空，这里为 utf-8（不需要传递）
   *  - sign_type: 签名方式，不可空，这里为 MD5（不需要传递）
   *  - sign: 密钥，不可空（不需要传递）
   *  -
   *  - trade_no: 支付宝交易号，不可空（必须传递）
   *  - logistics_name: 物流公司名称，不可空（必须传递）
   *  - invoice_no: 物流发货单号，可空（可选传递）
   *  - transport_type: 发货时的运输类型，可空，POST、EXPRESS、EMS 三个值之一（可选传递）
   *  - create_transport_type: 创建交易时的运输类型，可空，POST、EXPRESS、EMS 三个值之一（可选传递）
   *  - seller_ip: 卖家IP，可空（可选传递）
   * @return \DOMDocument 支付宝返回XML处理结果
   */
  public static function Payment_sendgoods($parameters){
    include_once CMSALIPAY_MODULE_PATH . "/cmsalipay.config.inc";
    include_once CMSALIPAY_MODULE_PATH . "/lib/cmsalipay.service.inc";
    $parameters_new = array(
      "service" => "send_goods_confirm_by_platform",
      "partner" => trim($aliapy_config['partner']),
      "_input_charset" => trim(strtolower($aliapy_config['input_charset'])),
      //"sign_type" => "",不需要传递，后续组装
      //"sign" => "",不需要传递，后续生成
    );
    $parameters_new['trade_no'] = $parameters['trade_no'];
    $parameters_new['logistics_name'] = $parameters['logistics_name'];
    if(isset($parameters['invoice_no'])){ $parameters_new['invoice_no'] = $parameters['invoice_no']; }
    if(isset($parameters['transport_type'])){ $parameters_new['transport_type'] = $parameters['transport_type']; }
    if(isset($parameters['create_transport_type'])){ $parameters_new['create_transport_type'] = $parameters['create_transport_type']; }
    if(isset($parameters['seller_ip'])){ $parameters_new['seller_ip'] = $parameters['seller_ip']; }
    $alipayService = new CmsAlipayService($aliapy_config);
    $xml_dom = $alipayService->send_goods_confirm_by_platform($parameters_new);
    return $xml_dom;
  }

  /**
   * 关闭交易接口.
   * @access public
   * @static
   * @param array $parameters 请求参数数组。
   *
   * 必需传递的参数包括：
   *  - trade_no/out_order_no
   *
   * 可选传递的参数包括：
   *  - ip、trade_role
   *
   * 部分数组元素说明：
   *  - service: 接口名称，不可空，这里为标准双接口 trade_create_by_buyer（不需要传递）
   *  - partner: 合作身份者ID，不可空（不需要传递）
   *  - _input_charset: 参数编码字符集，不可空，这里为 utf-8（不需要传递）
   *  - sign_type: 签名方式，不可空，这里为 MD5（不需要传递）
   *  - sign: 密钥，不可空（不需要传递）
   *  -
   *  - trade_no: 支付宝交易号，可空（可选传递，trade_no和out_order_no不可同时为空）
   *  - out_order_no: 商户网站唯一订单号，可空（可选传递，trade_no和out_order_no不可同时为空）
   *  - ip: ip地址，可空（可选传递）
   *  - trade_role: 角色，可空，买家－B；卖家－S（可选传递）
   * @return \DOMDocument 支付宝返回XML处理结果
   */
  public static function Payment_closetrade($parameters){
    include_once CMSALIPAY_MODULE_PATH . "/cmsalipay.config.inc";
    include_once CMSALIPAY_MODULE_PATH . "/lib/cmsalipay.service.inc";
    $parameters_new = array(
      "service" => "close_trade",
      "partner" => trim($aliapy_config['partner']),
      "_input_charset" => trim(strtolower($aliapy_config['input_charset'])),
      //"sign_type" => "",不需要传递，后续组装
      //"sign" => "",不需要传递，后续生成
    );
    if(isset($parameters['trade_no'])){ $parameters_new['trade_no'] = $parameters['trade_no']; }
    if(isset($parameters['out_order_no'])){ $parameters_new['out_order_no'] = $parameters['out_order_no']; }
    if(isset($parameters['ip'])){ $parameters_new['ip'] = $parameters['ip']; }
    if(isset($parameters['trade_role'])){ $parameters_new['trade_role'] = $parameters['trade_role']; }
    $alipayService = new CmsAlipayService($aliapy_config);
    $xml_dom = $alipayService->close_trade($parameters_new);
    return $xml_dom;
  }

  /**
   * 即时到账接入.
   * @access public
   * @static
   * @param array $parameters 请求参数数组。
   *
   * 必需传递的参数包括：
   *  - out_trade_no、subject、total_fee
   * 
   * 可选传递的参数包括：
   *  - body、show_url
   *
   * 部分数组元素说明：
   *  - service: 接口名称，不可空，这里为标准双接口 trade_create_by_buyer（不需要传递）
   *  - partner: 合作身份者ID，不可空（不需要传递）
   *  - _input_charset: 参数编码字符集，不可空，这里为 utf-8（不需要传递）
   *  - sign_type: 签名方式，不可空，这里为 MD5（不需要传递）
   *  - sign: 密钥，不可空（不需要传递）
   *  - notify_url: 合服务器异步通知页面路径，可空，建议使用异步通知（不需要传递）
   *  - return_url: 页面跳转同步通知页面路径，可空，不建议使用同步通知（不需要传递）
   *  -
   *  - out_trade_no: 商户网站唯一订单号，不可空（必须传递）
   *  - subject: 商品名称，不可空（必须传递）
   *  - payment_type: 收款类型，不可空，这里为值 1，代表商品购买（不需要传递）
   *  - seller_email: 卖家支付宝账号，不可空（不需要传递）
   *  - body: 商品描述，可空（可选传递）
   *  - total_fee: 交易金额，不可空（必须传递）
   *  - show_url: 商品展示URL，可空，收银台页面上，商品展示的超链接（可选传递）
   *  - seller_id: 卖家支付宝账号对应的支付宝唯一用户号，可空（不需要传递）
   * @return string 提交表单HTML文本
   */
  public static function Payment_direct($parameters){
    include_once CMSALIPAY_MODULE_PATH . "/cmsalipay.config.inc";
    include_once CMSALIPAY_MODULE_PATH . "/lib/cmsalipay.service.inc";
    $parameters_new = array(
      "service" => "create_direct_pay_by_user",
      "partner" => trim($aliapy_config['partner']),
      "_input_charset" => trim(strtolower($aliapy_config['input_charset'])),
      //"sign_type" => "",不需要传递，后续组装
      //"sign" => "",不需要传递，后续生成
      "notify_url" => trim($aliapy_config['notify_url']),
      "return_url" => trim($aliapy_config['return_url']),

      "payment_type" => 1,
      "seller_email" => trim($aliapy_config['seller_email']),
      //"seller_id" => "",
    );
    $parameters_new['out_trade_no'] = $parameters['out_trade_no'];
    $parameters_new['subject'] = $parameters['subject'];
    $parameters_new['total_fee'] = $parameters['total_fee'];
    if(isset($parameters['body'])){ $parameters_new['body'] = $parameters['body']; }
    if(isset($parameters['show_url'])){ $parameters_new['show_url'] = $parameters['show_url']; }
    $alipayService = new CmsAlipayService($aliapy_config);
    $html_text = $alipayService->create_direct_pay_by_user($parameters_new);
    return $html_text;
  }
  
  public static function Payment_dualfun_test($order_id){
    if(empty($order_id)){ return; }
    //订单商品
    $order_products = array();
    $product = array(
      'id' => 1, 'order_product_id' => 7, 'order_id' => $order_id,
      'title' => '测试商品1号', 'qty' => 2, 'price' => 0.01, 'data' => array()
    );
    $order_products[] = (object)$product;
    $product = array(
      'id' => 2, 'order_product_id' => 8, 'order_id' => $order_id,
      'title' => '测试商品2号', 'qty' => 1, 'price' => 0.01, 'data' => array()
    );
    $order_products[] = (object)$product;
    //订单
    $order = new stdClass();
    $order->id = 1;
    $order->order_id = $order_id;
    $order->user_id = $GLOBALS['user']->uid;
    $order->order_total = 0;
    $order->product_count = 0;
    $order->order_status = '';
    $order->payment_method = 'alipay_dualfun';
    //$order->payment_method2 = '';//如果是标准双接口支付，可用于判断具体支付方式：alipayescow、alipaydirect

    $order->logistics_type = 'EMS';
    $order->logistics_fee = '0.01';
    $order->logistics_payment = 'SELLER_PAY';
    $order->receive_name = '姬常峰';
    $order->receive_address = '北京市朝阳区红军营南路媒体村天畅园8号楼503室';
    $order->receive_zip = '100012';
    $order->receive_phone = '01084829710';
    $order->receive_mobile = '13621308927';
    $order->billing_name = '';
    $order->billing_address = '';
    $order->billing_zip = '';
    $order->billing_phone = '';
    $order->billing_mobile = '';

    $order->trade_no = '';
    $order->logistics_name = '';
    $order->invoice_no = '';
    $order->transport_type = '';
    $order->create_transport_type = '';
    $order->seller_ip = '';

    $order->data = array();
    $order->created = time();
    $order->modified = time();
    $order->host = MiniFieldCommon::GetIp();
    //订单日志
    $order_logs = array();
    $log = array(
      'order_log_id' => 1, 'order_id' => $order_id, 'uid' => $GLOBALS['user']->uid,
      'changes' => '创建订单', 'created' =>  time()
    );
    $order_logs[] = (object)$log;
    //订单支付处理
    $order->order_total = 0;
    $order->product_count = 0;
    $subject = array();
    foreach($order_products as $product){
      $order->order_total += $product->price * $product->qty;
      $order->product_count += $product->qty;
      $subject[] = $product->title;
    }
    $subject = implode(', ', $subject);//注意长度范围 String(256)
    $parameters = array(
      'out_trade_no' => $order->order_id,
      'subject' => $subject,
      'logistics_type' => $order->logistics_type,
      'logistics_fee' => $order->logistics_fee,
      'logistics_payment' => $order->logistics_payment,
      'price' => $order->order_total,

      'body' => '2个测试商品',
      'show_url' => $GLOBALS['base_url'],
      'receive_name' => $order->receive_name,
      'receive_address' => $order->receive_address,
      'receive_zip' => $order->receive_zip,
      'receive_phone' => $order->receive_phone,
      'receive_mobile' => $order->receive_mobile
    );
    header('Content-type: text/html;charset=utf-8');
    if($order->payment_method == 'alipay_dualfun'){
      echo self::Payment_dualfun($parameters);
    }else{
      echo '不支持的支付方式：' . $order->payment_method;
    }
  }
  public static function Payment_sendgoods_test($order_id){
    if(empty($order_id)){ return; }
    //订单
    $order = new stdClass();
    $order->id = 1;
    $order->order_id = $order_id;
    $order->user_id = $GLOBALS['user']->uid;
    $order->trade_no = '';
    $order->logistics_name = '';
    $order->invoice_no = '';
    $order->transport_type = '';
    $order->create_transport_type = '';
    $order->seller_ip = '';
    $parameters = array(
      'trade_no' => $order->trade_no,
      'logistics_name' => $order->logistics_name,

      'invoice_no' => $order->invoice_no,
      'transport_type' => $order->transport_type,
      'create_transport_type' => $order->create_transport_type,
      'seller_ip' => $order->seller_ip
    );
    $xml_dom = self::Payment_sendgoods($parameters);
    header('Content-type: text/html;charset=utf-8');
    echo $xml_dom->saveXML();
    /* $response = "";
    if(!empty($xml_dom->getElementsByTagName("response")->item(0)->nodeValue)){
      $response = $xml_dom->getElementsByTagName("response")->item(0)->nodeValue;
    }
    echo $response; */
  }
  public static function Payment_closetrade_test($order_id){
    if(empty($order_id)){ return; }
    //订单
    $order = new stdClass();
    $order->id = 1;
    $order->order_id = $order_id;
    $order->user_id = $GLOBALS['user']->uid;
    $order->trade_no = '';
    $order->host = MiniFieldCommon::GetIp();
    $parameters = array(
      'trade_no' => $order->trade_no,
      'out_order_no' => $order->order_id,

      'ip' => $order->ip,
      'trade_role' => 'S'
    );
    $xml_dom = self::Payment_closetrade($parameters);
    header('Content-type: text/html;charset=utf-8');
    echo $xml_dom->saveXML();
    /* //获取成功标识is_success
    $itemIs_success = $xml_dom->getElementsByTagName("is_success");
    $nodeIs_success = $itemIs_success->item(0)->nodeValue;//T代表成功，F代表失败
    echo 'Is_success: ' . $nodeIs_success;
    if($nodeIs_success == 'F'){
      //获取错误代码 error
      $itemError_code = $xml_dom->getElementsByTagName("error");
      $nodeError_code = $itemError_code->item(0)->nodeValue;
      echo 'Error_code: ' . $nodeError_code;
    } */
  }
  public static function Payment_direct_test($order_id){
    if(empty($order_id)){ return; }
    //订单
    $order = new stdClass();
    $order->id = 1;
    $order->order_id = $order_id;
    $order->created = time();
    $order->userid = $GLOBALS['user']->uid;
    $order->money = 0.01;
    $order->type = 'recharge';
    $order->accounttype = 'alipay';
    $order->account = '';
    $order->accountname = '';
    $order->accountphone = '';
    $order->status = 1;

    $order->trade_no = '';

    $order->data = array();
    $order->host = MiniFieldCommon::GetIp();
    $subject = '充值';
    $parameters = array(
      'out_trade_no' => $order->order_id,
      'subject' => $subject,
      'total_fee' => $order->money
    );
    header('Content-type: text/html;charset=utf-8');
    echo self::Payment_direct($parameters);
  }
}