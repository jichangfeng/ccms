<?php
/**
 * @author Changfeng Ji <jichf@qq.com>
 */
final class MiniFieldCommon{
  /**
   * Get all the menu path of specified modules.
   * @access public
   * @static
   * @param string $modules The collection of module name. Each element is a module name.
   * @return array The menu paths.
   */
  public static function MenuPaths($modules = array()){
    if(empty($modules)){
      return array();
    }
    $StaticName = __CLASS__ . '_' . __FUNCTION__ . '_' . implode('_', $modules);
    $paths = MiniFieldStatic::GetStatic($StaticName, false);
    if(isset($paths)){
      return $paths;
    }
    $paths = array();
    foreach($modules as $module){
      if(function_exists($module . '_menu')){
        $router_items = call_user_func($module . '_menu');
        $paths = array_merge($paths, array_keys($router_items));
      }
    }
    MiniFieldStatic::SetStatic($StaticName, $paths, false);
    return $paths;
  }
  /**
   * Determine whether the path can access by current user.
   * @access public
   * @static
   * @param string $path The path, for example node/5.
   * @return bool
   */
  public static function MenuAccessCheck($path = NULL){
    $router_item = menu_get_item($path);
    return $router_item['access'] ? true : false;
  }

  /**
   * mini of field tpl render
   * @access public
   * @static
   * @param type $filepath
   * @param type $variables
   * @return type 
   */
  public static function Render($filepath, $variables = array()){
    if(file_exists($filepath)){
      extract($variables, EXTR_SKIP);
      ob_start();
      include $filepath;
      $contents = ob_get_contents();
      ob_end_clean();
      return $contents;
    }else{
      //TODO:need error handle
      return t($filepath . ' 页面未找到！');
    }
  }

  /**
   * Returns HTML for a query pager.
   *
   * Menu callbacks that display paged query results should call theme('pager') to
   * retrieve a pager control so that users can view other results. Format a list
   * of nearby pages with additional query results.
   * @access public
   * @static
   * @param type $variables
   *   An associative array containing:
   *   - tags: An array of labels for the controls in the pager.
   *   - element: An optional integer to distinguish between multiple pagers on
   *     one page.
   *   - parameters: An associative array of query string parameters to append to
   *     the pager links.
   *   - quantity: The number of pages in the list.
   *
   * @ingroup themeable
   * @return string.
   */
  public static function HtmlPager($variables = array()){
    $variables_default = array(
      'tags' => array(ts('首页', 'ucfirst', 'pager'), ts('上一页', 'ucfirst', 'pager'), '', ts('下一页', 'ucfirst', 'pager'), ts('末页', 'ucfirst', 'pager')),
      'element' => 0,
      'parameters' => array(),
      'quantity' => 9
    );
    $variables = array_merge($variables_default, $variables);
    global $pager_page_array;
    if(!isset($pager_page_array[$variables['element']])){
      return '';
    }
    $pager = theme_pager($variables);
    $newpager = preg_replace('#<h2.*?/h2>#', '', $pager);/* #<h2.*?>.*?</h2># */
    return $newpager ? $newpager : $pager;
  }
  /**
   * Returns HTML for the "first page" link in a query pager.
   *
   * @param $variables
   *   An associative array containing:
   *   - text: The name (or image) of the link.
   *   - element: An optional integer to distinguish between multiple pagers on
   *     one page.
   *   - parameters: An associative array of query string parameters to append to
   *     the pager links.
   *
   * @ingroup themeable
   * @return string.
   */
  public static function HtmlPager_First($variables = array()){
    $variables_default = array(
      'text' => ts('首页', 'ucfirst', 'pager'),
      'element' => 0,
      'parameters' => array()
    );
    $variables = array_merge($variables_default, $variables);
    global $pager_page_array;
    if(!isset($pager_page_array[$variables['element']])){
      return '';
    }
    return theme_pager_first($variables);
  }
  /**
   * Returns HTML for the "last page" link in query pager.
   *
   * @param $variables
   *   An associative array containing:
   *   - text: The name (or image) of the link.
   *   - element: An optional integer to distinguish between multiple pagers on
   *     one page.
   *   - parameters: An associative array of query string parameters to append to
   *     the pager links.
   *
   * @ingroup themeable
   * @return string.
   */
  public static function HtmlPager_Last($variables = array()){
    $variables_default = array(
      'text' => ts('末页', 'ucfirst', 'pager'),
      'element' => 0,
      'parameters' => array()
    );
    $variables = array_merge($variables_default, $variables);
    global $pager_page_array;
    if(!isset($pager_page_array[$variables['element']])){
      return '';
    }
    return theme_pager_last($variables);
  }
  /**
   * Returns HTML for the "previous page" link in a query pager.
   *
   * @param $variables
   *   An associative array containing:
   *   - text: The name (or image) of the link.
   *   - element: An optional integer to distinguish between multiple pagers on
   *     one page.
   *   - interval: The number of pages to move backward when the link is clicked.
   *   - parameters: An associative array of query string parameters to append to
   *     the pager links.
   *
   * @ingroup themeable
   * @return string.
   */
  public static function HtmlPager_Previous($variables = array()){
    $variables_default = array(
      'text' => ts('上一页', 'ucfirst', 'pager'),
      'element' => 0,
      'interval' => 1,
      'parameters' => array()
    );
    $variables = array_merge($variables_default, $variables);
    global $pager_page_array;
    if(!isset($pager_page_array[$variables['element']])){
      return '';
    }
    return theme_pager_previous($variables);
  }
  /**
   * Returns HTML for the "next page" link in a query pager.
   *
   * @param $variables
   *   An associative array containing:
   *   - text: The name (or image) of the link.
   *   - element: An optional integer to distinguish between multiple pagers on
   *     one page.
   *   - interval: The number of pages to move backward when the link is clicked.
   *   - parameters: An associative array of query string parameters to append to
   *     the pager links.
   *
   * @ingroup themeable
   * @return string.
   */
  public static function HtmlPager_Next($variables = array()){
    $variables_default = array(
      'text' => ts('下一页', 'ucfirst', 'pager'),
      'element' => 0,
      'interval' => 1,
      'parameters' => array()
    );
    $variables = array_merge($variables_default, $variables);
    global $pager_page_array;
    if(!isset($pager_page_array[$variables['element']])){
      return '';
    }
    return theme_pager_next($variables);
  }

