<?php
/**
 * @author Changfeng Ji <jichf@qq.com>
 */
final class MiniFieldFile{
  /**
   * 
   * @access public
   * @static
   * @param int $fid file ID.
   * @param boolean $reset to reset the internal cache and load from the database; FALSE (default) to load from the internal cache, if set.
   * @return string An object representing the file, or FALSE if the file was not found.
   */
  public static function loadFile($fid, $reset = false){
    if(!is_numeric($fid)){
      return false;
    }
    $StaticName = __CLASS__ . '_' . __FUNCTION__ . '_' . $fid;
    $file = MiniFieldStatic::GetStatic($StaticName);
    if($reset){
      $file = MiniFieldStatic::ClearStatic($StaticName);
    }
    if(!isset($file)){
      $file = file_load($fid);
      if($file && $file->fid){
        /* $file->realpath = drupal_realpath($file->uri); */
        $file->filepath = self::getFilepathByUri($file->uri);
        $file->isimage = count(self::FileValidateIsImage($file)) ? FALSE : TRUE;
        MiniFieldStatic::SetStatic($StaticName, $file);
      }
    }
    return $file;
  }

  /**
   * Upload file and save file to database.
   * @static
   * @access public
   * @param string $source A string specifying the filepath or URI of the uploaded file to save.
   * @param int $sourceindex The key of array $sourceindex $_FILES['files']['name'][$source]
   * @param string $destination :
   *  - A string containing the URI $source should be copied to.
   *  - This must be a stream wrapper URI. If this value is omitted, Drupal's temporary files scheme will be used ("temporary://").
   * @param array $validators Each element is still a array. such as:
   *  - array('MiniFieldFile', 'FileValidateExtensions', $extensions). $extensions: string with a space separated list of allowed extensions
   *  - array('MiniFieldFile', 'FileValidateSize', $file_limit). $file_limit: integer specifying the maximum file size in bytes. Zero indicates that no limit should be enforced.
   *  - array('MiniFieldFile', 'FileValidateIsImage')
   *  - and so on.
   * @param int $status file status, 1 or 0.
   * 
   * @return object/string/null/false :
   *  - An object containing the file information if the upload succeeded,
   *  - An array containing the error information if the upload failed,
   *  - FALSE in the event of an error.
   */
  public static function FileSaveUpload($source, $sourceindex = 0, $destination = FALSE, $validators = array(), $status = 0){
    global $user;
    $errors = array();
    if(!is_array($_FILES['files']['name'][$source])){ return FALSE; }
    if(preg_match('/^\d+$/', $sourceindex) == 0){ return FALSE; }
    if(!isset($_FILES['files']['name'][$source][$sourceindex])){ return FALSE; }
    // Make sure there's an upload to process.
    if(empty($_FILES['files']['name'][$source][$sourceindex])) { return FALSE; }
    
    $file_name = $_FILES['files']['name'][$source][$sourceindex];
    $file_type = $_FILES['files']['type'][$source][$sourceindex];
    $file_tmp_name = $_FILES['files']['tmp_name'][$source][$sourceindex];
    $file_error = $_FILES['files']['error'][$source][$sourceindex];
    $file_size = $_FILES['files']['size'][$source][$sourceindex];
    
    // Check for file upload errors and return errors if a lower level system error occurred. For a complete list of errors:
    // See http://php.net/manual/en/features.file-upload.errors.php.
    switch ($file_error) {
      case UPLOAD_ERR_INI_SIZE:
      case UPLOAD_ERR_FORM_SIZE:
        $errors[0] = ts('这个文件 %file 不能保存，因为它超过了 %maxsize，即允许上传的最大值', 'ucfirst', 'minifield', array('%file' => $file_name, '%maxsize' => format_size(file_upload_max_size())));
        return $errors;
      case UPLOAD_ERR_PARTIAL:
      case UPLOAD_ERR_NO_FILE:
        $errors[0] = ts('这个文件 %file 不能保存，因为上传没有完成', 'ucfirst', 'minifield', array('%file' => $file_name));
        return $errors;
      case UPLOAD_ERR_OK:
        // Final check that this is a valid upload, if it isn't, use the default error handler.
        if (is_uploaded_file($file_tmp_name)) {
          break;
        }
      default:// Unknown error
        $errors[0] = ts('这个文件 %file 不能保存，一个未知的错误发生', 'ucfirst', 'minifield', array('%file' => $file_name));
        return $errors;
    }
    
    // If the destination is not provided, use the temporary directory.
    if (empty($destination)) {
      $destination = 'temporary://';
    }
    // Assert that the destination contains a valid stream.
    $destination_scheme = file_uri_scheme($destination);
    if (!$destination_scheme || !file_stream_wrapper_valid_scheme($destination_scheme)) {
      $errors[0] = ts('这个文件 %file 不能保存，因为目标 %destination 无效', 'ucfirst', 'minifield', array('%file' => $file_name, '%destination' => $destination));
      return $errors;
    }    
    // A URI may already have a trailing slash or look like "public://".
    if (substr($destination, -1) != '/') {
      $destination .= '/';
    }
    
    // Begin building file object.
    $file = new stdClass();
    $file->uid      = $user->uid;
    $file->status   = $status;
    $file->filename = trim($file_name, '.');
    $file->uri      = $file_tmp_name;
    $file->filemime = file_get_mimetype($file->filename);
    $file->filesize = $file_size;
    $file->source   = $source;
    $file->uri = $destination . date("YmdHis") . rand(10000, 99999) . '.' . self::getFileExtension($file_name);
    while(file_exists($file->uri)){
      $file->uri = $destination . date("YmdHis") . rand(10000, 99999) . '.' . self::getFileExtension($file_name);
    }

    // Move uploaded files from PHP's upload_tmp_dir to Drupal's temporary directory.
    // This overcomes open_basedir restrictions for future fileoperations.
    if (!drupal_move_uploaded_file($file_tmp_name, $file->uri)) {
      $errors[0] = ts('文件 %file 上传失败. 不能移动被上传的文件', 'ucfirst', 'minifield', array('%file' => $file_name));
      return $errors;
    }
    
    //validators
    foreach($validators as $args) {
      if(count($args) >= 2){
        $class = array_shift($args);
        $function = array_shift($args);
        if(method_exists($class, $function)){
          array_unshift($args, $file);
          $errors = array_merge(call_user_func_array(array($class, $function), $args), $errors);
          if(!empty($errors)){
            file_unmanaged_delete($file->uri);
            return $errors;
          }
        }
      }
    }
    // Set the permissions on the new file.
    drupal_chmod($file->uri);
    $file = file_save($file);
    if ($file && $file->fid) {
      return $file;
    }
    return FALSE;
  }

