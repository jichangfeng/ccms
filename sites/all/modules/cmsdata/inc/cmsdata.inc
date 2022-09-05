<?php
/**
 *
 * @param type $handle handle style (list, add, edit, delete)
 * @param type $cid Category Identification number
 * @param type $id Data Identification number
 * @return type
 */
function cmsdata($handle, $cid, $id = 0){
  if(!isset(CMSDataField::$AllowedCides[$cid])){
    return '访问页面不存在！';
  }
  unset ($cides);

  $arrHandles = array('list', 'add', 'edit', 'delete');
  if(!in_array($handle, $arrHandles)){
    return $handle . ' 处理方式未定义！';
  }
  unset ($arrHandles);
  $filepath = "cmsdata.$handle.inc";
  if(!file_exists(CMSDATA_MODULE_PATH . "/inc/" . $filepath)){
    return CMSDATA_MODULE_PATH . "/inc/" . $filepath . ' 文件未找到！';
  }
  include $filepath;
  $function = "cmsdata_" . $handle;
  if(function_exists($function)){
    return $id == 0 ? $function($cid) : $function($cid, $id);
  }else{
    return $handle . ' 处理函数未定义！';
  }
}

/**
 *
 * @param type $mixed Category identification number or other.
 */
function cmsdata_handle($mixed){
  if($mixed == CMSDATA_CID_NAVIGATION){
    return cmsdata_handle_navigation();
  }else if($mixed == CMSDATA_CID_TRANSLATION){
    return cmsdata_handle_translation();
  }else if($mixed == 'file'){
    return cmsdata_handle_file();
  }
  $arg = arg(2);
  if(is_string($arg) && $arg == 'menurebuild'){
    menu_rebuild();
    echo 'ok';exit;
  }else if(is_string($arg) && $arg == 'clearcache'){
    cache_clear_all('cmscache', 'cache', true);
    MiniFieldStatic::ClearStaticsAll();
    drupal_flush_all_caches();
    db_truncate('watchdog')->execute();
    echo 'ok';exit;
  }else if(is_string($arg) && $arg == 'delete'){
    if(isset($_POST['ides']) && is_array($_POST['ides'])){
      MiniFieldData::DeleteDatasByCid($mixed, array(array('id', $_POST['ides'], 'IN')), true);
    }
    echo 'ok';exit;
  }
  exit;
}
function cmsdata_handle_navigation(){
  $arg = arg(2);
  $arrHtmls = array(
    'GenerateIndexHtml', 'ClearIndexHtml',
    'GenerateTplindexHtmlsAll', 'ClearTplindexHtmlsAll',
    'GenerateTplindexHtmlsByNavtags', 'ClearTplindexHtmlsByNavtags',
    'GenerateTplarticleHtmlsAll', 'ClearTplarticleHtmlsAll',
    'GenerateTplarticleHtmlsAllByNavtags', 'ClearTplarticleHtmlsAllByNavtags',
    'GenerateTplarticleHtmlsAllByNavtag', 'ClearTplarticleHtmlsAllByNavtag',
    'GenerateTplarticleHtmlsByNavtagNodeides', 'ClearTplarticleHtmlsByNavtagNodeides'
  );
  if(is_string($arg) && $arg == 'delete'){
    if(isset($_POST['navtags']) && is_array($_POST['navtags'])){
      CMSNavigation::DeleteNavigations($_POST['navtags']);
    }
    echo 'ok';exit;
  }else if(is_string($arg) && in_array($arg, $arrHtmls)){
    $staticpath = CMSSiteInfo::GetAttr('staticpath');
    $max_execution_time = (int)ini_get('max_execution_time');
    ini_set('max_execution_time', $max_execution_time + 1 * 60 * 60);
    switch ($arg) {
      case 'GenerateIndexHtml':
        variable_set('GenerateIndexHtml', 1);
        $filepath = CMSSiteInfo::GetAttr('themepath') . '/tpl/index.tpl.php';
        $content = MiniFieldCommon::Render($filepath, array());
        $destination = 'index.html';
        file_unmanaged_save_data($content, $destination, FILE_EXISTS_REPLACE);
        variable_del('GenerateIndexHtml');
        break;
      case 'ClearIndexHtml':
        MiniFieldFile::Remove('index.html');
        break;
      case 'GenerateTplindexHtmlsAll':
        variable_set('GenerateTplindexHtml', 1);
        CMSNavigation::GenerateTplindexHtmlsAll($staticpath);
        variable_del('GenerateTplindexHtml');
        break;
      case 'ClearTplindexHtmlsAll':
        CMSNavigation::ClearTplindexHtmlsAll($staticpath);
        break;
      case 'GenerateTplindexHtmlsByNavtags':
        if(isset($_POST['navtags']) && is_array($_POST['navtags'])){
          variable_set('GenerateTplindexHtml', 1);
          CMSNavigation::GenerateTplindexHtmlsByNavtags($staticpath, $_POST['navtags']);
          variable_del('GenerateTplindexHtml');
        }
        break;
      case 'ClearTplindexHtmlsByNavtags':
        if(isset($_POST['navtags']) && is_array($_POST['navtags'])){
          CMSNavigation::ClearTplindexHtmlsByNavtags($staticpath, $_POST['navtags']);
        }
        break;
      case 'GenerateTplarticleHtmlsAll':
        variable_set('GenerateTplarticleHtml', 1);
        CMSNavigation::GenerateTplarticleHtmlsAll($staticpath);
        variable_del('GenerateTplarticleHtml');
        break;
      case 'ClearTplarticleHtmlsAll':
        CMSNavigation::ClearTplarticleHtmlsAll($staticpath);
        break;
      case 'GenerateTplarticleHtmlsAllByNavtags':
        if(isset($_POST['navtags']) && is_array($_POST['navtags'])){
          variable_set('GenerateTplarticleHtml', 1);
          CMSNavigation::GenerateTplarticleHtmlsAllByNavtags($staticpath, $_POST['navtags']);
          variable_del('GenerateTplarticleHtml');
        }
        break;
      case 'ClearTplarticleHtmlsAllByNavtags':
        if(isset($_POST['navtags']) && is_array($_POST['navtags'])){
          CMSNavigation::ClearTplarticleHtmlsAllByNavtags($staticpath, $_POST['navtags']);
        }
      case 'GenerateTplarticleHtmlsAllByNavtag':
        if(isset($_POST['navtag']) && (is_numeric($_POST['navtag']) || is_string($_POST['navtag']))){
          variable_set('GenerateTplarticleHtml', 1);
          CMSNavigation::GenerateTplarticleHtmlsAllByNavtag($staticpath, $_POST['navtag']);
          variable_del('GenerateTplarticleHtml');
        }
        break;
      case 'ClearTplarticleHtmlsAllByNavtag':
        if(isset($_POST['navtag']) && (is_numeric($_POST['navtag']) || is_string($_POST['navtag']))){
          CMSNavigation::ClearTplarticleHtmlsAllByNavtag($staticpath, $_POST['navtag']);
        }
      case 'GenerateTplarticleHtmlsByNavtagNodeides':
        if(isset($_POST['navtag']) && (is_numeric($_POST['navtag']) || is_string($_POST['navtag']))){
          if(isset($_POST['nodeides']) && is_array($_POST['nodeides'])){
            variable_set('GenerateTplarticleHtml', 1);
            CMSNavigation::GenerateTplarticleHtmlsByNavtagNodeides($staticpath, $_POST['navtag'], $_POST['nodeides']);
            variable_del('GenerateTplarticleHtml');
          }
        }
        break;
      case 'ClearTplarticleHtmlsByNavtagNodeides':
        if(isset($_POST['navtag']) && (is_numeric($_POST['navtag']) || is_string($_POST['navtag']))){
          if(isset($_POST['nodeides']) && is_array($_POST['nodeides'])){
            CMSNavigation::ClearTplarticleHtmlsByNavtagNodeides($staticpath, $_POST['navtag'], $_POST['nodeides']);
          }
        }
        break;
      default:
        break;
    }
    ini_set('max_execution_time', $max_execution_time);
    echo 'ok';exit;
  }
  exit;
}
function cmsdata_handle_translation(){
  $arg = arg(2);
  if(is_string($arg) && $arg == 'fixed'){
    $variables = array(
      'Category' => MiniFieldCategory::LoadDataByCidId(null, CMSDATA_CID_TRANSLATION, true),
      'arrDatas' => MiniFieldTranslation::Translations(false)
    );
    $filepath = CMSDATA_MODULE_PATH."/tpl/cmsdata-list-fixedtrans.tpl.php";
    return MiniFieldCommon::Render($filepath, $variables);
  }
  exit;
}
function cmsdata_handle_file(){
  $arg = arg(2);
  if(is_string($arg) && $arg == 'delete'){
    $fid = arg(3);
    $file = file_load($fid);
    if($file && $file->fid){
      echo file_delete($file, true) ? 'success' : 'failed';exit;
    }
    echo 'failed';exit;
  }else if(is_string($arg) && $arg == 'download'){
    $fid = arg(3);
    MiniFieldFile::FileDownloadByFid($fid);
    echo 'File can not be downloaded';exit;
  }else if(is_string($arg) && $arg == 'securitypic'){
    $sessionname = arg(3);
    if(is_string($sessionname) || is_numeric($sessionname)){
      MiniFieldFile::generateSecurityPicture($sessionname, MiniFieldFile::getRandomString(4));
    }
    exit;
  }else if(is_string($arg) && $arg == 'securimage'){
    MiniFieldFile::generateSecurimage(arg(3));
    exit;
  }
  exit;
}