  /**
   * split the string to array
   * @access public
   * @static
   * @param string $subject The splitted string.
   * @param bool $removespaces Whether remove the blank value. default: true.
   * @param bool $mergerepeat Whether merge the repeat value. default: true.
   * @return array 
   */
  public static function SplittedByNewline($subject, $removeblank = true, $mergerepeat = true){
    $pattern = '/\r\n|\r|\n/';
    $arrValues = preg_split($pattern, $subject);
    if($removeblank == true){
      foreach($arrValues as $key=>$value){
        if(strlen(trim($value)) == 0){ unset($arrValues[$key]); }
      }
    }
    if($mergerepeat == true){ $arrValues = array_unique($arrValues); }
    $arrValues = array_values($arrValues);
    return $arrValues;
  }
  /**
   * join the array to string
   * @access public
   * @static
   * @param array $piecesarray The joined array.
   * @return string 
   */
  public static function JoinedByNewline($piecesarray){
    return implode(PHP_EOL, $piecesarray);
  }

  /**
   * Get ip
   * @access public
   * @static
   * @return string 
   */
  public static function GetIp(){
    $realip = '';
    $unknown = 'unknown';
    if (isset($_SERVER)){
      if(isset($_SERVER['HTTP_X_FORWARDED_FOR']) && !empty($_SERVER['HTTP_X_FORWARDED_FOR']) && strcasecmp($_SERVER['HTTP_X_FORWARDED_FOR'], $unknown)){
        $arr = explode(',', $_SERVER['HTTP_X_FORWARDED_FOR']);
        foreach($arr as $ip){
          $ip = trim($ip);
          if ($ip != 'unknown'){
            $realip = $ip;
            break;
          }
        }
      }else if(isset($_SERVER['HTTP_CLIENT_IP']) && !empty($_SERVER['HTTP_CLIENT_IP']) && strcasecmp($_SERVER['HTTP_CLIENT_IP'], $unknown)){
        $realip = $_SERVER['HTTP_CLIENT_IP'];
      }else if(isset($_SERVER['REMOTE_ADDR']) && !empty($_SERVER['REMOTE_ADDR']) && strcasecmp($_SERVER['REMOTE_ADDR'], $unknown)){
        $realip = $_SERVER['REMOTE_ADDR'];
      }else{
        $realip = $unknown;
      }
    }else{
      if(getenv('HTTP_X_FORWARDED_FOR') && strcasecmp(getenv('HTTP_X_FORWARDED_FOR'), $unknown)){
        $realip = getenv("HTTP_X_FORWARDED_FOR");
      }else if(getenv('HTTP_CLIENT_IP') && strcasecmp(getenv('HTTP_CLIENT_IP'), $unknown)){
        $realip = getenv("HTTP_CLIENT_IP");
      }else if(getenv('REMOTE_ADDR') && strcasecmp(getenv('REMOTE_ADDR'), $unknown)){
        $realip = getenv("REMOTE_ADDR");
      }else{
        $realip = $unknown;
      }
    }
    $realip = preg_match("/[\d\.]{7,15}/", $realip, $matches) ? $matches[0] : $unknown;
    return $realip;
  }
  /**
   * Get ip lookup
   * @access public
   * @static
   * @param string $ip
   * @return
   *   A ip lookup array upon successful ip load, or False if the ip lookup cannot be loaded.
   *   The ip lookup array keys include start, end, country, province, city, district, isp, type, desc, ip.
   */
  public static function GetIpLookup($ip = ''){
    if(empty($ip)){
      $ip = self::GetIp();
    }
    $res = @file_get_contents('http://int.dpool.sina.com.cn/iplookup/iplookup.php?format=js&ip=' . $ip);
    if(empty($res)){ return false; }
    $jsonMatches = array();
    preg_match('#\{.+?\}#', $res, $jsonMatches);
    if(!isset($jsonMatches[0])){ return false; }
    $json = json_decode($jsonMatches[0], true);
    if(isset($json['ret']) && $json['ret'] == 1){
      $json['ip'] = $ip;
      unset($json['ret']);
    }else{
      return false;
    }
    return $json;
  }

