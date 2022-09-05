# 概述

CCMS 是基于 Drupal 的 CMS 内容管理系统

# 运行环境
- PHP 5.6+
- Drupal 7.x

# 安装流程

### 1. 安装 Drupal 7.23

### 2. 启用 Mini Field、Cms系列模块
 - `Mini Field` 模块可以拿出来单独安装使用。

### 3. 基本设置

 - 位置：Home > Administration > Configuration > System > Site information
 - 路径：admin/config/system/site-information
 - 更改 SITE DETAILS 信息
  >- 更改 FRONT PAGE - Default front page 为 `siteroot`
  >- 更改 ERROR PAGES - Default 403 (access denied) page 为 `site403`
  >- 更改 ERROR PAGES - Default 404 (not found) page 为 `site404`

### 4. 启用验证码权限

 - 位置：Home > Administration > People > Permissions
 - 路径：admin/people/permissions
 - 为 `Cms data > CMS文件 高级验证码` 启用 `anonymous user` 和 `authenticated user` 权限
 - 注：登录CMS时需要用到此验证码功能

### 5. 登录CMS（路径：cms）

### 6. 设置网站信息：

 - 网站标题：必须填写
 - 主题路径：建议为 `sites/all/themes/*`（ * 为目录名称）
 - 静态化路径：建议留空

### 7. 访问 `网站角色` 页，执行 `清理角色` 操作，然后编辑角色并保存

### 8. 访问 `网站用户` 里的各用户列表页，执行 `清理用户` 操作

### 9. 文件配置

 - 路径：sites/default/settings.php
 - 示例：
   ```php
    /*  */
    $databases = array (
      'default' => 
      array (
        'default' => 
        array (
          'database' => 'community',
          'username' => 'root',
          'password' => 'nosmoking',
          'host' => 'localhost',
          'port' => '',
          'driver' => 'mysql',
          'prefix' => '',
        ),
      ),
      'sso_db' => 
      array (
        'default' => 
        array (
          'database' => 'sso',
          'username' => 'root',
          'password' => 'nosmoking',
          'host' => 'localhost',
          'port' => '3306',
          'driver' => 'mysql',
          'prefix' => '',
        ),  
      ),
    );
    /*  */
    $conf['cache'] = 1;
    $conf['block_cache'] = 1;
    $conf['page_compression'] = 1;
    $conf['preprocess_css'] = 1;
    $conf['preprocess_js'] = 1;
    $conf['css_gzip_compression'] = true;
    $conf['js_gzip_compression'] = true;
    /*  */
    $conf['lock_inc'] = 'sites/all/modules/memcache/memcache-lock.inc';
    $conf['session_inc'] = 'sites/all/modules/memcache/unstable/memcache-session.inc';
    $conf['session_write_interval'] = 180;
    $conf['cache_backends'][] = 'sites/all/modules/memcache/memcache.inc';
    $conf['memcache_stampede_protection'] = TRUE;
    $conf['cache_default_class'] = 'MemCacheDrupal';
    $conf['cache_backends'][] = 'sites/all/modules/filecache/filecache.inc';
    $conf['filecache_directory'] = '/tmp/filecache-' . substr(conf_path(), 6);
    $conf['cache_class_cache_form'] = 'DrupalFileCache';
    $conf['page_cache_without_database'] = TRUE;
    $conf['page_cache_invoke_hooks'] = FALSE;
   ```

### 10. 引入 Css、Javascript 资源

 - 函数：
   ```php
   cmsAddCss($data = NULL, $options = NULL); //添加CSS资源
   cmsAddJs($data = NULL, $options = NULL); //添加Javascript资源
   ```
 - 示例：
   ```php
   /* 添加项目内部 Css 资源 */
   cmsAddCss(CMS_MODULE_PATH . '/tpl/css/cmsFramePage.css');
   cmsAddCss(CMSSiteInfo::GetAttr('themepath') . '/css/msgbox.css');
   /* 添加项目内部 Javascript 资源 */
   cmsAddJs(CMS_MODULE_PATH . '/tpl/js/preprocess.js');
   cmsAddJs(CMSSiteInfo::GetAttr('themepath') . '/js/msgbox.js');
   /* 添加项目外部 Css 资源 */
   cmsAddCss('http://api.map.baidu.com/library/SearchInfoWindow/1.5/src/SearchInfoWindow_min.css', 'external');//百度搜索信息窗口CSS
   /* 添加项目外部 Javascript 资源 */
   cmsAddJs('http://api.map.baidu.com/library/SearchInfoWindow/1.5/src/SearchInfoWindow_min.js', 'external');//百度搜索信息窗口JS
   ```
 - 注释：添加的项目外部 Css、Javascript 资源，不会被合并压缩。
 - 后台引入公用的 Css、Javascript 资源：
   ```php
   在 cms 模块的钩子函数 cms_init() 里添加 Css、Javascript 资源
   在 cms 模块的钩子函数 cms_css_alter 里修改（设置访问那些模块时，添加的Css资源才会有效）
   在 cms 模块的钩子函数 cms_js_alter() 里修改（设置访问那些模块时，添加的Javascript资源才会有效）
   ```
 - 前台引入公用的 Css、Javascript 资源：
   ```php
   在 cmssite 模块的钩子函数 cmssite_init() 里添加 Css、Javascript 资源
   在 cmssite 模块的钩子函数 cmssite_css_alter 里修改（设置访问那些模块时，添加的Css资源才会有效）
   在 cmssite 模块的钩子函数 cmssite_js_alter() 里修改（设置访问那些模块时，添加的Javascript资源才会有效）
   ```
 - 各个界面独自需要的 Css、Javascript 资源，直接在相应的 `.tpl.php` 文件里引入即可（在输出 `drupal_get_css()` 之前）
 