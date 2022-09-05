<?php
/**
 * The symmetric encryption algorithm.
 * @author Changfeng Ji <jichf@qq.com>
 */
final class MiniFieldEncrypt{
  /**
   * The simple encryption of symmetric encryption algorithm.
   * @access public
   * @static
   * @param string $string The unencrypted string(raw string).
   * @param string $key The encryption key.
   * @return mixed The encrypted string or <b>FALSE</b> on failure.
   */
  public static function EncryptSimple($string, $key = ''){
    if(!is_string($string)){
      return false;
    }
    $strArr = str_split(base64_encode($string));
    $key = str_split(base64_encode(md5($key)));
    $keyCount = count($key);
    $keyIndex = 0;
    $strArrKeys = array_keys($strArr);
    foreach($strArrKeys as $k){
      if($keyIndex >= $keyCount){
        $keyIndex = 0;
      }
      $strArr[$k] .= $key[$keyIndex];
      $keyIndex++;
    }
    return join('', $strArr);
  }
  /**
   * The simple decryption of symmetric encryption algorithm.
   * @access public
   * @static
   * @param string $string The encrypted string.
   * @param string $key The decryption key.
   * @return mixed The decrypted string(raw string) or <b>FALSE</b> on failure.
   */
  public static function DecryptSimple($string, $key = ''){
    if(!is_string($string)){
      return false;
    }
    $strArr = str_split($string, 2);
    $key = str_split(base64_encode(md5($key)));
    $keyCount = count($key);
    $keyIndex = 0;
    $strArrKeys = array_keys($strArr);
    foreach($strArrKeys as $k){
      if($keyIndex >= $keyCount){
        $keyIndex = 0;
      }
      if(isset($strArr[$k][1]) && $strArr[$k][1] === $key[$keyIndex]){
        $strArr[$k] = $strArr[$k][0];
      }
      $keyIndex++;
    }
    return base64_decode(join('', $strArr));
  }

  /**
   * The encryption of symmetric encryption algorithm.
   * @access public
   * @static
   * @param string $string The unencrypted string(raw string).
   * @param string $key The encryption key.
   * @return mixed The encrypted string or <b>FALSE</b> on failure.
   */
  public static function Encrypt($string, $key = ''){
    if(!is_string($string)){
      return false;
    }
    $key = md5($key);
    $key_length = strlen($key);
    $string = substr(md5($string . $key), 0, 8) . $string;
    $string_length = strlen($string);
    $randomKeys = $box = array();
    $result = '';
    for($i = 0;$i < 256;$i++){
      $randomKeys[$i] = ord($key[$i % $key_length]);
      $box[$i] = $i;
    }
    $j = 0;
    for($i = 0;$i < 256;$i++){
      $j = ($j + $box[$i] + $randomKeys[$i]) % 256;
      $tmp = $box[$i];
      $box[$i] = $box[$j];
      $box[$j] = $tmp;
    }
    $a = $j = 0;
    for($i = 0;$i < $string_length;$i++){
      $a = ($a + 1) % 256;
      $j = ($j + $box[$a]) % 256;
      $tmp = $box[$a];
      $box[$a] = $box[$j];
      $box[$j] = $tmp;
      $result .= chr(ord($string[$i]) ^ ($box[($box[$a] + $box[$j]) % 256]));
    }
    return base64_encode($result);
  }
  /**
   * The decryption of symmetric encryption algorithm.
   * @access public
   * @static
   * @param string $string The encrypted string.
   * @param string $key The decryption key.
   * @return mixed The decrypted string(raw string) or <b>FALSE</b> on failure.
   */
  public static function Decrypt($string, $key = ''){
    if(!is_string($string)){
      return false;
    }
    $key = md5($key);
    $key_length = strlen($key);
    $string = base64_decode($string);
    $string_length = strlen($string);
    $randomKeys = $box = array();
    $result = '';
    for($i = 0;$i < 256;$i++){
      $randomKeys[$i] = ord($key[$i % $key_length]);
      $box[$i] = $i;
    }
    $j = 0;
    for($i = 0;$i < 256;$i++){
      $j = ($j + $box[$i] + $randomKeys[$i]) % 256;
      $tmp = $box[$i];
      $box[$i] = $box[$j];
      $box[$j] = $tmp;
    }
    $a = $j = 0;
    for($i = 0;$i < $string_length;$i++){
      $a = ($a + 1) % 256;
      $j = ($j + $box[$a]) % 256;
      $tmp = $box[$a];
      $box[$a] = $box[$j];
      $box[$j] = $tmp;
      $result .= chr(ord($string[$i]) ^ ($box[($box[$a] + $box[$j]) % 256]));
    }
    if(substr($result, 0, 8) == substr(md5(substr($result, 8) . $key), 0, 8)){
      return substr($result, 8);
    }else{
      return false;
    }
  }
}