  /**
   * Save file to database.
   * @static
   * @access public
   * @param string $filepath A string specifying the filepath or URI of the saved file to save(gb2312 encoded).
   * @param string $destination :
   *  - A string containing the URI $source should be copied to.
   *  - This must be a stream wrapper URI. If this value is omitted, Drupal's temporary files scheme will be used ("temporary://").
   * @param array $validators Each element is still a array. such as:
   *  - array('MiniFieldFile', 'FileValidateExtensions', $extensions). $extensions: string with a space separated list of allowed extensions
   *  - array('MiniFieldFile', 'FileValidateSize', $file_limit). $file_limit: integer specifying the maximum file size in bytes. Zero indicates that no limit should be enforced.
   *  - array('MiniFieldFile', 'FileValidateIsImage')
   *  - and so on.
   * @param int $status file status, 1 or 0.
   *
   * @return object/string/null/false :
   *  - An object containing the file information if the upload succeeded,
   *  - An array containing the error information if the upload failed,
   *  - FALSE in the event of an error.
   */
  public static function FileSave($filepath, $destination = FALSE, $validators = array(), $status = 0){
    $filepath_gb2312 = $filepath;
    $filepath = iconv('gb2312', 'utf-8', $filepath);
    global $user;
    $errors = array();
    if(!is_file($filepath_gb2312)){
      $errors[0] = format_string('这个文件 %file 不存在', array('%file' => $filepath));
      return $errors;
    }
    $file_name = self::getFileName($filepath);
    $file_size = filesize($filepath);
    // If the destination is not provided, use the temporary directory.
    if (empty($destination)) {
      $destination = 'temporary://';
    }
    // Assert that the destination contains a valid stream.
    $destination_scheme = file_uri_scheme($destination);
    if (!$destination_scheme || !file_stream_wrapper_valid_scheme($destination_scheme)) {
      $errors[0] = ts('这个文件 %file 不能保存，因为目标 %destination 无效', 'ucfirst', 'minifield', array('%file' => $file_name, '%destination' => $destination));
      return $errors;
    }
    // A URI may already have a trailing slash or look like "public://".
    if (substr($destination, -1) != '/') {
      $destination .= '/';
    }
    
    $file = new stdClass();
    $file->uid      = $user->uid;
    $file->status   = $status;
    $file->filename = trim($file_name, '.');
    $file->filemime = file_get_mimetype($file->filename);
    $file->filesize = $file_size;
    $file->uri = "public://" . date("YmdHis") . rand(10000, 99999) . '.' . self::getFileExtension($file_name);
    while(file_exists($file->uri)){
      $file->uri = "public://" . date("YmdHis") . rand(10000, 99999) . '.' . self::getFileExtension($file_name);
    }
    $newuri = file_unmanaged_copy($filepath_gb2312, $file->uri, FILE_EXISTS_RENAME);
    if($newuri){
      $file->uri = $newuri;
    }else{
      $errors[0] = format_string('这个文件 %file 保存失败', array('%file' => $filepath));
      return $errors;
    }

    //validators
    foreach($validators as $args) {
      if(count($args) >= 2){
        $class = array_shift($args);
        $function = array_shift($args);
        if(method_exists($class, $function)){
          array_unshift($args, $file);
          $errors = array_merge(call_user_func_array(array($class, $function), $args), $errors);
          if(!empty($errors)){
            file_unmanaged_delete($file->uri);
            return $errors;
          }
        }
      }
    }
    // Set the permissions on the new file.
    drupal_chmod($file->uri);
    $file = file_save($file);
    if ($file && $file->fid) {
      return $file;
    }
    return FALSE;
  }
  