  /**
   * Get pinyins array from a string with chinese.
   * @access public
   * @static
   * @param string $string a string with chinese.
   * @param bool $onlyzh whether only chinese. default: true.
   * @return array 
   */
  public static function Pinyins($string, $onlyzh = true){
    $Pinyins = array(
      array("a", -20319), array("ai", -20317), array("an", -20304), array("ang", -20295), array("ao", -20292),
      array("ba", -20283), array("bai", -20265), array("ban", -20257), array("bang", -20242), array("bao", -20230),
      array("bei", -20051), array("ben", -20036), array("beng", -20032), array("bi", -20026), array("bian", -20002),
      array("biao", -19990), array("bie", -19986), array("bin", -19982), array("bing", -19976), array("bo", -19805),
      array("bu", -19784),
      array("ca", -19775), array("cai", -19774), array("can", -19763), array("cang", -19756), array("cao", -19751),
      array("ce", -19746), array("ceng", -19741), array("cha", -19739), array("chai", -19728), array("chan", -19725),
      array("chang", -19715), array("chao", -19540), array("che", -19531), array("chen", -19525), array("cheng", -19515),
      array("chi", -19500), array("chong", -19484), array("chou", -19479), array("chu", -19467), array("chuai", -19289),
      array("chuan", -19288), array("chuang", -19281), array("chui", -19275), array("chun", -19270), array("chuo", -19263),
      array("ci", -19261), array("cong", -19249), array("cou", -19243), array("cu", -19242), array("cuan", -19238),
      array("cui", -19235), array("cun", -19227), array("cuo", -19224),
      array("da", -19218), array("dai", -19212), array("dan", -19038), array("dang", -19023), array("dao", -19018),
      array("de", -19006), array("deng", -19003), array("di", -18996), array("dian", -18977), array("diao", -18961),
      array("die", -18952), array("ding", -18783), array("diu", -18774), array("dong", -18773), array("dou", -18763),
      array("du", -18756), array("duan", -18741), array("dui", -18735), array("dun", -18731), array("duo", -18722),
      array("e", -18710), array("en", -18697), array("er", -18696),
      array("fa", -18526), array("fan", -18518), array("fang", -18501), array("fei", -18490), array("fen", -18478),
      array("feng", -18463), array("fo", -18448), array("fou", -18447), array("fu", -18446),
      array("ga", -18239), array("gai", -18237), array("gan", -18231), array("gang", -18220), array("gao", -18211),
      array("ge", -18201), array("gei", -18184), array("gen", -18183), array("geng", -18181), array("gong", -18012),
      array("gou", -17997), array("gu", -17988), array("gua", -17970), array("guai", -17964), array("guan", -17961),
      array("guang", -17950), array("gui", -17947), array("gun", -17931), array("guo", -17928),
      array("ha", -17922), array("hai", -17759), array("han", -17752), array("hang", -17733), array("hao", -17730),
      array("he", -17721), array("hei", -17703), array("hen", -17701), array("heng", -17697), array("hong", -17692),
      array("hou", -17683), array("hu", -17676), array("hua", -17496), array("huai", -17487), array("huan", -17482),
      array("huang", -17468), array("hui", -17454), array("hun", -17433), array("huo", -17427),
      array("ji", -17417), array("jia", -17202), array("jian", -17185), array("jiang", -16983), array("jiao", -16970),
      array("jie", -16942), array("jin", -16915), array("jing", -16733), array("jiong", -16708), array("jiu", -16706),
      array("ju", -16689), array("juan", -16664), array("jue", -16657), array("jun", -16647),
      array("ka", -16474), array("kai", -16470), array("kan", -16465), array("kang", -16459), array("kao", -16452),
      array("ke", -16448), array("ken", -16433), array("keng", -16429), array("kong", -16427), array("kou", -16423),
      array("ku", -16419), array("kua", -16412), array("kuai", -16407), array("kuan", -16403), array("kuang", -16401),
      array("kui", -16393), array("kun", -16220), array("kuo", -16216),
      array("la", -16212), array("lai", -16205), array("lan", -16202), array("lang", -16187), array("lao", -16180),
      array("le", -16171), array("lei", -16169), array("leng", -16158), array("li", -16155), array("lia", -15959),
      array("lian", -15958), array("liang", -15944), array("liao", -15933), array("lie", -15920), array("lin", -15915),
      array("ling", -15903), array("liu", -15889), array("long", -15878), array("lou", -15707), array("lu", -15701),
      array("lv", -15681), array("luan", -15667), array("lue", -15661), array("lun", -15659), array("luo", -15652),
      array("ma", -15640), array("mai", -15631), array("man", -15625), array("mang", -15454), array("mao", -15448),
      array("me", -15436), array("mei", -15435), array("men", -15419), array("meng", -15416), array("mi", -15408),
      array("mian", -15394), array("miao", -15385), array("mie", -15377), array("min", -15375), array("ming", -15369),
      array("miu", -15363), array("mo", -15362), array("mou", -15183), array("mu", -15180),
      array("na", -15165), array("nai", -15158), array("nan", -15153), array("nang", -15150), array("nao", -15149),
      array("ne", -15144), array("nei", -15143), array("nen", -15141), array("neng", -15140), array("ni", -15139),
      array("nian", -15128), array("niang", -15121), array("niao", -15119), array("nie", -15117), array("nin", -15110),
      array("ning", -15109), array("niu", -14941), array("nong", -14937), array("nu", -14933), array("nv", -14930),
      array("nuan", -14929), array("nue", -14928), array("nuo", -14926),
      array("o", -14922), array("ou", -14921),
      array("pa", -14914), array("pai", -14908), array("pan", -14902), array("pang", -14894), array("pao", -14889),
      array("pei", -14882), array("pen", -14873), array("peng", -14871), array("pi", -14857), array("pian", -14678),
      array("piao", -14674), array("pie", -14670), array("pin", -14668), array("ping", -14663), array("po", -14654),
      array("pu", -14645),
      array("qi", -14630), array("qia", -14594), array("qian", -14429), array("qiang", -14407), array("qiao", -14399),
      array("qie", -14384), array("qin", -14379), array("qing", -14368), array("qiong", -14355), array("qiu", -14353),
      array("qu", -14345), array("quan", -14170), array("que", -14159), array("qun", -14151),
      array("ran", -14149), array("rang", -14145), array("rao", -14140), array("re", -14137), array("ren", -14135),
      array("reng", -14125), array("ri", -14123), array("rong", -14122), array("rou", -14112), array("ru", -14109),
      array("ruan", -14099), array("rui", -14097), array("run", -14094), array("ruo", -14092),
      array("sa", -14090), array("sai", -14087), array("san", -14083), array("sang", -13917), array("sao", -13914),
      array("se", -13910), array("sen", -13907), array("seng", -13906), array("sha", -13905), array("shai", -13896),
      array("shan", -13894), array("shang", -13878), array("shao", -13870), array("she", -13859), array("shen", -13847),
      array("sheng", -13831), array("shi", -13658), array("shou", -13611), array("shu", -13601), array("shua", -13406),
      array("shuai", -13404), array("shuan", -13400), array("shuang", -13398), array("shui", -13395), array("shun", -13391),
      array("shuo", -13387), array("si", -13383), array("song", -13367), array("sou", -13359), array("su", -13356),
      array("suan", -13343), array("sui", -13340), array("sun", -13329), array("suo", -13326),
      array("ta", -13318), array("tai", -13147), array("tan", -13138), array("tang", -13120), array("tao", -13107),
      array("te", -13096), array("teng", -13095), array("ti", -13091), array("tian", -13076), array("tiao", -13068),
      array("tie", -13063), array("ting", -13060), array("tong", -12888), array("tou", -12875), array("tu", -12871),
      array("tuan", -12860), array("tui", -12858), array("tun", -12852), array("tuo", -12849),
      array("wa", -12838), array("wai", -12831), array("wan", -12829), array("wang", -12812), array("wei", -12802),
      array("wen", -12607), array("weng", -12597), array("wo", -12594), array("wu", -12585),
      array("xi", -12556), array("xia", -12359), array("xian", -12346), array("xiang", -12320), array("xiao", -12300),
      array("xie", -12120), array("xin", -12099), array("xing", -12089), array("xiong", -12074), array("xiu", -12067),
      array("xu", -12058), array("xuan", -12039), array("xue", -11867), array("xun", -11861),
      array("ya", -11847), array("yan", -11831), array("yang", -11798), array("yao", -11781), array("ye", -11604),
      array("yi", -11589), array("yin", -11536), array("ying", -11358), array("yo", -11340), array("yong", -11339),
      array("you", -11324), array("yu", -11303), array("yuan", -11097), array("yue", -11077), array("yun", -11067),
      array("za", -11055), array("zai", -11052), array("zan", -11045), array("zang", -11041), array("zao", -11038),
      array("ze", -11024), array("zei", -11020), array("zen", -11019), array("zeng", -11018), array("zha", -11014),
      array("zhai", -10838), array("zhan", -10832), array("zhang", -10815), array("zhao", -10800), array("zhe", -10790),
      array("zhen", -10780), array("zheng", -10764), array("zhi", -10587), array("zhong", -10544), array("zhou", -10533),
      array("zhu", -10519), array("zhua", -10331), array("zhuai", -10329), array("zhuan", -10328), array("zhuang", -10322),
      array("zhui", -10315), array("zhun", -10309), array("zhuo", -10307), array("zi", -10296), array("zong", -10281),
      array("zou", -10274), array("zu", -10270), array("zuan", -10262), array("zui", -10260), array("zun", -10256), array("zuo", -10254)
    );
    $results = array();
    for($i = 0; $i < mb_strlen($string); $i++){
      $subject = mb_substr($string, $i, 1);
      $pattern = '/^[\x{4e00}-\x{9fa5}]$/u';
      if(preg_match($pattern, $subject) > 0){
        $subject_gb2312 = iconv("UTF-8", "gb2312", $subject);
        $subject_utf_8 = iconv("gb2312","UTF-8", $subject_gb2312);
        if($subject_utf_8 == $subject){ $subject = $subject_gb2312; }
        $firstchar = ord($subject{0});
        if($firstchar >= ord("A") && $firstchar <= ord("z")){ $results[] = strtoupper($subject{0}); }
        $asc = ord($subject{0}) * 256 + ord($subject{1}) - 65536;
        for($j = count($Pinyins) - 1;$j >= 0;$j--){
          if($Pinyins[$j][1] <= $asc){
            $results[] = $Pinyins[$j][0];
            break;
          }
        }
      }else if(!$onlyzh){
        $results[] = $subject;
      }
    }
    return $results;
  }
  /**
   * Get pinyin first letters array from a string with chinese.
   * @access public
   * @static
   * @param string $string a string with chinese.
   * @param bool $onlyzh whether only chinese. default: true.
   * @return array
   */
  public static function PinyinFirstLetters($string, $onlyzh = true){
    $results = array();
    for($i = 0; $i < mb_strlen($string); $i++){
      $subject = mb_substr($string, $i, 1);
      $pattern = '/^[\x{4e00}-\x{9fa5}]$/u';
      if(preg_match($pattern, $subject) > 0){
        $subject_gb2312 = iconv("UTF-8", "gb2312", $subject);
        $subject_utf_8 = iconv("gb2312","UTF-8", $subject_gb2312);
        if($subject_utf_8 == $subject){ $subject = $subject_gb2312; }
        $firstchar = ord($subject{0});
        if($firstchar >= ord("A") && $firstchar <= ord("z")){ $results[] = strtoupper($subject{0}); }
        $asc = ord($subject{0}) * 256 + ord($subject{1}) - 65536;
        if($asc >= -20319 and $asc <= -20284){ $results[] = "A"; }
        else if($asc >= -20283 and $asc <= -19776){ $results[] = "B"; }
        else if($asc >= -19775 and $asc <= -19219){ $results[] = "C"; }
        else if($asc >= -19218 and $asc <= -18711){ $results[] = "D"; }
        else if($asc >= -18710 and $asc <= -18527){ $results[] = "E"; }
        else if($asc >= -18526 and $asc <= -18240){ $results[] = "F"; }
        else if($asc >= -18239 and $asc <= -17923){ $results[] = "G"; }
        else if($asc >= -17922 and $asc <= -17418){ $results[] = "I"; }
        else if($asc >= -17417 and $asc <= -16475){ $results[] = "J"; }
        else if($asc >= -16474 and $asc <= -16213){ $results[] = "K"; }
        else if($asc >= -16212 and $asc <= -15641){ $results[] = "L"; }
        else if($asc >= -15640 and $asc <= -15166){ $results[] = "M"; }
        else if($asc >= -15165 and $asc <= -14923){ $results[] = "N"; }
        else if($asc >= -14922 and $asc <= -14915){ $results[] = "O"; }
        else if($asc >= -14914 and $asc <= -14631){ $results[] = "P"; }
        else if($asc >= -14630 and $asc <= -14150){ $results[] = "Q"; }
        else if($asc >= -14149 and $asc <= -14091){ $results[] = "R"; }
        else if($asc >= -14090 and $asc <= -13319){ $results[] = "S"; }
        else if($asc >= -13318 and $asc <= -12839){ $results[] = "T"; }
        else if($asc >= -12838 and $asc <= -12557){ $results[] = "W"; }
        else if($asc >= -12556 and $asc <= -11848){ $results[] = "X"; }
        else if($asc >= -11847 and $asc <= -11056){ $results[] = "Y"; }
        else if($asc >= -11055 and $asc <= -10247){ $results[] = "Z"; }
        else{ $results[] = ""; }
      }else if(!$onlyzh){
        $results[] = $subject;
      }
    }
    return $results;
  }

