<?php
/**
 * @author Changfeng Ji <jichf@qq.com>
 */
final class MiniFieldMail{
  /**
   *
   * @access public
   * @static
   * @param type $exceptions Should we throw external exceptions?
   * @return \PHPMailer
   */
  public static function GetMailInstance($exceptions = false){
    require_once 'phpmailer/class.phpmailer.php';
    $mail = new PHPMailer(($exceptions == true));//the true param means it will throw exceptions on errors, which we need to catch
    return $mail;
  }

  /**
   *
   * @access public
   * @static
   * @param \PHPMailer $mail PHPMailer instance
   * @return boolean
   */
  public static function SendBasic(PHPMailer $mail){
    if(!$mail->Send()){
      return $mail->ErrorInfo;
    }else{
      return true;
    }
  }

  /**
   *
   * @access public
   * @static
   * @param \PHPMailer $mail PHPMailer instance
   * @return mixed true if successfully.
   */
  public static function SendAdvanced(PHPMailer $mail){
    try {
      $mail->Send();
      return true;
    }catch(phpmailerException $e){
      return $e->errorMessage(); //Pretty error messages from PHPMailer
    }catch(Exception $e){
      return $e->getMessage(); //Boring error messages from anything else!
    }
  }

  /**
   *
   * @access public
   * @static
   * @param \PHPMailer $mail PHPMailer instance
   * @param array $params All keys:
   * - * Mailer(string) Method to send mail: ("smtp", "mail", "sendmail", or "qmail").
   * - * SMTPSecure(string) Sets connection prefix. Options are "", "ssl" or "tls".
   * - * SMTPAuth(bool) Sets SMTP authentication. Utilizes the Username and Password variables.
   * - * SMTPDebug(bool) Sets SMTP class debugging on or off.
   * - * SMTPKeepAlive(bool) Prevents the SMTP connection from being closed after each mail sending.
   * - * Port(int) Sets the default SMTP server port.
   * - * Host(string) Sets the SMTP hosts. All hosts must be separated by a semicolon.
   *                 You can also specify a different port for each host by using this format: [hostname:port] (e.g. "smtp1.example.com:25;smtp2.example.com").
   *                 Hosts will be tried in order.
   * - * Username(string) Sets SMTP username.
   * - * Password(string) Sets SMTP password.
   * - * Timeout(int) Sets the SMTP server timeout in seconds. This function will not work with the win32 version.
   * - * {SetFrom AddReplyTo AddAddress AddCC AddBCC}(array) Each key is a mail address, each value is a mail name.
   * - * Subject(string) Sets the Subject of the message.
   * - * AltBody(string) Sets the text-only body of the message.
   *                   This automatically sets the email to multipart/alternative.
   *                   This body can be read by mail clients that do not have HTML email capability such as mutt.
   *                   Clients that can read HTML will view the normal Body.
   * - * Body(string) Sets the Body of the message.  This can be either an HTML or text body.
   *                If HTML then set ContentType(true).
   * - * WordWrap(int) Sets word wrapping on the body of the message to a given number of characters.
   * - * Attachments(array) Each element is still a array with one to four elements($path[, $name, $encoding, $type]):
   *                      $path(string) Path to the attachment.
   *                      $name(string) Overrides the attachment name.
   *                      $encoding(string) File encoding. Options for this are "8bit", "7bit", "binary", "base64", and "quoted-printable".
   *                      $type(string) File extension (MIME) type.
   * - * ContentType(bool) Sets the Content-type of the message. true if content type is html, false otherwise.
   * - * CharSet(string) Sets the CharSet of the message.
   */
  public static function ProcessParams(PHPMailer $mail, $params = array()){
    if(!($mail instanceof PHPMailer) || !is_array($params)){ return; }
    foreach($params as $key => $value){
      switch($key){
        case 'Mailer':
          if($value == 'smtp'){
            $mail->IsSMTP();
          }else if($value == 'sendmail'){
            $mail->IsSendmail();
          }else if($value == 'qmail'){
            $mail->IsQmail();
          }else if($value == 'mail'){
            $mail->IsMail();
          }
          break;
        case 'SMTPSecure':
          if(in_array($value, array('', 'ssl', 'tls'))){ $mail->$key = $value; }
          break;
        case 'SMTPAuth':
        case 'SMTPDebug':
        case 'SMTPKeepAlive':
          if(is_bool($value)){ $mail->$key = $value; }
          break;
        case 'Port':
        case 'Timeout':
        case 'WordWrap':
          if(preg_match('/^[0-9]*[1-9][0-9]*$/', $value) > 0){ $mail->$key = $value; }
          break;
        case 'Host':
        case 'Username':
        case 'Password':
        case 'Subject':
        case 'AltBody':
        case 'CharSet':
          if(strlen($value) > 0){ $mail->$key = $value; }
          break;
        case 'SetFrom':
        case 'AddReplyTo':
        case 'AddAddress':
        case 'AddCC':
        case 'AddBCC':
          if(is_array($value)){
            foreach($value as $address => $name){
              $mail->$key($address, $name);
            }
          }
          break;
        case 'Body':
          if(isset($params['ContentType']) && $params['ContentType'] == true){
            $value = preg_replace('/\\\\/', '', $value);//Strip backslashes
            $mail->MsgHTML($value);
          }else{
            $mail->Body = $value;
          }
          break;
        case 'Attachments':
          if(is_array($value)){
            foreach($value as $attach){
              if(is_array($attach) && isset($attach[0]) && is_file($attach[0])){
                if(!isset($attach[1])){
                  $mail->AddAttachment($attach[0]);
                }else if(!isset($attach[2])){
                  $mail->AddAttachment($attach[0], $attach[1]);
                }else if(!isset($attach[3])){
                  $mail->AddAttachment($attach[0], $attach[1], $attach[2]);
                }else{
                  $mail->AddAttachment($attach[0], $attach[1], $attach[2], $attach[3]);
                }
              }
            }
          }
          break;
        case 'ContentType':
          if(is_bool($value)){ $mail->IsHTML($value); }
          break;
      }
    }
  }
}