  /**
   * Check for files with names longer than we can store in the database.
   * @access public
   * @static
   * @param $file
   *   A Drupal file object.
   * @return
   *   An array. If the file name is too long, it will contain an error message.
   */
  public static function FileValidateNameLength(stdClass $file, $length = 240) {
    $errors = array();
    if (empty($file->filename)) {
      $errors[] = ts("文件名为空，请为文件命名", 'ucfirst', 'minifield');
    }else if (strlen($file->filename) > $length) {
      $errors[] = ts("文件名长度超过 %length 字符限制，请重命名文件后再试", 'ucfirst', 'minifield', array('%length' => $file_name));
    }
    return $errors;
  }
  
  /**
   * Check that the filename ends with an allowed extension.
   * @access public
   * @static
   * @param $file
   *   A Drupal file object.
   * @param $extensions
   *   A string with a space separated list of allowed extensions.
   *
   * @return
   *   An array. If the file extension is not allowed, it will contain an error message.
   */
  public static function FileValidateExtensions(stdClass $file, $extensions) {
    $errors = array();
    $regex = '/\.(' . preg_replace('/ +/', '|', preg_quote($extensions)) . ')$/i';
    if (!preg_match($regex, $file->filename)) {
      $errors[] = ts('仅拥有如下扩展名的文件允许上传：%files-allowed', 'ucfirst', 'minifield', array('%files-allowed' => $extensions));
    }
    return $errors;
  }
  
