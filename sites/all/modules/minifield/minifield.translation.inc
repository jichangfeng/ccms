<?php
/**
 *
 * @param string $text standard text.
 * @param string $handle handle function. Possible values are :
 *  - strtolower : Make a string lowercase
 *  - strtoupper : Make a string uppercase
 *  - lcfirst : Make a string's first character lowercase
 *  - ucfirst : Make a string's first character uppercase
 *  - ucwords : Uppercase the first character of each word in a string
 * @param mixed $tag translation identifier or group by:
 *  - pager, groupby, default, navigation, node, role, user, translation, cmsfield, minifield
 * @param $args
 *   An associative array of replacements to make after translation. Based
 *   on the first character of the key, the value is escaped and/or themed.
 *   See format_string() for details.
 * @return string
 * @author Changfeng Ji <jichf@qq.com>
 */
function ts($text, $handle = '', $tag = 'all', array $args = array()){
  return MiniFieldTranslation::GetTranslation($text, $handle, $tag, $args);
}

final class MiniFieldTranslation{
  /**
   * language tag array
   * @access public
   * @static
   * @var array. 
   * Each element's key and value all are a string of language tag:
   *  -  zhs: Simplified Chinese
   *  -  zht: Traditional Chinese
   *  -  enus: United States English
   */
  public static $Languages = array(
      'zhs' => 'zhs',
      'zht' => 'zht',
      'enus' => 'enus'
  );

  public static function SetLanguage($lang){
    global $minifield;
    $minifield['lang'] = $lang;
  }

  /**
   *
   * @access public
   * @static
   * @param array $translations
   * @param bool $priority it's high priority if $priority is true, and low priority otherwise.
   */
  public static function AddTranslations($translations, $priority){
    global $minifield;
    if(!isset($minifield['translations']) || !is_array($minifield['translations'])){
      $minifield['translations'] = array();
    }
    if(is_array($translations) && $priority == true){
      $minifield['translations'] = array_merge_recursive($minifield['translations'], $translations);
    }else if(is_array($translations)){
      $minifield['translations'] = array_merge_recursive($translations, $minifield['translations']);
    }
  }

  /**
   *
   * @access public
   * @static
   * @param string $text standard text.
   * @param string $handle handle function. Possible values are :
   * - strtolower : Make a string lowercase
   * - strtoupper : Make a string uppercase
   * - lcfirst : Make a string's first character lowercase
   * - ucfirst : Make a string's first character uppercase
   * - ucwords : Uppercase the first character of each word in a string
   * @param mixed $tag translation identifier or group by(default, siteinfo, navigation, node, userinfo, roleinfo, translation).
   * @param $args
 *   An associative array of replacements to make after translation. Based
 *   on the first character of the key, the value is escaped and/or themed.
 *   See format_string() for details.
   * @return string
   */
  public static function GetTranslation($text, $handle = '', $tag = 'default', array $args = array()){
    global $minifield;
    $lang = (isset($minifield['lang']) && in_array($minifield['lang'], self::$Languages)) ? $minifield['lang'] : self::$Languages['zhs'];
    if(isset($minifield['translations']['id'][$tag])){
      $Data = $minifield['translations']['id'][$tag];
      $text = isset($Data->$lang) ? $Data->$lang : $text;
    }else if(isset($minifield['translations'][$tag][$text])){
      $Data = $minifield['translations'][$tag][$text];
      $text = isset($Data->$lang) ? $Data->$lang : $text;
    }
    $text = empty($args) ? $text: format_string($text, $args);
    if(in_array($handle, array('strtolower', 'strtoupper', 'lcfirst', 'ucfirst', 'ucwords'))){
      $text = $handle($text);
    }
    return $text;
  }

