<?php
/**
 * The asymmetric encryption algorithm by openssl.
 * @author Changfeng Ji <jichf@qq.com>
 */
final class MiniFieldOpenSSL{
  /**
   * The private key resource.
   * @var resource 
   */
  private $privateKey;
  /**
   * The public key resource.
   * @var resource 
   */
  private $publicKey;
  private $keyPath;
  /**
   * The asymmetric encryption algorithm by openssl.
   * 
   * Public key and private key is a pair of,
   * if use public key to encrypt data, it is only with the corresponding private key can decrypt;
   * If use the private key to encrypt data, so it is only with the corresponding public key can decrypt.
   * @param string $keyPath The key path.
   * @throws Exception Throw exception if the key path not exists or is not a directory.
   */
  public function __construct($keyPath) {
    if(empty($keyPath) || !is_dir($keyPath)){
      throw new Exception('Must set the keys save path');
    }
    $this->keyPath = $keyPath;
  }
  /**
   * Create the key pair(private key and public key), save them to $this->keyPath.
   * @link http://php.net/manual/en/function.openssl-pkey-new.php
   * @param array $configargs [optional] <p>
   *  You can finetune the key generation (such as specifying the number of
   *  bits) using <i>configargs</i>. See
   *  <b>openssl_csr_new</b> for more information about
   *  <i>configargs</i>.
   *  Such as: array("digest_alg" => "sha512", "private_key_bits" => 4096, "private_key_type" => OPENSSL_KEYTYPE_RSA).
   *  </p>
   * @return boolean <b>TRUE</b> on success or <b>FALSE</b> on failure.
   */
  public function createKey($configargs = array()){
    if(!empty($configargs)){
      $res = openssl_pkey_new($configargs);
    }else{
      $res = openssl_pkey_new();
    }
    if(!$res){
      return false;
    }
    $rt = openssl_pkey_export($res, $privKey);
    if(!$rt){
      return false;
    }
    $rp = openssl_pkey_get_details($res);
    $pubKey = $rp['key'];
    
    file_put_contents($this->keyPath . DIRECTORY_SEPARATOR . 'private.key', $privKey);
    file_put_contents($this->keyPath . DIRECTORY_SEPARATOR . 'public.key', $pubKey);
    $this->privateKey = openssl_pkey_get_private($privKey);
    $this->publicKey = openssl_pkey_get_public($pubKey);
  }
  /**
   * Setup the private key.
   * @return boolean TRUE if successful, FALSE otherwise.
   */
  public function setupPrivateKey(){
    if(is_resource($this->privateKey)){
      return true;
    }
    $file = $this->keyPath . DIRECTORY_SEPARATOR . 'private.key';
    if(!file_exists($file)){
      return false;
    }
    $privKey = file_get_contents($file);
    if(empty($privKey)){
      return false;
    }
    $this->privateKey = openssl_pkey_get_private($privKey);
    return true;
  }
  /**
   * Setup the public key.
   * @return boolean TRUE if successful, FALSE otherwise.
   */
  public function setupPublicKey(){
    if(is_resource($this->publicKey)){
      return true;
    }
    $file = $this->keyPath . DIRECTORY_SEPARATOR . 'public.key';
    if(!file_exists($file)){
      return false;
    }
    $pubKey = file_get_contents($file);
    if(empty($pubKey)){
      return false;
    }
    $this->publicKey = openssl_pkey_get_public($pubKey);
    return true;
  }
  /**
   * Encrypt data with the private key.
   * @param string $data The unencrypted string(raw string).
   * @return mixed The encrypted string or <b>FALSE</b> on failure.
   */
  public function privateEncrypt($data){
    if(!is_string($data)){
      return false;
    }
    if(!$this->setupPrivateKey()){
      return false;
    }
    $encrypted = '';
    $r = openssl_private_encrypt($data, $encrypted, $this->privateKey);
    if($r){
      return base64_encode($encrypted);
    }
    return false;
  }
  /**
   * Decrypt data with the private key.
   * @param string $encrypted The encrypted string.
   * @return mixed The decrypted string(raw string) or <b>FALSE</b> on failure.
   */
  public function privateDecrypt($encrypted){
    if(!is_string($encrypted)){
      return false;
    }
    if(!$this->setupPrivateKey()){
      return false;
    }
    $encrypted = base64_decode($encrypted);
    $decrypted = '';
    $r = openssl_private_decrypt($encrypted, $decrypted, $this->privateKey);
    if($r){
      return $decrypted;
    }
    return false;
  }
  /**
   * Encrypt data with the public key.
   * @param string $data The unencrypted string(raw string).
   * @return mixed The encrypted string or <b>FALSE</b> on failure.
   */
  public function publicEncrypt($data){
    if(!is_string($data)){
      return false;
    }
    if(!$this->setupPublicKey()){
      return false;
    }
    $encrypted = '';
    $r = openssl_public_encrypt($data, $encrypted, $this->publicKey);
    if($r){
      return base64_encode($encrypted);
    }
    return false;
  }
  /**
   * Decrypt data with the public key.
   * @param string $encrypted The encrypted string.
   * @return mixed The decrypted string(raw string) or <b>FALSE</b> on failure.
   */
  public function publicDecrypt($encrypted){
    if(!is_string($encrypted)){
      return false;
    }
    if(!$this->setupPublicKey()){
      return false;
    }
    $encrypted = base64_decode($encrypted);
    $decrypted = '';
    $r = openssl_public_decrypt($encrypted, $decrypted, $this->publicKey);
    if($r){
      return $decrypted;
    }
    return false;
  }
}