  /**
   * Check that the file's size is below certain limits.
   * @access public
   * @static
   * @param $file
   *   A Drupal file object.
   * @param $file_limit
   *   An integer specifying the maximum file size in bytes. Zero indicates that
   *   no limit should be enforced.
   * 
   * @return
   *   An array. If the file size exceeds limits, it will contain an error message.
   */
  public static function FileValidateSize(stdClass $file, $file_limit = '2M') {
    $file_limit = self::getBytes($file_limit);
    $errors = array();
    if ($file_limit && $file->filesize > $file_limit) {
      $errors[] = ts('这个文件是 %filesize 超过了文件最大大小 %maxsize', 'ucfirst', 'minifield', array('%filesize' => format_size($file->filesize), '%maxsize' => format_size($file_limit)));
    }
    return $errors;
  }
  
  /**
   * Check that the file is recognized by image_get_info() as an image.
   * @access public
   * @static
   * @param $file
   *   A Drupal file object.
   *
   * @return
   *   An array. If the file is not an image, it will contain an error message.
   */
  public static function FileValidateIsImage(stdClass $file) {
    $errors = array();
    if(is_file($file->uri)){
      $file->filesize = filesize($file->uri);
    }else{
      $errors[] = 'file not found';
    }
    $errors = array_merge($errors, self::FileValidateSize($file, '100M'));
    if(!empty($errors)) {
      return $errors;
    }
    $info = image_get_info($file->uri);
    if (!$info || empty($info['extension'])) {
      $errors[] = ts('仅支持 JPEG, PNG and GIF 格式的图片', 'ucfirst', 'minifield');
    }else{
      $file->imageinfo = $info;
    }
    return $errors;
  }
  
  /**
   * Verify that image dimensions are within the specified maximum and minimum.
   *
   * Non-image files will be ignored. If a image toolkit is available the image
   * will be scaled to fit within the desired maximum dimensions.
   *
   * @access public
   * @static
   * @param $file
   *   A Drupal file object. This function may resize the file affecting its
   *   size.
   * @param $maximum_dimensions
   *   An optional string in the form WIDTHxHEIGHT e.g. '640x480' or '85x85'. If
   *   an image toolkit is installed the image will be resized down to these
   *   dimensions. A value of 0 indicates no restriction on size, so resizing
   *   will be attempted.
   * @param $minimum_dimensions
   *   An optional string in the form WIDTHxHEIGHT. This will check that the
   *   image meets a minimum size. A value of 0 indicates no restriction.
   * @param $imagecut
   *   An optional string. The string is one of 'image_scale', 'image_crop', 'image_scale_and_crop'.
   * @param $imagecut_coordinate
   *   An optional string in the form X-COORDINATExY-COORDINATE. The value is valid when $imagecut equals to 'image_crop'.
   *
   * @return
   *   An array. If the file is an image and did not meet the requirements, it
   *   will contain an error message.
   */
  public static function FileValidateImageResolution(stdClass $file, $maximum_dimensions = 0, $minimum_dimensions = 0, $imagecut = '', $imagecut_coordinate = 0) {
    $errors = array();
    $file->filesize = filesize($file->uri);
    $errors = array_merge($errors, self::FileValidateSize($file, '100M'));
    if(!empty($errors)) {
      return $errors;
    }
    // Check first that the file is an image.
    $info = image_get_info($file->uri);
    if($info) {
      if($maximum_dimensions){
        // Check that it is smaller than the given dimensions.
        list($width, $height) = explode('x', $maximum_dimensions);
        if($info['width'] > $width || $info['height'] > $height) {
          if(!in_array($imagecut, array('image_scale', 'image_scale_and_crop', 'image_crop'))){
            $errors[] = ts('这个图片太大；最大尺度是 %dimensions 像素', 'ucfirst', 'minifield', array('%dimensions' => $maximum_dimensions));
            return $errors;
          }
          // Try to resize the image to fit the dimensions.
          $image = image_load($file->uri);
          if($image){
            //The image was resized to fit within the maximum allowed dimensions of $maximum_dimensions pixels.
            switch ($imagecut) {
              case 'image_scale':
                image_scale($image, $width, $height);
                break;
              case 'image_scale_and_crop':
                image_scale_and_crop($image, $width, $height);
                break;
              case 'image_crop':
                list($xcoor, $ycoor) = explode('x', $imagecut_coordinate);
                if(!is_numeric($xcoor) || $xcoor <= 0){ $xcoor = 0; }
                if(!is_numeric($ycoor) || $ycoor <= 0){ $ycoor = 0; }
                image_crop($image, $xcoor, $ycoor, $width, $height);
                break;
            }
            image_save($image);
            $file->filesize = $image->info['file_size'];
          }else {
            $errors[] = ts('这个图片太大；最大尺度是 %dimensions 像素', 'ucfirst', 'minifield', array('%dimensions' => $maximum_dimensions));
          }
        }
      }
      if($minimum_dimensions){
        // Check that it is larger than the given dimensions.
        list($width, $height) = explode('x', $minimum_dimensions);
        if($info['width'] < $width || $info['height'] < $height){
          $errors[] = ts('这个图片太小；最小尺度是 %dimensions 像素', 'ucfirst', 'minifield', array('%dimensions' => $minimum_dimensions));
        }
      }
    }
    return $errors;
  }

