<?php
require_once "cmsalipay.core.inc";
/**
 * 支付宝各接口请求提交类
 * 详细：构造支付宝各接口表单HTML文本，获取远程HTTP数据
 * 日期：2012-09
 * @author Changfeng Ji <jichf@qq.com>
 */
final class CmsAlipaySubmit{
  /**
   * 生成要请求给支付宝的参数数组
   * @access private
   * @static
   * @param array $parameters 请求前的参数数组
   * @param string $key 支付宝交易安全校验码
   * @param string $sign_type 签名类型 默认值：MD5
   * @return array 要请求的参数数组（经过过滤、排序、签名）
   */
  private static function BuildRequestParameterArray($parameters, $key, $sign_type = 'MD5'){
    //除去待签名参数数组中的空值和签名参数
    $parameters_filter = CmsAlipayCore::FilterParameters($parameters);
    //对待签名参数数组排序
    $parameters_sort = CmsAlipayCore::SortArray($parameters_filter);
    //生成签名结果字符串
    $mysign = CmsAlipayCore::BuildMySign($parameters_sort, trim($key), strtoupper(trim($sign_type)));
    //签名结果字符串与签名方式加入请求提交参数组中
    $parameters_sort['sign'] = $mysign;
    $parameters_sort['sign_type'] = strtoupper(trim($sign_type));
    return $parameters_sort;
  }

  /**
   * 生成要请求给支付宝的参数数组字符串
   * @access private
   * @static
   * @param array $parameters 请求前的参数数组
   * @param string $key 支付宝交易安全校验码
   * @param string $sign_type 签名类型 默认值：MD5
   * @return string 要请求的参数数组字符串（经过过滤、排序、签名）
   */
  private static function BuildRequestParameterString($parameters, $key, $sign_type = 'MD5'){
    //待请求参数数组
    $parameters = self::BuildRequestParameterArray($parameters, $key, $sign_type);
    //把参数组中所有元素，按照“参数=参数值”的模式用“&”字符拼接成字符串，并对参数值做urlencode编码
    $request_data = CmsAlipayCore::CreateLinkStringUrlencode($parameters);
    return $request_data;
  }

  /**
   * 构造提交表单HTML数据
   * @access public
   * @static
   * @param array $parameters 请求参数数组
   * @param string $gateway 网关地址
   * @param string $method 提交方式。两个值可选：post、get
   * @param string $button_name 确认按钮显示文字
   * @param string $key 支付宝交易安全校验码
   * @param string $sign_type 签名类型 默认值：MD5
   * @param string $input_charset 字符编码格式 目前支持 gbk 或 utf-8
   * @return string 提交表单HTML文本
   */
  public static function BuildForm($parameters, $gateway, $method, $button_name, $key, $sign_type = 'MD5', $input_charset = 'utf-8'){
    //要请求的参数数组
    $parameters = self::BuildRequestParameterArray($parameters, $key, $sign_type);
    $sHtml = "<form id='alipaysubmit' name='alipaysubmit' action='" . $gateway . "_input_charset=" . trim(strtolower($input_charset)) . "' method='" . $method . "'>";
    while(list($key, $val) = each($parameters)){
      $sHtml .= "<input type='hidden' name='" . $key . "' value='" . $val . "'/>";
    }
    //submit按钮控件请不要含有name属性
    $sHtml = $sHtml . "<input type='submit' value='" . $button_name . "'></form>";
    $sHtml = $sHtml . "<script>document.forms['alipaysubmit'].submit();</script>";
    return $sHtml;
  }

  /**
   * 构造模拟远程HTTP的POST请求，获取支付宝的返回XML处理结果
   * 注意：该功能PHP5环境及以上支持，因此必须服务器、本地电脑中装有支持DOMDocument、SSL的PHP配置环境。建议本地调试时使用PHP开发软件
   * @access public
   * @static
   * @param array $parameters 请求参数数组
   * @param string $gateway 网关地址
   * @param string $key 支付宝交易安全校验码
   * @param string $sign_type 签名类型 默认值：MD5
   * @param string $input_charset 字符编码格式 目前支持 gbk 或 utf-8
   * @return \DOMDocument 支付宝返回XML处理结果
   */
  public static function SendPostInfo($parameters, $gateway, $key, $sign_type = 'MD5', $input_charset = 'utf-8'){
    //要请求的参数数组字符串
    $request_data = self::BuildRequestParameterString($parameters, $key, $sign_type);
    //请求的url完整链接
    $url = $gateway . $request_data;
    //远程获取数据
    $xml_data = CmsAlipayCore::GetHttpResponse($url, trim(strtolower($input_charset)));
    //解析XML
    $doc = new DOMDocument();
    $doc->loadXML($xml_data);
    return $doc;
  }
}