  /**
   * 
   * @access public
   * @static
   * @param numeric $num
   * @param int $mode 0 = Simplified Chinese; 1 = Traditional Chinese
   * @return string
   */
  public static function NumberToChinese($num, $mode = 0){
    if(!in_array($mode, array(0, 1))){ $mode = 0; }
    switch($mode) {
      case 0:
        $CNum = array(
          array('零', '一', '二', '三', '四', '五', '六', '七', '八', '九'),
          array('', '十', '百', '千'),
          array('', '万', '亿', '万亿')
        );
        break;
      case 1:
        $CNum = array(
          array('零', '壹', '贰', '叁', '肆', '伍', '陆', '柒', '捌', '玖'),
          array('', '拾', '佰', '仟'),
          array('', '萬', '億', '萬億')
        );
        break;
    }
    if (is_integer($num)) {
      $int = (string)$num;
    } else if (is_numeric($num)) {
      $num = explode('.', (string)floatval($num));
      $int = $num[0];
      $fl  = isset($num[1]) ? $num[1] : FALSE;
    }
    $len = strlen($int);// 长度
    $chinese = array();// 中文

    $str = strrev($int);// 反转的数字
    for($i = 0; $i < $len; $i += 4 ) {
      $s = array(0 => $str[$i], 1 => $str[$i+1], 2 => $str[$i+2], 3 => $str[$i+3]);
      $j = '';
      /* 千位 */
      if ($s[3] !== '') {
        $s[3] = (int)$s[3];
        if ($s[3] !== 0) {
          $j .= $CNum[0][$s[3]] . $CNum[1][3];
        } else {
          if ($s[2] != 0 || $s[1] != 0 || $s[0] != 0) {
            $j .= $CNum[0][0];
          }
        }
      }
      /* 百位 */
      if ($s[2] !== '') {
        $s[2] = (int)$s[2];
        if ($s[2] !== 0) {
          $j .= $CNum[0][$s[2]] . $CNum[1][2];
        } else {
          if ($s[3] != 0 && ($s[1] != 0 || $s[0] != 0) ) {
            $j .= $CNum[0][0];
          }
        }
      }
      /* 十位 */
      if ($s[1] !== '') {
        $s[1] = (int)$s[1];
        if ($s[1] !== 0) {
          $j .= $CNum[0][$s[1]] . $CNum[1][1];
        } else {
          if ($s[0] != 0 && $s[2] != 0) {
            $j .= $CNum[0][$s[1]];
          }
        }
      }
      /* 个位 */
      if ($s[0] !== '') {
        $s[0] = (int)$s[0];
        if ($s[0] !== 0) {
          $j .= $CNum[0][$s[0]] . $CNum[1][0];
        } else {
          // do nothing
        }
      }
      $j .= $CNum[2][$i/4];
      array_unshift($chinese, $j);
    }
    $chs = implode('', $chinese);
    if ($fl) {
      $chs .= '点';
      for($i = 0,$j = strlen($fl); $i < $j; $i++) {
        $t = (int)$fl[$i];
        $chs .= $CNum[0][$t];
      }
    }
    return $chs;
  }