  /**
   * Scan files.
   * @access public
   * @static
   * @param string $filepath The file path.
   * @param string $dir_separator The directory separator.
   * @param boolean $recursive Whether scan subdirectories or not, defaults to TRUE.
   * @param boolean $istree Whether to use tree model, defaults to TRUE.
   * @param string $in_charset The input charset.
   * @param string $out_charset The output charset.
   * @return array The collection of files.
   */
  public static function ScanFiles($filepath, $dir_separator = '/', $recursive = true, $istree = true, $in_charset = '', $out_charset = ''){
    $filepath = rtrim($filepath, '\\/');
    $trees = array();
    if($dh = @opendir($filepath)){
      while(($file = readdir($dh)) !== false){
        if($file == "." || $file == ".."){
          continue;
        }
        $path = $filepath . $dir_separator . $file;
        $outpath = $in_charset != $out_charset ? iconv($in_charset, $out_charset, $path) : $path;
        $isdir = is_dir($path);
        if($isdir && $recursive && $istree){
          $trees[$outpath] = self::ScanFiles($path, $dir_separator, $recursive, $istree, $in_charset, $out_charset);
        }else if($isdir && $recursive){
          $trees = array_merge($trees, self::ScanFiles($path, $dir_separator, $recursive, $istree, $in_charset, $out_charset));
        }else if($isdir){
          //nothing to do
        }else{
          $trees[] = $outpath;
        }
      }
    }
    return $trees;
  }
  /**
   * Vigorously erase files and directories.
   * @access public
   * @static
   * @param type $fileglob mixed If string, must be a file name (foo.txt), glob pattern (*.txt), or directory name.
   *                              If array, must be an array of file names, glob patterns, or directories.
   * @return boolean bool true on success or false on failure.
   */
  public static function Remove($fileglob)
  {
    if(is_string($fileglob) && preg_match('/^\s*$/', $fileglob) == 0){
      if(is_file($fileglob)){
        return unlink($fileglob);
      }else if(is_dir($fileglob)) {
        $ok = self::Remove("$fileglob/*");
        if(!$ok){
          return false;
        }
        return rmdir($fileglob);
      }else{
        $matchings = glob($fileglob);
        if($matchings === false){
          trigger_error(sprintf('No files match supplied glob %s', $fileglob), E_USER_WARNING);
          return false;
        }
        /* $rcs = array_map('Remove', $matching);
        if (in_array(false, $rcs)) {
          return false;
        } */
        $results = array();
        foreach($matchings as $matching){
          $results[] = self::Remove($matching);
        }
        if(in_array(false, $results)){
          return false;
        }
      }
    }else if(is_array($fileglob)){
      /* $rcs = array_map('Remove', $fileglob);
      if (in_array(false, $rcs)) {
        return false;
      } */
      $results = array();
      foreach($fileglob as $matching){
        $results[] = self::Remove($matching);
      }
      if(in_array(false, $results)){
        return false;
      }
    }else{
      trigger_error('Param #1 must be filename or glob pattern, or array of filenames or glob patterns', E_USER_ERROR);
      return false;
    }
    return true;
  }
  
