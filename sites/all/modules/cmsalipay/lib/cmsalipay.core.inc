<?php
/**
 * 支付宝接口公用函数类
 * 详细：该类是请求、通知返回两个文件所调用的公用函数核心处理文件
 * 日期：2012-09
 * @author Changfeng Ji <jichf@qq.com>
 */
final class CmsAlipayCore{
  /**
   * 除去数组中的空值和签名参数
   * @access public
   * @static
   * @param array $parameters 签名参数组
   * @return array 去掉空值与签名参数后的新签名参数组
   */
  public static function FilterParameters($parameters){
    $parameters_filter = array();
    while(list($key, $val) = each($parameters)) {
      if($key == "sign" || $key == "sign_type" || $val == "") continue;
      else $parameters_filter[$key] = $parameters[$key];
    }
    return $parameters_filter;
  }

  /**
   * 对数组排序
   * @access public
   * @static
   * @param array $array 排序前的数组
   * @return array 排序后的数组
   */
  public static function SortArray($array){
    ksort($array);
    reset($array);
    return $array;
  }

  /**
   *
   * @access public
   * @static
   * @param array $parameters 要签名的数组
   * @param string $key 支付宝交易安全校验码
   * @param string $sign_type 签名类型 默认值：MD5
   * @return string 签名结果字符串
   */
  public static function BuildMySign($parameters, $key, $sign_type = 'MD5'){
    //生成待签名字符串
    $prestr = self::CreateLinkString($parameters);
    //把拼接后的字符串再与安全校验码直接连接起来
    $prestr = $prestr . $key;
    //把最终的字符串签名，获得签名结果字符串
    $mysgin = self::Sign($prestr, $sign_type);
    return $mysgin;
  }

  /**
   * 把数组所有元素，按照“参数=参数值”的模式用“&”字符拼接成字符串.
   * @access public
   * @static
   * @param array $parameters 需要拼接的数组
   * @return string 拼接完成以后的字符串
   */
  public static function CreateLinkString($parameters){
    $args  = array();
    while(list($key, $val) = each($parameters)) {
      $args[] = $key . "=" . $val;
    }
    $arg = implode('&', $args);
    //如果存在转义字符，那么去掉转义
    if(get_magic_quotes_gpc()){ $arg = stripslashes($arg); }
    return $arg;
  }

  /**
   * 把数组所有元素，按照“参数=参数值”的模式用“&”字符拼接成字符串，并对字符串做 urlencode 编码
   * @access public
   * @static
   * @param array $parameters 需要拼接的数组
   * @return string 拼接完成以后的字符串
   */
  public static function CreateLinkStringUrlencode($parameters){
    $args  = array();
    while(list($key, $val) = each($parameters)) {
      $args[] = $key . "=" . urlencode($val);
    }
    $arg = implode('&', $args);
    //如果存在转义字符，那么去掉转义
    if(get_magic_quotes_gpc()){ $arg = stripslashes($arg); }
    return $arg;
  }

  /**
   * 签名字符串
   * @access public
   * @static
   * @param string $prestr 需要签名的字符串
   * @param string $sign_type 签名类型 默认值：MD5
   * @return string 签名结果字符串
   */
  public static function Sign($prestr, $sign_type = 'MD5'){
    $sign = '';
    if($sign_type == 'MD5'){
      $sign = md5($prestr);
    }elseif($sign_type == 'DSA'){
      //DSA 签名方法待后续开发
      die("DSA 签名方法待后续开发，请先使用MD5签名方式");
    }else{
      die("支付宝暂不支持" . $sign_type . "类型的签名方式");
    }
    return $sign;
  }

  /**
   * 写日志，方便测试（看网站需求，也可以改成把记录存入数据库）
   * 注意：服务器需要开通fopen配置
   * @access public
   * @static
   * @param string $logpath 写日志路径
   * @param string $word 要写入日志里的文本内容 默认值：空值
   */
  public static function WriteLog($logpath, $word = ''){
    $fp = fopen($logpath, "a");
    flock($fp, LOCK_EX) ;
    fwrite($fp, "执行日期：" . strftime("%Y%m%d%H%M%S", time()) . "\n" . $word . "\n");
    flock($fp, LOCK_UN);
    fclose($fp);
  }

  /**
   * 远程获取数据
   * 注意：该函数的功能可以用curl来实现和代替。curl需自行编写。
   * @access public
   * @static
   * @param string $url 指定URL完整路径地址
   * @param string $input_charset 编码格式。默认值：空值
   * @param int $time_out 超时时间。默认值：60
   * @return string 远程输出的数据
   */
  public static function GetHttpResponse($url, $input_charset = '', $time_out = "60"){
    $urlarr = parse_url($url);
    $errno = "";
    $errstr = "";
    $transports = "";
    $responseText = "";
    if($urlarr["scheme"] == "https"){
      $transports = "ssl://";
      $urlarr["port"] = "443";
    }else{
      $transports = "tcp://";
      $urlarr["port"] = "80";
    }
    $fp = @fsockopen($transports . $urlarr['host'], $urlarr['port'], $errno, $errstr, $time_out);
    if(!$fp){
      die("ERROR: $errno - $errstr<br />\n");
    }else{
      if(trim($input_charset) == ''){
        fputs($fp, "POST " . $urlarr["path"] . " HTTP/1.1\r\n");
      }else{
        fputs($fp, "POST " . $urlarr["path"] . '?_input_charset=' . $input_charset . " HTTP/1.1\r\n");
      }
      fputs($fp, "Host: " . $urlarr["host"] . "\r\n");
      fputs($fp, "Content-type: application/x-www-form-urlencoded\r\n");
      fputs($fp, "Content-length: " . strlen($urlarr["query"]) . "\r\n");
      fputs($fp, "Connection: close\r\n\r\n");
      fputs($fp, $urlarr["query"] . "\r\n\r\n");
      while(!feof($fp)){
        $responseText .= @fgets($fp, 1024);
      }
      fclose($fp);
      $responseText = trim(stristr($responseText, "\r\n\r\n"), "\r\n");
      return $responseText;
    }
  }

  /**
   * 实现多种字符编码方式
   * @access public
   * @static
   * @param string $input 需要编码的字符串
   * @param string $_output_charset 输出的编码格式
   * @param string $_input_charset 输入的编码格式
   * @return string 编码后的字符串
   */
  public static function CharsetEncode($input, $_output_charset, $_input_charset){
    $output = "";
    if(!isset($_output_charset)) $_output_charset = $_input_charset;
    if($_input_charset == $_output_charset || $input == null){
      $output = $input;
    }elseif(function_exists("mb_convert_encoding")){
      $output = mb_convert_encoding($input, $_output_charset, $_input_charset);
    }elseif(function_exists("iconv")){
      $output = iconv($_input_charset, $_output_charset, $input);
    }else die("sorry, you have no libs support for charset change.");
    return $output;
  }
}