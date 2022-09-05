<?php
require_once "cmsalipay.core.inc";
/**
 * 支付宝通知处理类
 * 详细：处理支付宝各接口通知返回
 * 日期：2012-09
 * 注意：调试通知返回时，可查看或改写log日志的写入TXT里的数据，来检查通知返回是否正常
 * @author Changfeng Ji <jichf@qq.com>
 */
final class CmsAlipayNotify{
  /**
   * 支付宝基本配置信息:
   *  - 比如包含子元素：partner, key, seller_email, return_url, notify_url, sign_type, input_charset, transport.
   *  - 当前必须的子元素：partner, key, sign_type, input_charset, transport.
   * @var array
   */
  public $aliapy_config;
  /**
   * HTTPS形式消息验证地址
   * @var string
   */
  public $https_verify_url = 'https://mapi.alipay.com/gateway.do?service=notify_verify&';
  /**
   * HTTP形式消息验证地址
   * @var string
   */
  public $http_verify_url = 'http://notify.alipay.com/trade/notify_query.do?';

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
   * 针对notify_url验证消息是否是支付宝发出的合法消息
   * @access public
   * @param string $logpath 写日志路径
   * @return boolean 验证结果
   */
  public function VerifyNotify($logpath = ''){
    if(empty($_POST)){//判断POST来的数组是否为空
      return false;
    }else{
      //生成签名结果字符串
      $mysign = $this->GetMySign($_POST);
      //获取支付宝远程服务器ATN结果（验证是否是支付宝发来的消息）
      $responseTxt = 'true';
      if(!empty($_POST["notify_id"])){ $responseTxt = $this->GetResponse($_POST["notify_id"]); }
      //写日志记录
      if(!empty($logpath)){
        $log_text = "responseTxt=" . $responseTxt . "\n" . "notify_url_log:sign=" . $_POST["sign"] . "&mysign=" . $mysign.",";
        $log_text = $log_text . CmsAlipayCore::CreateLinkString($_POST);
        CmsAlipayCore::WriteLog($logpath, $log_text);
      }
      /*
       * 验证
       * $responseTxt的结果不是true，与服务器设置问题、合作身份者ID、notify_id一分钟失效有关
       * mysign与sign不等，与安全校验码、请求时的参数格式（如：带自定义参数等）、编码格式有关
       */
      if(preg_match("/true$/i", $responseTxt) && $mysign == $_POST["sign"]){
        return true;
      }else{
        return false;
      }
    }
  }

  /**
   * 针对return_url验证消息是否是支付宝发出的合法消息
   * @access public
   * @param string $logpath 写日志路径
   * @return boolean 验证结果
   */
  public function VerifyReturn($logpath = ''){
    if(empty($_GET)){//判断GET来的数组是否为空
      return false;
    }else{
      //生成签名结果字符串
      $mysign = $this->GetMySign($_GET);
      //获取支付宝远程服务器ATN结果（验证是否是支付宝发来的消息）
      $responseTxt = 'true';
      if(!empty($_GET["notify_id"])){ $responseTxt = $this->GetResponse($_GET["notify_id"]); }
      //写日志记录
      if(!empty($logpath)){
        $log_text = "responseTxt=" . $responseTxt . "\n" . "notify_url_log:sign=" . $_GET["sign"] . "&mysign=" . $mysign.",";
        $log_text = $log_text . CmsAlipayCore::CreateLinkString($_GET);
        CmsAlipayCore::WriteLog($logpath, $log_text);
      }
      /*
       * 验证
       * $responseTxt的结果不是true，与服务器设置问题、合作身份者ID、notify_id一分钟失效有关
       * mysign与sign不等，与安全校验码、请求时的参数格式（如：带自定义参数等）、编码格式有关
       */
      if(preg_match("/true$/i", $responseTxt) && $mysign == $_GET["sign"]){
        return true;
      }else{
        return false;
      }
    }
  }

  /**
   * 根据反馈回来的信息，生成签名结果字符串
   * @access private
   * @param array $parameters 通知返回来的参数数组
   * @return string 生成的签名结果字符串
   */
  private function GetMySign($parameters){
    //除去待签名参数数组中的空值和签名参数
    $parameters_filter = CmsAlipayCore::FilterParameters($parameters);
    //对待签名参数数组排序
    $parameters_sort = CmsAlipayCore::SortArray($parameters_filter);
    //生成签名结果字符串
    $mysign = CmsAlipayCore::BuildMySign($parameters_sort, trim($this->aliapy_config['key']), strtoupper(trim($this->aliapy_config['sign_type'])));
    return $mysign;
  }

  /**
   * 获取远程服务器ATN结果,验证返回URL
   * @access private
   * @param string $notify_id 通知校验ID
   * @return mixed 服务器ATN结果:
   *  - invalid 命令参数不对. 出现这个错误，请检测返回处理中partner和key是否为空.
   *  - true 返回正确信息.
   *  - false 请检查防火墙或者是服务器阻止端口问题以及验证时间是否超过一分钟.
   */
  private function GetResponse($notify_id){
    $partner = trim($this->aliapy_config['partner']);
    $transport = strtolower(trim($this->aliapy_config['transport']));
    $veryfy_url = '';
    if($transport == 'https') {
      $veryfy_url = $this->https_verify_url;
    }else{
      $veryfy_url = $this->http_verify_url;
    }
    $veryfy_url = $veryfy_url . "partner=" . $partner . "&notify_id=" . $notify_id;
    $responseTxt = CmsAlipayCore::GetHttpResponse($veryfy_url);
    return $responseTxt;
  }
}