  /**
   * 
   * @access public
   * @static
   * @param string $filepath
   * @param boolean $remove_extension default to false.
   * @return string
   */
  public static function getFileName($filepath, $remove_extension = false){
    $filepath = trim($filepath, ' .');
    $filepaths = preg_split('/\/|\\\\/', $filepath);
    $filename = end($filepaths);
    if($remove_extension){
      $dotPos = strrpos($filename, '.');
      if($dotPos !== false){
        $filename = substr($filename, 0, strrpos($filename, '.'));
      }
    }
    return $filename;
  }
  
  /**
   * 
   * @access public
   * @static
   * @param string $filepath
   * @return string
   */
  public static function getFileExtension($filepath){
    $filepath = trim($filepath, ' .');
    $dotPos = strrpos($filepath, '.');
    if($dotPos === false){
      return '';
    }
    return substr($filepath, strrpos($filepath, '.') + 1);
  }
  
  /**
   * 
   * @access public
   * @static
   * @param string $uri URI to a file for which we need an external URL, or the path to a shipped file.
   * @return string
   */
  public static function getFilepathByUri($uri){
    /*$DrupalPublicStreamWrapper = new DrupalPublicStreamWrapper();
    $DrupalPublicStreamWrapper->setUri($uri);
    $filepath = $DrupalPublicStreamWrapper->getExternalUrl();*/
    $filepath = file_create_url($uri);
    return $filepath;
  }
  /**
   * 
   * @access public
   * @static
   * @param object/int $file The file object or file id.
   * @return string
   */
  public static function getFilepathByFile($file){
    if(!isset($file->fid) && is_numeric($file)) {
      $file = self::loadFile($file);
    }
    if(isset($file->uri)){
      return self::getFilepathByUri($file->uri);
    }
    return '';
  }
  
  /**
   * convert shorthand php.ini notation into bytes, much like how the PHP source does it
   * @link http://pl.php.net/manual/en/function.ini-get.php
   * @access public
   * @static
   * @param string $val {文件尺寸[数值g|G|m|M|k|K]}.
   * @return int
   */
  public static function getBytes($val) {
    $val = trim($val);
    if(!$val){ return 0; }
    $last = strtolower($val[strlen($val)-1]);
    switch($last) {
      // The 'G' modifier is available since PHP 5.1.0
      case 'g':
        $val *= 1024;
      case 'm':
        $val *= 1024;
      case 'k':
        $val *= 1024;
    }
    return $val;
  }

  /**
   *
   * @access public
   * @static
   * @param int $length
   * @param string $chars
   * @return string
   */
  public static function getRandomString($length, $chars = 'ABCDEFGHKLMNPRSTUVWYZabcdefghklmnprstuvwyz23456789'){
    $hashes = array();
    $max = mb_strlen($chars) - 1;
    while(count($hashes) < $length){
      $hashes[] = mt_rand(0, $max);
      $hashes = array_unique($hashes);
    }
    $string = "";
    foreach($hashes as $hash){
      $string .= mb_substr($chars, $hash, 1);
    }
    return $string;
  }

  /**
   *
   * @access public
   * @static
   * @param string $sessionname
   * @param string $securitycode
   * @param int $width
   * @param int $height
   */
  public static function generateSecurityPicture($sessionname, $securitycode, $width = 80, $height = 30){
    drupal_session_start();
    $_SESSION[$sessionname] = $securitycode;
    Header("Content-type: image/PNG");
    $im = imagecreate($width, $height);
    $white = imagecolorallocate($im, 255, 255, 255);
    $black = imagecolorallocate($im, 0, 0, 0);
    $randcolor = imagecolorallocate($im, mt_rand(0,200), mt_rand(0,200), mt_rand(0,200));
    imagefill($im, $width, $height, $white);
    imagerectangle($im, 0, 0, $width - 1, $height - 1, $black);
    $fontsize = $height * 0.5;
    $x = 5;
    $y = round($height / 2 - $fontsize / 2);
    $securitycode = implode(' ', str_split($securitycode));
    imagestring($im, $fontsize, $x, $y, $securitycode, $randcolor);
    for($i = 0;$i < 100;$i++){
      imagesetpixel($im, rand()%$width , rand()%$height , $black);
    }
    imagepng($im);
    imagedestroy($im);
  }

