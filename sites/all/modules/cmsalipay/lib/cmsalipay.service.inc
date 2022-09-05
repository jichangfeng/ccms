<?php
require_once "cmsalipay.submit.inc";
/**
 * 支付宝各接口构造类
 * 详细：构造支付宝各接口请求参数
 * 日期：2012-09
 * @author Changfeng Ji <jichf@qq.com>
 */
final class CmsAlipayService{
  /**
   * 支付宝基本配置信息:
   *  - 比如包含子元素：partner, key, seller_email, return_url, notify_url, sign_type, input_charset, transport.
   *  - 当前必须的子元素：partner, key, sign_type, input_charset.
   * @var array
   */
  public $aliapy_config;
  /**
   * 支付宝网关地址
   * @var string
   */
  public $gateway = 'https://mapi.alipay.com/gateway.do?';

  /**
   *
   * @param array $aliapy_config 支付宝基本配置信息:
   *  - 比如包含子元素：partner, key, seller_email, return_url, notify_url, sign_type, input_charset, transport.
   *  - 当前必须的子元素：partner, key, sign_type, input_charset.
   */
  public function __construct($aliapy_config){
    $this->aliapy_config = $aliapy_config;
  }

  /**
   * 构造标准双接口
   * @access public
   * @param array $parameters 请求参数数组
   * @return string 提交表单HTML文本
   */
  public function trade_create_by_buyer($parameters){
    //设置按钮名称
    $button_name = "确认";
    $method = 'get';
    //生成表单提交HTML文本信息
    $html_text = CmsAlipaySubmit::BuildForm($parameters, $this->gateway, $method, $button_name, $this->aliapy_config['key'], $this->aliapy_config['sign_type'], $this->aliapy_config['input_charset']);
    return $html_text;
  }

  /**
   * 构造确认发货接口
   * @access public
   * @param type $parameters 请求参数数组
   * @return \DOMDocument 支付宝返回XML处理结果
   */
  public function send_goods_confirm_by_platform($parameters){
    $xml_dom = CmsAlipaySubmit::SendPostInfo($parameters, $this->gateway, $this->aliapy_config['key'], $this->aliapy_config['sign_type'], $this->aliapy_config['input_charset']);
    return $xml_dom;
  }

  /**
   * 构造交易关闭接口
   * @access public
   * @param type $parameters 请求参数数组
   * @return \DOMDocument 支付宝返回XML处理结果
   */
  public function close_trade($parameters){
    $xml_dom = CmsAlipaySubmit::SendPostInfo($parameters, $this->gateway, $this->aliapy_config['key'], $this->aliapy_config['sign_type'], $this->aliapy_config['input_charset']);
    return $xml_dom;
  }

  /**
   * 构造即时到账接口
   * @access public
   * @param array $parameters 请求参数数组
   * @return string 提交表单HTML文本
   */
  public function create_direct_pay_by_user($parameters){
    //设置按钮名称
    $button_name = "确认";
    $method = 'get';
    //生成表单提交HTML文本信息
    $html_text = CmsAlipaySubmit::BuildForm($parameters, $this->gateway, $method, $button_name, $this->aliapy_config['key'], $this->aliapy_config['sign_type'], $this->aliapy_config['input_charset']);
    return $html_text;
  }

  /**
   * 用于防钓鱼，调用接口query_timestamp来获取时间戳的处理函数
   * 注意：该功能PHP5环境及以上支持，因此必须服务器、本地电脑中装有支持DOMDocument、SSL的PHP配置环境。建议本地调试时使用PHP开发软件
   * @access public
   * @return string 时间戳字符串
   */
  public function query_timestamp(){
    $url = $this->gateway . "service=query_timestamp&partner=" . trim($this->aliapy_config['partner']);
    $encrypt_key = "";
    $doc = new DOMDocument();
    $doc->load($url);
    $itemEncrypt_key = $doc->getElementsByTagName("encrypt_key");
    $encrypt_key = $itemEncrypt_key->item(0)->nodeValue;
    return $encrypt_key;
  }
  /**
   *
   * @access public
   * @param array $parameters 请求参数数组
   */
  public function alipay_interface($parameters){
    /*
     * 根据不同的接口特性，选择一种请求方式
     * 1.构造表单提交HTML数据:（$method可赋值为get或post）
     * CmsAlipaySubmit::BuildForm($parameters, $gateway, $method, $button_name, $key, $sign_type, $input_charset);
     * 2.构造模拟远程HTTP的POST请求，获取支付宝的返回XML处理结果:
     * 注意：若要使用远程HTTP获取数据，必须开通SSL服务，该服务请找到php.ini配置文件设置开启，建议与您的网络管理员联系解决。
     * CmsAlipaySubmit::SendPostInfo($parameters, $gateway, $key, $sign_type, $input_charset);
     */
  }
}