  /**
   *
   * @access public
   * @static
   * @param string $separator Defaults to an string '\0'.
   * @param string $group Defaults to an string ''.
   * @return array Each element is still a array with one to four elements($value[, $name, $title, $selected])
   */
  public static function GroupBies($separator = '\0', $group = ''){
    $arrValueArrays = array(
      array('default', ts('默认', 'ucwords', 'groupby'),"", 1),
      array('translation', ts('网站翻译', 'ucwords', 'groupby'), ""),
      array('information', ts('网站信息', 'ucwords', 'groupby'), ""),
      array('navigation', ts('网站栏目', 'ucwords', 'groupby'), ""),
      array('node', ts('栏目内容', 'ucwords', 'groupby'), ""),
      array('role', ts('网站角色', 'ucwords', 'groupby'), ""),
      array('user', ts('网站用户', 'ucwords', 'groupby'), "")
    );
    return $arrValueArrays;
  }

  /**
   * default translations
   * @access public
   * @static
   * @param bool $handle
   * @return array
   */
  public static function Translations($handle){
    $translations = array();
    $arrDatas = array();
    $arrDatas['groupby'] = array(
      (object)array('zhs' => '默认', 'zht' => '默認', 'enus' => 'default'),
      (object)array('zhs' => '网站翻译', 'zht' => '網站翻譯', 'enus' => 'site translation'),
      (object)array('zhs' => '网站信息', 'zht' => '網站信息', 'enus' => 'site information'),
      (object)array('zhs' => '网站栏目', 'zht' => '網站欄目', 'enus' => 'site navigation'),
      (object)array('zhs' => '栏目内容', 'zht' => '欄目內容', 'enus' => 'navigation content'),
      (object)array('zhs' => '网站角色', 'zht' => '網站角色', 'enus' => 'site role'),
      (object)array('zhs' => '网站用户', 'zht' => '網站用戶', 'enus' => 'site user')
    );
    $arrDatas['default'] = array(
      (object)array('zhs' => 'zh-CN', 'zht' => 'zh-TW', 'enus' => 'en-GB'),
      (object)array('zhs' => '前台首页', 'zht' => '前臺首頁', 'enus' => 'forestage homepage'),
      (object)array('zhs' => '后台首页', 'zht' => '後臺首頁', 'enus' => 'backstage homepage'),
      (object)array('zhs' => '内容管理', 'zht' => '內容管理', 'enus' => 'content management'),
      (object)array('zhs' => '会员管理', 'zht' => '會員管理', 'enus' => 'member management'),
      (object)array('zhs' => '字段管理', 'zht' => '字段管理', 'enus' => 'field management'),
      (object)array('zhs' => '网站翻译', 'zht' => '網站翻譯', 'enus' => 'site translation'),
      (object)array('zhs' => '网站信息', 'zht' => '網站信息', 'enus' => 'site information'),
      (object)array('zhs' => '网站栏目', 'zht' => '網站欄目', 'enus' => 'site navigation'),
      (object)array('zhs' => '栏目内容', 'zht' => '欄目內容', 'enus' => 'navigation content'),
      (object)array('zhs' => '用户资料', 'zht' => '用戶資料', 'enus' => 'user material'),
      (object)array('zhs' => '网站用户', 'zht' => '網站用戶', 'enus' => 'site user'),
      (object)array('zhs' => '网站角色', 'zht' => '網站角色', 'enus' => 'site role'),
      (object)array('zhs' => '网站类别', 'zht' => '網站類別', 'enus' => 'site category'),
      (object)array('zhs' => '网站字段', 'zht' => '網站字段', 'enus' => 'site field'),
      (object)array('zhs' => '网站用户字段', 'zht' => '網站用戶字段', 'enus' => 'site user field'),
      (object)array('zhs' => '网站内容字段', 'zht' => '網站内容字段', 'enus' => 'site content field'),
      (object)array('zhs' => '注册', 'zht' => '註冊', 'enus' => 'register'),
      (object)array('zhs' => '登录', 'zht' => '登錄', 'enus' => 'login'),
      (object)array('zhs' => '注销', 'zht' => '註銷', 'enus' => 'logout'),
      (object)array('zhs' => '操作', 'zht' => '操作', 'enus' => 'operation'),
      (object)array('zhs' => '全选', 'zht' => '全選', 'enus' => 'check all'),
      (object)array('zhs' => '全反选', 'zht' => '全反選', 'enus' => 'uncheck all'),
      (object)array('zhs' => '列表', 'zht' => '列表', 'enus' => 'list'),
      (object)array('zhs' => '查看', 'zht' => '查看', 'enus' => 'view'),
      (object)array('zhs' => '添加', 'zht' => '添加', 'enus' => 'add'),
      (object)array('zhs' => '编辑', 'zht' => '編輯', 'enus' => 'edit'),
      (object)array('zhs' => '删除', 'zht' => '刪除', 'enus' => 'delete'),
      (object)array('zhs' => '保存', 'zht' => '保存', 'enus' => 'save'),
      (object)array('zhs' => '重置', 'zht' => '重置', 'enus' => 'reset'),
      (object)array('zhs' => '预览', 'zht' => '預覽', 'enus' => 'preview'),
      (object)array('zhs' => '成功', 'zht' => '成功', 'enus' => 'success'),
      (object)array('zhs' => '失败', 'zht' => '失敗', 'enus' => 'failed'),
      (object)array('zhs' => '删除', 'zht' => '刪除', 'enus' => 'delete'),
      (object)array('zhs' => '搜索', 'zht' => '搜索', 'enus' => 'search'),
      (object)array('zhs' => '权重', 'zht' => '權重', 'enus' => 'weight'),
      (object)array('zhs' => '排序', 'zht' => '排序', 'enus' => 'sort'),
      (object)array('zhs' => '升序', 'zht' => '升序', 'enus' => 'ascending'),
      (object)array('zhs' => '降序', 'zht' => '降序', 'enus' => 'descending'),
      (object)array('zhs' => '唯一', 'zht' => '唯一', 'enus' => 'unique'),
      (object)array('zhs' => '执行中', 'zht' => '執行中', 'enus' => 'runing'),
      (object)array('zhs' => '请至少勾选一项', 'zht' => '請至少勾選一項', 'enus' => 'please check one at least'),
      (object)array('zhs' => '确定要', 'zht' => '確定要', 'enus' => 'confirm to'),
      (object)array('zhs' => '确定要删除此数据吗？', 'zht' => '確定要刪除此數據嗎？', 'enus' => 'confirm to delete this data?'),
      (object)array('zhs' => '确定要删除勾选的数据吗？', 'zht' => '確定要刪除勾選的數據嗎？', 'enus' => 'confirm to delete checked datas?')
    );
    $arrDatas['translation'] = array(
      (object)array('zhs' => '添加翻译', 'zht' => '添加翻譯', 'enus' => 'add translation'),
      (object)array('zhs' => '全部', 'zht' => '全部', 'enus' => 'all'),
      (object)array('zhs' => '简体中文', 'zht' => '簡體中文', 'enus' => 'simplified chinese'),
      (object)array('zhs' => '繁体中文', 'zht' => '繁体中文', 'enus' => 'traditional chinese'),
      (object)array('zhs' => '美国英文', 'zht' => '美國英文', 'enus' => 'united states english'),
      (object)array('zhs' => '固定翻译', 'zht' => '固定翻譯', 'enus' => 'fixed translation'),
      (object)array('zhs' => '隶属于', 'zht' => '隸屬於', 'enus' => 'group by'),
      (object)array('zhs' => '优先级较高', 'zht' => '優先級較高', 'enus' => 'higher priority'),
      (object)array('zhs' => '优先级较低', 'zht' => '優先級較低', 'enus' => 'lower priority')
    );
    $arrDatas['information'] = array(
        
    );
    $arrDatas['navigation'] = array(
      (object)array('zhs' => '添加栏目', 'zht' => '添加欄目', 'enus' => 'add navigation'),
      (object)array('zhs' => '菜单重建', 'zht' => '菜單重建', 'enus' => 'menu rebuild'),
      (object)array('zhs' => '清理缓存', 'zht' => '清理緩存', 'enus' => 'clear cache'),
      (object)array('zhs' => '生成首页HTML', 'zht' => '生成首頁HTML', 'enus' => 'generate home HTML'),
      (object)array('zhs' => '清理首页HTML', 'zht' => '清理首頁HTML', 'enus' => 'clear home HTML'),
      (object)array('zhs' => '生成所有栏目的栏目模板HTML', 'zht' => '生成所有欄目的欄目模板HTML', 'enus' => 'generate all navigation template HTML'),
      (object)array('zhs' => '清理所有栏目的栏目模板HTML', 'zht' => '清理所有欄目的欄目模板HTML', 'enus' => 'clear all navigation template HTML'),
      (object)array('zhs' => '生成部分栏目的栏目模板HTML', 'zht' => '生成部份欄目的欄目模板HTML', 'enus' => 'generate part navigation template HTML'),
      (object)array('zhs' => '清理部分栏目的栏目模板HTML', 'zht' => '清理部份欄目的欄目模板HTML', 'enus' => 'clear part navigation template HTML'),
      (object)array('zhs' => '生成所有栏目的所有内容模板HTML', 'zht' => '生成所有欄目的所有內容模板HTML', 'enus' => 'generate all content template HTML of all navigations'),
      (object)array('zhs' => '清理所有栏目的所有内容模板HTML', 'zht' => '清理所有欄目的所有內容模板HTML', 'enus' => 'clear all content template HTML of all navigations'),
      (object)array('zhs' => '生成部分栏目的所有内容模板HTML', 'zht' => '生成部份欄目的所有內容模板HTML', 'enus' => 'generate part content template HTML of all navigations'),
      (object)array('zhs' => '清理部分栏目的所有内容模板HTML', 'zht' => '清理部份欄目的所有內容模板HTML', 'enus' => 'clear part content template HTML of all navigations'),
      (object)array('zhs' => '生成单个栏目的所有内容模板HTML', 'zht' => '生成單個欄目的所有內容模板HTML', 'enus' => 'generate all content template HTML of one navigation'),
      (object)array('zhs' => '清理单个栏目的所有内容模板HTML', 'zht' => '清理單個欄目的所有內容模板HTML', 'enus' => 'clear all content template HTML of all navigation'),
      (object)array('zhs' => '生成单个栏目的部分内容模板HTML', 'zht' => '生成單個欄目的部份內容模板HTML', 'enus' => 'generate part content template HTML of one navigation'),
      (object)array('zhs' => '清理单个栏目的部分内容模板HTML', 'zht' => '清理單個欄目的部份內容模板HTML', 'enus' => 'clear part content template HTML of all navigation'),
      (object)array('zhs' => '当前栏目或子栏目下存在内容，不能删除', 'zht' => '當前欄目或子欄目下存在內容，不能刪除', 'enus' => 'content exist under the current navigation or its child navigations, can not be deleted'),
      (object)array('zhs' => '栏目或其子栏目下存在内容时，不能删除', 'zht' => '欄目或其子欄目下存在內容時，不能刪除', 'enus' => 'content exist under the navigation or its child navigations, can not be deleted'),
      (object)array('zhs' => '确定要删除此栏目吗？', 'zht' => '確定要刪除此欄目嗎？', 'enus' => 'confirm to delete this navigation?'),
      (object)array('zhs' => '确定要删除勾选的栏目吗？', 'zht' => '確定要刪除勾選的欄目嗎？', 'enus' => 'confirm to delete checked navigations?'),
      (object)array('zhs' => '确定要执行菜单重建操作吗？', 'zht' => '確定要執行菜單重建操作嗎？', 'enus' => 'confirm to perform menu rebuild operation?'),
      (object)array('zhs' => '确定要执行清理缓存操作吗？', 'zht' => '確定要執行清理緩存操作嗎？', 'enus' => 'confirm to perform clear cache operation?'),
    );
    $arrDatas['node'] = array(
      (object)array('zhs' => '内容结构树', 'zht' => '內容結構樹', 'enus' => 'content structure trees'),
      (object)array('zhs' => '内容', 'zht' => '內容', 'enus' => 'content'),
      (object)array('zhs' => '当前栏目不支持投稿', 'zht' => '當前欄目不支持投稿', 'enus' => 'current navigation can not add content'),
      (object)array('zhs' => '内容列表', 'zht' => '內容列表', 'enus' => 'content list'),
      (object)array('zhs' => '内容编辑', 'zht' => '內容編輯', 'enus' => 'content edit'),
      (object)array('zhs' => '全部', 'zht' => '全部', 'enus' => 'all'),
      (object)array('zhs' => '已发布', 'zht' => '已發佈', 'enus' => 'published'),
      (object)array('zhs' => '未发布', 'zht' => '未發佈', 'enus' => 'not published'),
      (object)array('zhs' => '标记为', 'zht' => '標記為', 'enus' => 'marked as'),
      (object)array('zhs' => '移动到', 'zht' => '移動到', 'enus' => 'moved to'),
      (object)array('zhs' => '仅支持绿色字体栏目', 'zht' => '僅支持綠色字體欄目', 'enus' => 'support only navigation of the green font'),
      (object)array('zhs' => '生成内容模板HTML', 'zht' => '生成內容模板HTML', 'enus' => 'generate content template HTML'),
      (object)array('zhs' => '清理内容模板HTML', 'zht' => '清理內容模板HTML', 'enus' => 'clear content template HTML'),
      (object)array('zhs' => '确定要执行标记吗？', 'zht' => '確定要執行標記嗎？', 'enus' => 'confirm to perform mark operation?'),
      (object)array('zhs' => '确定要执行移动吗？', 'zht' => '確定要執行移動嗎？', 'enus' => 'confirm to perform move operation?'),
    );
    $arrDatas['role'] = array(
      (object)array('zhs' => '添加角色', 'zht' => '添加角色', 'enus' => 'add role'),
      (object)array('zhs' => '清理角色', 'zht' => '清理角色', 'enus' => 'clear roles'),
      (object)array('zhs' => '角色权限', 'zht' => '角色權限', 'enus' => 'role permissions'),
      (object)array('zhs' => '角色名', 'zht' => '角色名', 'enus' => 'role name'),
      (object)array('zhs' => '确定要删除此角色吗？', 'zht' => '確定要刪除此角色嗎？', 'enus' => 'confirm to delete this role?'),
      (object)array('zhs' => '确定要删除勾选的角色吗？', 'zht' => '確定要刪除勾選的角色嗎？', 'enus' => 'confirm to delete checked roles?'),
      (object)array('zhs' => '当前角色下存在用户，不能删除', 'zht' => '當前角色下存在用戶，不能刪除', 'enus' => 'users exist under the current role, can not be deleted'),
      (object)array('zhs' => '角色下存在用户时，不能删除', 'zht' => '角色下存在用戶時，不能刪除', 'enus' => 'users exist under the role, can not be deleted')
    );
    $arrDatas['user'] = array(
      (object)array('zhs' => '用户结构树', 'zht' => '用戶結構樹', 'enus' => 'user structure trees'),
      (object)array('zhs' => '添加用户', 'zht' => '添加用戶', 'enus' => 'add user'),
      (object)array('zhs' => '清理用户', 'zht' => '清理用戶', 'enus' => 'clear users'),
      (object)array('zhs' => '全部用户', 'zht' => '全部用戶', 'enus' => 'all users'),
      (object)array('zhs' => '被限制的用户', 'zht' => '被限制的用戶', 'enus' => 'restricted users'),
      (object)array('zhs' => '正常用户', 'zht' => '正常用戶', 'enus' => 'normal users'),
      (object)array('zhs' => '用户名', 'zht' => '用戶名', 'enus' => 'user name'),
      (object)array('zhs' => '密码', 'zht' => '密碼', 'enus' => 'password'),
      (object)array('zhs' => '邮箱', 'zht' => '郵箱', 'enus' => 'mail'),
      (object)array('zhs' => '手机号', 'zht' => '手機號', 'enus' => 'mobile'),
      (object)array('zhs' => '创建时间', 'zht' => '創建時間', 'enus' => 'created time'),
      (object)array('zhs' => '状态', 'zht' => '狀態', 'enus' => 'status'),
      (object)array('zhs' => '角色', 'zht' => '角色', 'enus' => 'role'),
      (object)array('zhs' => '不修改时请留空', 'zht' => '不修改時請留空', 'enus' => 'please leave empty when not modify'),
      (object)array('zhs' => '启用', 'zht' => '啟用', 'enus' => 'enabled'),
      (object)array('zhs' => '禁用', 'zht' => '禁用', 'enus' => 'disabled'),
      (object)array('zhs' => '确定要删除此用户吗？', 'zht' => '確定要刪除此用戶嗎？', 'enus' => 'confirm to delete this user? '),
      (object)array('zhs' => '确定要删除勾选的用户吗？', 'zht' => '確定要刪除勾選的用戶嗎？', 'enus' => 'confirm to delete checked users?')
    );
    $arrDatas['cmsfield'] = array(
      (object)array('zhs' => '字段类别', 'zht' => '字段類別', 'enus' => 'field category'),
      (object)array('zhs' => '字段', 'zht' => '字段', 'enus' => 'field'),
      (object)array('zhs' => '字段结构树', 'zht' => '字段結構樹', 'enus' => 'field structure trees'),
      (object)array('zhs' => '添加用户基础字段', 'zht' => '添加用戶基礎字段', 'enus' => 'add user basic field'),
      (object)array('zhs' => '添加内容基础字段', 'zht' => '添加內容基礎字段', 'enus' => 'add content basic field')
    );
    $arrDatas['minifield'] = array(
      (object)array('zhs' => '表名称 %tablename 已存在，不能重复', 'zht' => '表名稱 %tablename 已存在，不能重複', 'enus' => 'the table name %tablename already exists，can not be repeated'),
      (object)array('zhs' => '所属字段类别标识 %cid 不存在', 'zht' => '所屬字段類別標識 %cid 不存在', 'enus' => 'the field category id %cid already does not exists'),
      (object)array('zhs' => '字段名称 %tablename 已存在，不能重复', 'zht' => '字段名稱 %tablename 已存在，不能重複', 'enus' => 'the field name %tablename already exists，can not be repeated'),
      (object)array('zhs' => '字段类型 %fieldtype 与字段大小 %fieldsize 不匹配', 'zht' => '字段類型 %fieldtype 與字段大小 %fieldsize 不匹配', 'enus' => 'the field type %fieldtype and field size %fieldsize does not match'),
      (object)array('zhs' => '当字段类型为 int、numeric 时，字段默认值必须为整数', 'zht' => '當字段類型為 int、numeric 時，字段默認值必須為整數', 'enus' => 'the field default value must be an integer if field type is int or numeric'),
      (object)array('zhs' => '当字段类型为 float 时，字段默认值必须为数值', 'zht' => '當字段類型為 float 時，字段默認值必須為數值', 'enus' => 'the field default value must be an floating point value if field type is float'),
      (object)array('zhs' => '没有添加字段数据', 'zht' => '沒有添加字段數據', 'enus' => 'no field data added'),
      (object)array('zhs' => '这个文件 %file 不能保存，因为它超过了 %maxsize，即允许上传的最大值', 'zht' => '這個文件 %file 不能保存，因為它超過了 %maxsize，即允許上傳的最大值', 'enus' => 'The file %file could not be saved, because it exceeds %maxsize, the maximum allowed size for uploads'),
      (object)array('zhs' => '这个文件 %file 不能保存，因为上传没有完成', 'zht' => '這個文件 %file 不能保存，因為上傳沒有完成', 'enus' => 'The file %file could not be saved, because the upload did not complete'),
      (object)array('zhs' => '这个文件 %file 不能保存，一个未知的错误发生', 'zht' => '這個文件 %file 不能保存，一個未知的錯誤發生', 'enus' => 'The file %file could not be saved. An unknown error has occurred'),
      (object)array('zhs' => '这个文件 %file 不能保存，因为目标 %destination 无效', 'zht' => '這個文件 %file 不能保存，因為目標 %destination 無效', 'enus' => 'The file %file could not be uploaded, because the destination %destination is invalid'),
      (object)array('zhs' => '文件 %file 上传失败. 不能移动被上传的文件', 'zht' => '文件 %file 上傳失敗. 不能移動被上傳的文件', 'enus' => 'file %file upload error. could not move uploaded file'),
      (object)array('zhs' => '文件名为空，请为文件命名', 'zht' => '文件名為空，請為文件命名', 'enus' => 'The file\'s name is empty. Please give a name to the file'),
      (object)array('zhs' => '文件名长度超过 %length 字符限制，请重命名文件后再试', 'zht' => '文件名長度超過 %length 字符限制，請重命名文件后再試', 'enus' => 'The file\'s name exceeds the %length characters limit. Please rename the file and try again'),
      (object)array('zhs' => '仅拥有如下扩展名的文件允许上传：%files-allowed', 'zht' => '僅擁有如下擴展名的文件允許上傳：%files-allowed', 'enus' => 'Only files with the following extensions are allowed: %files-allowed'),
      (object)array('zhs' => '这个文件是 %filesize 超过了文件最大大小 %maxsize', 'zht' => '這個文件是 %filesize 超過了文件最大大小 %maxsize', 'enus' => 'The file is %filesize exceeding the maximum file size of %maxsize'),
      (object)array('zhs' => '仅支持 JPEG, PNG and GIF 格式的图片', 'zht' => '僅支持 JPEG, PNG and GIF 格式的圖片', 'enus' => 'Only JPEG, PNG and GIF images are allowed'),
      (object)array('zhs' => '这个图片太大；最大尺度是 %dimensions 像素', 'zht' => '這個圖片太大；最大尺度是 %dimensions 像素', 'enus' => 'The image is too large; the maximum dimensions are %dimensions pixels'),
      (object)array('zhs' => '这个图片太小；最小尺度是 %dimensions 像素', 'zht' => '這個圖片太小；最小尺度是 %dimensions 像素', 'enus' => 'The image is too small; the minimum dimensions are %dimensions pixels')
    );
    $arrDatas['pager'] = array(
      (object)array('zhs' => '首页', 'zht' => '首頁', 'enus' => 'first'),
      (object)array('zhs' => '上一页', 'zht' => '上一頁', 'enus' => 'previous'),
      (object)array('zhs' => '下一页', 'zht' => '下一頁', 'enus' => 'next'),
      (object)array('zhs' => '末页', 'zht' => '末頁', 'enus' => 'last')
    );
    if($handle == true){
      foreach($arrDatas as $key => $Datas){
        foreach($Datas as $Data){
          if(!isset($translations[$key]) || !is_array($translations[$key])){ $translations[$key] = array(); }
          foreach(self::$Languages as $Language){
            if(isset($Data->$Language)){ $translations[$key][$Data->$Language] = $Data; }
          }
        }
      }
    }else{
      $translations = $arrDatas;
    }
    return $translations;
  }
}