  /**
   *
   * @param string $action Includes 'show', 'play', 'check'
   * @param string $code Match security code if $action equals to 'check'.
   * @return mixed When $action equals to 'check', boolean true if security code is matched, false otherwise.
   */
  public static function generateSecurimage($action, $securitycode = ''){
    switch ($action) {
      case 'show':
        drupal_session_start();
        require_once 'misc/resources/securimage/securimage_show_coco.php';
        break;
      case 'play':
        require_once 'misc/resources/securimage/securimage_play.php';
        break;
      case 'check':
        require_once 'misc/resources/securimage/securimage.php';
        $securimage = new Securimage();
        if (empty($securitycode) || $securimage->check($securitycode) == false) {
          return false;
        }else{
          return true;
        }
        break;
    }
  }

  /**
   *
   * @access public
   * @static
   * @param string $chl The data saved.
   * @param int $width The chart size width, in pixels.
   * @param int $height The chart size height, in pixels.
   * @param string $error_correction_level The one of 'L', 'M', 'Q', 'H'.
   * @param type $margin image margin.
   * @return string image url.
   */
  public static function generateQRcodeFromGoogle($chl, $width = 150, $height = 150, $error_correction_level = 'L', $margin = 0){
    $arrInfos = array();
    $arrInfos[] = 'cht=qr';//The chart type.
    //$arrInfos[] = 'choe=UTF-8';//UTF-8, Shift_JIS, ISO-8859-1
    $arrInfos[] = 'chl=' . $chl;
    $arrInfos[] = 'chs=' . $width . 'x' . $height;
    $arrInfos[] = 'chld=' . $error_correction_level . '|' . $margin;
    return 'http://chart.apis.google.com/chart?' . implode('&', $arrInfos);
  }

  /**
   *
   * @access public
   * @static
   * @param int $fid A file ID.
   */
  public static function FileDownloadByFid($fid){
    $file = self::loadFile($fid);
    if($file && $file->fid){
      if(!file_exists($file->uri)){
        drupal_not_found();
        drupal_exit();
      }
      $user_agent = $_SERVER['HTTP_USER_AGENT'];
      if(preg_match('/MSIE/i', $user_agent)){//Internet Explorer
        $file->filename = drupal_encode_path($file->filename);
      }elseif(preg_match('/Firefox/i', $user_agent)){//Mozilla Firefox
      }elseif(preg_match('/Chrome/i', $user_agent)){//Google Chrome
      }elseif(preg_match('/Safari/i', $user_agent)){//Apple Safari
      }elseif(preg_match('/Opera/i', $user_agent)){//Opera
        $file->filename = drupal_encode_path($file->filename);
      }elseif(preg_match('/Netscape/i', $user_agent)){//Netscape
      }elseif(preg_match('/Flock/i', $user_agent)){//Flock
      }
      $file->filesize = filesize($file->uri);
      $headers = file_get_content_headers($file);
      /*** Content-Disposition start ***/
      $inline_types = variable_get('file_inline_types', array('^text/', '^image/', 'flash$'));
      $disposition = 'attachment';
      foreach ($inline_types as $inline_type) {
        // Exclamation marks are used as delimiters to avoid escaping slashes.
        if (preg_match('!' . $inline_type . '!', $file->filemime)) {
          $disposition = 'inline';
        }
      }
      $headers['Content-Disposition'] = $disposition . '; filename="' . mime_header_encode($file->filename) . '"';
      /*** Content-Disposition end ***/
      file_transfer($file->uri, $headers);
    }
  }