  /**
   * Calculate time span
   * @access public
   * @static
   * @param int $time1 timestamp
   * @param int $time2 timestamp default to time() if null.
   * @param string $output default to 'years,months,weeks,days,hours,minutes,seconds'
   * @return array/boolean a array of time span, or false if error.
   */
  public static function TimeSpan($time1, $time2 = null, $output = 'years,months,weeks,days,hours,minutes,seconds'){
    // Array with the output formats
    $output = preg_split('/[^a-z]+/', strtolower((string) $output));
    if(empty($output)){
      return false;
    }
    // Make the output values into keys
    extract(array_flip($output), EXTR_SKIP);
    $time1 = max(0, (int)$time1);
    $time2 = empty($time2) ? time() : max(0, (int)$time2);
    // Calculate timespan (seconds)
    $timespan = abs($time1 - $time2);
    // Years and months do not match the formula exactly, due to leap years.
    isset($years) and $timespan -= (60 * 60 * 24 * 365) * ($years = (int)floor($timespan / (60 * 60 * 24 * 365)));
    isset($months) and $timespan -= (60 * 60 * 24 * 30) * ($months = (int)floor($timespan / (60 * 60 * 24 * 30)));
    isset($weeks) and $timespan -= (60 * 60 * 24 * 7) * ($weeks = (int)floor($timespan / (60 * 60 * 24 * 7)));
    isset($days) and $timespan -= (60 * 60 * 24) * ($days = (int)floor($timespan / (60 * 60 * 24)));
    isset($hours) and $timespan -= (60 * 60) * ($hours = (int)floor($timespan / (60 * 60)));
    isset($minutes) and $timespan -= 60 * ($minutes = (int)floor($timespan / 60));
    isset($seconds) and $seconds = $timespan;
    // Remove the variables that cannot be accessed
    unset($time1, $time2, $timespan);
    // Return the difference
    $timespan = array();
    foreach($output as $key){
      if (isset($$key)){
        //Add requested key to the output
        $timespan[$key] = $$key;
      }
    }
    // Invalid output formats string
    if (empty($timespan)){
      return false;
    }
    return $timespan;
  }
  public static function TimeSpanTest(){
    $timestamp = time() - rand(0, 300000);
    $timespan = self::TimeSpan($timestamp, null, 'days,hours,minutes,seconds');
    if($timespan){
      if(isset($timespan['days']) && $timespan['days'] == 1){
        $arrs[] = '昨天';
      }else if(isset($timespan['days']) && $timespan['days'] > 0){
        $arrs[] = $timespan['days'] . '天';
      }
      if(isset($timespan['hours']) && ($timespan['hours'] > 0 || count($arrs) > 0)){
        $arrs[] = $timespan['hours'] . '时';
      }
      if(isset($timespan['minutes']) && ($timespan['minutes'] > 0 || count($arrs) > 0)){
        $arrs[] = $timespan['minutes'] . '分';
      }
      $arrs[] = $timespan['seconds'] . '秒';
    }
    return implode('', $arrs);
  }