  /**
   *
   * @access public
   * @static
   * @param string $filepath Path to the file.
   * @param string $filename Whether to custom file name.
   * @param boolean $remove Whether to remove the file after downloading.
   */
  public static function FileDownloadByPath($filepath, $filename = '', $remove = false){
    $file = new stdClass();
    $file->filepath = trim($filepath, ' .');
    if(!file_exists($file->filepath)){
      drupal_not_found();
      drupal_exit();
    }
    $file->filename = $filename;
    if($file->filename == '' || self::getFileExtension($file->filename) != self::getFileExtension($file->filepath)){
      $file->filename = self::getFileName($file->filepath);
    }
    $file->filemime = file_get_mimetype($file->filepath);
    $user_agent = $_SERVER['HTTP_USER_AGENT'];
    if(preg_match('/MSIE/i', $user_agent)){//Internet Explorer
      $file->filename = drupal_encode_path($file->filename);
    }elseif(preg_match('/Firefox/i', $user_agent)){//Mozilla Firefox
    }elseif(preg_match('/Chrome/i', $user_agent)){//Google Chrome
    }elseif(preg_match('/Safari/i', $user_agent)){//Apple Safari
    }elseif(preg_match('/Opera/i', $user_agent)){//Opera
      $file->filename = drupal_encode_path($file->filename);
    }elseif(preg_match('/Netscape/i', $user_agent)){//Netscape
    }elseif(preg_match('/Flock/i', $user_agent)){//Flock
    }
    $file->filesize = filesize($file->filepath);
    $headers = file_get_content_headers($file);
    /*** Content-Disposition start ***/
    $inline_types = variable_get('file_inline_types', array('^text/', '^image/', 'flash$'));
    $disposition = 'attachment';
    foreach ($inline_types as $inline_type) {
      // Exclamation marks are used as delimiters to avoid escaping slashes.
      if (preg_match('!' . $inline_type . '!', $file->filemime)) {
        $disposition = 'inline';
      }
    }
    $headers['Content-Disposition'] = $disposition . '; filename="' . mime_header_encode($file->filename) . '"';
    /*** Content-Disposition end ***/

    if (ob_get_level()) {
      ob_end_clean();
    }
    foreach ($headers as $name => $value) {
      drupal_add_http_header($name, $value);
    }
    drupal_send_headers();
    if ($fd = fopen($file->filepath, 'rb')) {
      while (!feof($fd)) {
        print fread($fd, 1024);
      }
      fclose($fd);
      if($remove){ self::Remove($filepath); }
    } else {
      drupal_not_found();
    }
    drupal_exit();
  }

  /**
   *
   * @access public
   * @static
   * @param string $url
   * @param mixed $post <p>
   * May be an array or object containing properties.
   * </p>
   * <p>
   * If <i>query_data</i> is an array, it may be a simple
   * one-dimensional structure, or an array of arrays (which in
   * turn may contain other arrays).
   * </p>
   * <p>
   * If <i>query_data</i> is an object, then only public
   * properties will be incorporated into the result.
   * </p>
   * @return string The function returns the read data or FALSE on failure.
   */
  public static function PostUrl($url, $post = array()){
    $options = array(
      'http' => array(
        'method' => 'POST',
        'header' => 'Content-type:application/x-www-form-urlencoded',
        'content' => http_build_query($post),
        'timeout' => 15 * 60 // timeout(unit:s)
      )
    );
    $context = stream_context_create($options);
    $result = file_get_contents($url, false, $context);
    return $result;
  }
  /**
   *
   * @access public
   * @static
   * @param string $url
   * @param array $get
   * @return string The function returns the read data or FALSE on failure.
   */
  public static function GetUrl($url, $get = array()){
    $newget = array();
    foreach($get as $key => $value){
      $newget[] = $key . '=' . $value;
    }
    $result = file_get_contents($url . '?' . implode('&', $newget));
    return $result;
  }
}