  /**
   * Convert real value to placeholder value.
   * @example $key = 'userid', $realValue = '1', $placeholderValue = '{userid:1}'.
   * @example $key = 'username', $realValue = 'coco', $placeholderValue = '{username:coco}'.
   * @access public
   * @static
   * @param string $key The placeholder key. Such as roleid, userid, username, navid, nodeid, etc.
   * @param string $realValue The real value.
   * @return string The placeholder value or empty string if error.
   */
  public static function PlaceholderConvert($key, $realValue){
    return '{' . $key . ':' . $realValue . '}';
  }
  /**
   * Convert placeholder value to real value.
   * @example $key = 'userid', $realValue = '1', $placeholderValue = '{userid:1}'.
   * @example $key = 'username', $realValue = 'coco', $placeholderValue = '{username:coco}'.
   * @access public
   * @static
   * @param string $key The placeholder key. Such as roleid, userid, username, navid, nodeid, etc.
   * @param string $placeholderValue The placeholder value.
   * @return string The real value or empty string if error.
   */
  public static function PlaceholderInvert($key, $placeholderValue){
    preg_match('/\{' . $key . ':(.+?)\}/', $placeholderValue, $matches);
    return isset($matches[1]) ? $matches[1] : '';
  }
  /**
   * Get placeholder values information.
   * @example $key = 'userid', $realValue = '1', $placeholderValue = '{userid:1}'.
   * @example $key = 'username', $realValue = 'coco', $placeholderValue = '{username:coco}'.
   * @access public
   * @static
   * @param string $string The string.
   * @param array $keys The collection of placeholder keys. Each element is a placeholder key.
   * @return array Each element is still a array with three elements($key, $realValue, $placeholderValue):
   *  - $key: The placeholder key. Such as roleid, userid, username, navid, nodeid, etc.
   *  - $realValue: The real value.
   *  - $placeholderValue: The placeholder value.
   */
  public static function PlaceholderGet($string, $keys = array('roleid', 'userid', 'username', 'navid', 'nodeid')){
    if(empty($string)){
      return array();
    }
    $arrs = array();
    foreach($keys as $key){
      preg_match_all('/\{' . $key . ':(.+?)\}/', $string, $matches);
      $i = 0;
      while(isset($matches[0][$i]) && isset($matches[1][$i])){
        $arrs[] = array($key, $matches[1][$i], $matches[0][$i]);
        $i++;
      }
    }
    return $arrs;
  }
  public static function PlaceholderTest(){
    $string = 'Test one: ' . self::PlaceholderConvert('userid', 1) . chr(10);
    $string .= 'Test two: ' . self::PlaceholderConvert('username', 'admin') . chr(10);
    $string .= 'Test three: ' . self::PlaceholderConvert('userid', 2) . chr(10);
    $string .= 'Test four: ' . self::PlaceholderConvert('username', 'coco') . chr(10);
    $string .= 'Test five: ' . self::PlaceholderConvert('username', 'keke');
    $arrs = array();
    $arrs[0] = $string;
    $arrs[1] = self::PlaceholderInvert('userid', $string);
    $arrs[2] = self::PlaceholderInvert('username', $string);
    $arrs[3] = self::PlaceholderGet($string);
    foreach($arrs[3] as $arr){
      $string = str_replace($arr[2], $arr[0] . ' "' . $arr[1] . '"', $string);
    }
    $arrs[4] = $string;
    return $arrs;
  }
}