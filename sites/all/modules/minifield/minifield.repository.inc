<?php
/**
 * @author Changfeng Ji <jichf@qq.com>
 */
final class MiniFieldRepository{
  /**
   * Table storage engines
   * @access public
   * @static
   * @param string $separator Defaults to an string '\0'.
   * @param string $group Defaults to an string ''.
   * @return array Each element is still a array with one to four elements($value[, $name, $title, $selected])
   */
  public static function GroupTags($separator = '\0', $group = ''){
    $arrValueArrays = array();
    $arrValueArrays[] = array('core', '核心', '核心');
    $arrValueArrays[] = array('user', '用户', '用户');
    $arrValueArrays[] = array('content', '栏目内容', '栏目内容');
    $arrValueArrays[] = array('', '无', '', 1);
    return $arrValueArrays;
  }
  /**
   * Table storage engines
   * @access public
   * @static
   * @param string $separator Defaults to an string '\0'.
   * @param string $group Defaults to an string ''.
   * @return array Each element is still a array with one to four elements($value[, $name, $title, $selected])
   */
  public static function TableTypes($separator = '\0', $group = ''){
    $arrValueArrays = array();
    $arrValueArrays[] = array('MRG_MYISAM', 'MRG_MYISAM', 'Collection of identical MyISAM tables');
    $arrValueArrays[] = array('MyISAM', 'MyISAM', 'MyISAM storage engine', 1);
    $arrValueArrays[] = array('BLACKHOLE', 'BLACKHOLE', '/dev/null storage engine (anything you write to it disappears)');
    $arrValueArrays[] = array('CSV', 'CSV', 'CSV storage engine');
    $arrValueArrays[] = array('MEMORY', 'MEMORY', 'Hash based, stored in memory, useful for temporary tables');
    $arrValueArrays[] = array('ARCHIVE', 'ARCHIVE', 'Archive storage engine');
    $arrValueArrays[] = array('InnoDB', 'InnoDB', 'Supports transactions, row-level locking, and foreign keys');
    $arrValueArrays[] = array('PERFORMANCE_SCHEMA', 'PERFORMANCE_SCHEMA', 'Performance Schema');
    return $arrValueArrays;
  }

  /**
   * Table storage engines
   * @access public
   * @static
   * @param string $separator Defaults to an string '\0'.
   * @param string $group Defaults to an string ''.
   * @return array Each element is still a array with one to four elements($value[, $name, $title, $selected])
   */
  public static function TableCollations($separator = '\0', $group = ''){
    $arrValueArrays = array();
    $arrValueArrays[] = array('', '', '');
    $arrValueArrays[] = array($group.'_begin', 'armscii8', 'ARMSCII-8 Armenian');
    $arrValueArrays[] = array('armscii8_bin', 'armscii8_bin', '亚美尼亚语, 二进制');
    $arrValueArrays[] = array('armscii8_general_ci', 'armscii8_general_ci', '亚美尼亚语, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'armscii8', 'ARMSCII-8 Armenian');
    $arrValueArrays[] = array($group.'_begin', 'ascii', 'US ASCII');
    $arrValueArrays[] = array('ascii_bin', 'ascii_bin', '西欧 (多语言), 二进制');
    $arrValueArrays[] = array('ascii_general_ci', 'ascii_general_ci', '西欧 (多语言), 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'ascii', 'US ASCII');
    $arrValueArrays[] = array($group.'_begin', 'big5', 'Big5 Traditional Chinese');
    $arrValueArrays[] = array('big5_bin', 'big5_bin', '正体中文, 二进制');
    $arrValueArrays[] = array('big5_chinese_ci', 'big5_chinese_ci', '正体中文, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'big5', 'Big5 Traditional Chinese');
    $arrValueArrays[] = array($group.'_begin', 'binary', 'Binary pseudo charset');
    $arrValueArrays[] = array('binary', 'binary', '二进制');
    $arrValueArrays[] = array($group.'_end', 'binary', 'Binary pseudo charset');
    $arrValueArrays[] = array($group.'_begin', 'cp1250', 'Windows Central European');
    $arrValueArrays[] = array('cp1250_bin', 'cp1250_bin', '中欧 (多语言), 二进制');
    $arrValueArrays[] = array('cp1250_croatian_ci', 'cp1250_croatian_ci', '克罗地亚语, 不区分大小写');
    $arrValueArrays[] = array('cp1250_czech_cs', 'cp1250_czech_cs', '捷克语, 区分大小写');
    $arrValueArrays[] = array('cp1250_general_ci', 'cp1250_general_ci', '中欧 (多语言), 不区分大小写');
    $arrValueArrays[] = array('cp1250_polish_ci', 'cp1250_polish_ci', '波兰语, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'cp1250', 'Windows Central European');
    $arrValueArrays[] = array($group.'_begin', 'cp1251', 'Windows Cyrillic');
    $arrValueArrays[] = array('cp1251_bin', 'cp1251_bin', '西里尔语 (多语言), 二进制');
    $arrValueArrays[] = array('cp1251_bulgarian_ci', 'cp1251_bulgarian_ci', '保加利亚语, 不区分大小写');
    $arrValueArrays[] = array('cp1251_general_ci', 'cp1251_general_ci', '西里尔语 (多语言), 不区分大小写');
    $arrValueArrays[] = array('cp1251_general_cs', 'cp1251_general_cs', '西里尔语 (多语言), 区分大小写');
    $arrValueArrays[] = array('cp1251_ukrainian_ci', 'cp1251_ukrainian_ci', '乌克兰语, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'cp1251', 'Windows Cyrillic');
    $arrValueArrays[] = array($group.'_begin', 'cp1256', 'Windows Arabic');
    $arrValueArrays[] = array('cp1256_bin', 'cp1256_bin', '阿拉伯语, 二进制');
    $arrValueArrays[] = array('cp1256_general_ci', 'cp1256_general_ci', '阿拉伯语, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'cp1256', 'Windows Arabic');
    $arrValueArrays[] = array($group.'_begin', 'cp1257', 'Windows Baltic');
    $arrValueArrays[] = array('cp1257_bin', 'cp1257_bin', '巴拉克语 (多语言), 二进制');
    $arrValueArrays[] = array('cp1257_general_ci', 'cp1257_general_ci', '巴拉克语 (多语言), 不区分大小写');
    $arrValueArrays[] = array('cp1257_lithuanian_ci', 'cp1257_lithuanian_ci', '立陶宛语, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'cp1257', 'Windows Baltic');
    $arrValueArrays[] = array($group.'_begin', 'cp850', 'DOS West European');
    $arrValueArrays[] = array('cp850_bin', 'cp850_bin', '西欧 (多语言), 二进制');
    $arrValueArrays[] = array('cp850_general_ci', 'cp850_general_ci', '西欧 (多语言), 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'cp850', 'DOS West European');
    $arrValueArrays[] = array($group.'_begin', 'cp852', 'DOS Central European');
    $arrValueArrays[] = array('cp852_bin', 'cp852_bin', '中欧 (多语言), 二进制');
    $arrValueArrays[] = array('cp852_general_ci', 'cp852_general_ci', '中欧 (多语言), 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'cp852', 'DOS Central European');
    $arrValueArrays[] = array($group.'_begin', 'cp866', 'DOS Russian');
    $arrValueArrays[] = array('cp866_bin', 'cp866_bin', '俄语, 二进制');
    $arrValueArrays[] = array('cp866_general_ci', 'cp866_general_ci', '俄语, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'cp866', 'DOS Russian');
    $arrValueArrays[] = array($group.'_begin', 'cp932', 'SJIS for Windows Japanese');
    $arrValueArrays[] = array('cp932_bin', 'cp932_bin', '日语, 二进制');
    $arrValueArrays[] = array('cp932_japanese_ci', 'cp932_japanese_ci', '日语, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'cp932', 'SJIS for Windows Japanese');
    $arrValueArrays[] = array($group.'_begin', 'dec8', 'DEC West European');
    $arrValueArrays[] = array('dec8_bin', 'dec8_bin', '西欧 (多语言), 二进制');
    $arrValueArrays[] = array('dec8_swedish_ci', 'dec8_swedish_ci', '瑞典语, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'dec8', 'DEC West European');
    $arrValueArrays[] = array($group.'_begin', 'eucjpms', 'UJIS for Windows Japanese');
    $arrValueArrays[] = array('eucjpms_bin', 'eucjpms_bin', '日语, 二进制');
    $arrValueArrays[] = array('eucjpms_japanese_ci', 'eucjpms_japanese_ci', '日语, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'eucjpms', 'UJIS for Windows Japanese');
    $arrValueArrays[] = array($group.'_begin', 'euckr', 'EUC-KR Korean');
    $arrValueArrays[] = array('euckr_bin', 'euckr_bin', '朝鲜语, 二进制');
    $arrValueArrays[] = array('euckr_korean_ci', 'euckr_korean_ci', '朝鲜语, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'euckr', 'EUC-KR Korean');
    $arrValueArrays[] = array($group.'_begin', 'gb2312', 'GB2312 Simplified Chinese');
    $arrValueArrays[] = array('gb2312_bin', 'gb2312_bin', '简体中文, 二进制');
    $arrValueArrays[] = array('gb2312_chinese_ci', 'gb2312_chinese_ci', '简体中文, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'gb2312', 'GB2312 Simplified Chinese');
    $arrValueArrays[] = array($group.'_begin', 'gbk', 'GBK Simplified Chinese');
    $arrValueArrays[] = array('gbk_bin', 'gbk_bin', '简体中文, 二进制');
    $arrValueArrays[] = array('gbk_chinese_ci', 'gbk_chinese_ci', '简体中文, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'gbk', 'GBK Simplified Chinese');
    $arrValueArrays[] = array($group.'_begin', 'geostd8', 'GEOSTD8 Georgian');
    $arrValueArrays[] = array('geostd8_bin', 'geostd8_bin', '乔治亚语, 二进制');
    $arrValueArrays[] = array('geostd8_general_ci', 'geostd8_general_ci', '乔治亚语, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'geostd8', 'GEOSTD8 Georgian');
    $arrValueArrays[] = array($group.'_begin', 'greek', 'ISO 8859-7 Greek');
    $arrValueArrays[] = array('greek_bin', 'greek_bin', '希腊语, 二进制');
    $arrValueArrays[] = array('greek_general_ci', 'greek_general_ci', '希腊语, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'greek', 'ISO 8859-7 Greek');
    $arrValueArrays[] = array($group.'_begin', 'hebrew', 'ISO 8859-8 Hebrew');
    $arrValueArrays[] = array('hebrew_bin', 'hebrew_bin', '希伯来语, 二进制');
    $arrValueArrays[] = array('hebrew_general_ci', 'hebrew_general_ci', '希伯来语, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'hebrew', 'ISO 8859-8 Hebrew');
    $arrValueArrays[] = array($group.'_begin', 'hp8', 'HP West European');
    $arrValueArrays[] = array('hp8_bin', 'hp8_bin', '西欧 (多语言), 二进制');
    $arrValueArrays[] = array('hp8_english_ci', 'hp8_english_ci', '英语, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'hp8', 'HP West European');
    $arrValueArrays[] = array($group.'_begin', 'keybcs2', 'DOS Kamenicky Czech-Slovak');
    $arrValueArrays[] = array('keybcs2_bin', 'keybcs2_bin', '捷克斯洛伐克语, 二进制');
    $arrValueArrays[] = array('keybcs2_general_ci', 'keybcs2_general_ci', '捷克斯洛伐克语, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'keybcs2', 'DOS Kamenicky Czech-Slovak');
    $arrValueArrays[] = array($group.'_begin', 'koi8r', 'KOI8-R Relcom Russian');
    $arrValueArrays[] = array('koi8r_bin', 'koi8r_bin', '俄语, 二进制');
    $arrValueArrays[] = array('koi8r_general_ci', 'koi8r_general_ci', '俄语, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'koi8r', 'KOI8-R Relcom Russian');
    $arrValueArrays[] = array($group.'_begin', 'koi8u', 'KOI8-U Ukrainian');
    $arrValueArrays[] = array('koi8u_bin', 'koi8u_bin', '乌克兰语, 二进制');
    $arrValueArrays[] = array('koi8u_general_ci', 'koi8u_general_ci', '乌克兰语, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'koi8u', 'KOI8-U Ukrainian');
    $arrValueArrays[] = array($group.'_begin', 'latin1', 'cp1252 West European');
    $arrValueArrays[] = array('latin1_bin', 'latin1_bin', '西欧 (多语言), 二进制');
    $arrValueArrays[] = array('latin1_danish_ci', 'latin1_danish_ci', '丹麦语, 不区分大小写');
    $arrValueArrays[] = array('latin1_general_ci', 'latin1_general_ci', '西欧 (多语言), 不区分大小写');
    $arrValueArrays[] = array('latin1_general_cs', 'latin1_general_cs', '西欧 (多语言), 区分大小写');
    $arrValueArrays[] = array('latin1_german1_ci', 'latin1_german1_ci', '德语 (字典), 不区分大小写');
    $arrValueArrays[] = array('latin1_german2_ci', 'latin1_german2_ci', '德语 (电话本), 不区分大小写');
    $arrValueArrays[] = array('latin1_spanish_ci', 'latin1_spanish_ci', '西班牙语, 不区分大小写');
    $arrValueArrays[] = array('latin1_swedish_ci', 'latin1_swedish_ci', '瑞典语, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'latin1', 'cp1252 West European');
    $arrValueArrays[] = array($group.'_begin', 'latin2', 'ISO 8859-2 Central European');
    $arrValueArrays[] = array('latin2_bin', 'latin2_bin', '中欧 (多语言), 二进制');
    $arrValueArrays[] = array('latin2_croatian_ci', 'latin2_croatian_ci', '克罗地亚语, 不区分大小写');
    $arrValueArrays[] = array('latin2_czech_cs', 'latin2_czech_cs', '捷克语, 区分大小写');
    $arrValueArrays[] = array('latin2_general_ci', 'latin2_general_ci', '中欧 (多语言), 不区分大小写');
    $arrValueArrays[] = array('latin2_hungarian_ci', 'latin2_hungarian_ci', '匈牙利语, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'latin2', 'ISO 8859-2 Central European');
    $arrValueArrays[] = array($group.'_begin', 'latin5', 'ISO 8859-9 Turkish');
    $arrValueArrays[] = array('latin5_bin', 'latin5_bin', '土耳其语, 二进制');
    $arrValueArrays[] = array('latin5_turkish_ci', 'latin5_turkish_ci', '土耳其语, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'latin5', 'ISO 8859-9 Turkish');
    $arrValueArrays[] = array($group.'_begin', 'latin7', 'ISO 8859-13 Baltic');
    $arrValueArrays[] = array('latin7_bin', 'latin7_bin', '巴拉克语 (多语言), 二进制');
    $arrValueArrays[] = array('latin7_estonian_cs', 'latin7_estonian_cs', '爱沙尼亚语, 区分大小写');
    $arrValueArrays[] = array('latin7_general_ci', 'latin7_general_ci', '巴拉克语 (多语言), 不区分大小写');
    $arrValueArrays[] = array('latin7_general_cs', 'latin7_general_cs', '巴拉克语 (多语言), 区分大小写');
    $arrValueArrays[] = array($group.'_end', 'latin7', 'ISO 8859-13 Baltic');
    $arrValueArrays[] = array($group.'_begin', 'macce', 'Mac Central European');
    $arrValueArrays[] = array('macce_bin', 'macce_bin', '中欧 (多语言), 二进制');
    $arrValueArrays[] = array('macce_general_ci', 'macce_general_ci', '中欧 (多语言), 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'macce', 'Mac Central European');
    $arrValueArrays[] = array($group.'_begin', 'macroman', 'Mac West European');
    $arrValueArrays[] = array('macroman_bin', 'macroman_bin', '西欧 (多语言), 二进制');
    $arrValueArrays[] = array('macroman_general_ci', 'macroman_general_ci', '西欧 (多语言), 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'macroman', 'Mac West European');
    $arrValueArrays[] = array($group.'_begin', 'sjis', 'Shift-JIS Japanese');
    $arrValueArrays[] = array('sjis_bin', 'sjis_bin', '日语, 二进制');
    $arrValueArrays[] = array('sjis_japanese_ci', 'sjis_japanese_ci', '日语, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'sjis', 'Shift-JIS Japanese');
    $arrValueArrays[] = array($group.'_begin', 'swe7', '7bit Swedish');
    $arrValueArrays[] = array('swe7_bin', 'swe7_bin', '瑞典语, 二进制');
    $arrValueArrays[] = array('swe7_swedish_ci', 'swe7_swedish_ci', '瑞典语, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'swe7', '7bit Swedish');
    $arrValueArrays[] = array($group.'_begin', 'tis620', 'TIS620 Thai');
    $arrValueArrays[] = array('tis620_bin', 'tis620_bin', '泰语, 二进制');
    $arrValueArrays[] = array('tis620_thai_ci', 'tis620_thai_ci', '泰语, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'tis620', 'TIS620 Thai');
    $arrValueArrays[] = array($group.'_begin', 'ucs2', 'UCS-2 Unicode');
    $arrValueArrays[] = array('ucs2_bin', 'ucs2_bin', 'Unicode (多语言), 二进制');
    $arrValueArrays[] = array('ucs2_czech_ci', 'ucs2_czech_ci', '捷克语, 不区分大小写');
    $arrValueArrays[] = array('ucs2_danish_ci', 'ucs2_danish_ci', '丹麦语, 不区分大小写');
    $arrValueArrays[] = array('ucs2_esperanto_ci', 'ucs2_esperanto_ci', '世界语, 不区分大小写');
    $arrValueArrays[] = array('ucs2_estonian_ci', 'ucs2_estonian_ci', '爱沙尼亚语, 不区分大小写');
    $arrValueArrays[] = array('ucs2_general_ci', 'ucs2_general_ci', 'Unicode (多语言), 不区分大小写');
    $arrValueArrays[] = array('ucs2_hungarian_ci', 'ucs2_hungarian_ci', '匈牙利语, 不区分大小写');
    $arrValueArrays[] = array('ucs2_icelandic_ci', 'ucs2_icelandic_ci', '冰岛语, 不区分大小写');
    $arrValueArrays[] = array('ucs2_latvian_ci', 'ucs2_latvian_ci', '拉脱维亚语, 不区分大小写');
    $arrValueArrays[] = array('ucs2_lithuanian_ci', 'ucs2_lithuanian_ci', '立陶宛语, 不区分大小写');
    $arrValueArrays[] = array('ucs2_persian_ci', 'ucs2_persian_ci', '波斯语, 不区分大小写');
    $arrValueArrays[] = array('ucs2_polish_ci', 'ucs2_polish_ci', '波兰语, 不区分大小写');
    $arrValueArrays[] = array('ucs2_roman_ci', 'ucs2_roman_ci', '西欧, 不区分大小写');
    $arrValueArrays[] = array('ucs2_romanian_ci', 'ucs2_romanian_ci', '罗马尼亚语, 不区分大小写');
    $arrValueArrays[] = array('ucs2_sinhala_ci', 'ucs2_sinhala_ci', '未知, 不区分大小写');
    $arrValueArrays[] = array('ucs2_slovak_ci', 'ucs2_slovak_ci', '斯洛伐克语, 不区分大小写');
    $arrValueArrays[] = array('ucs2_slovenian_ci', 'ucs2_slovenian_ci', '斯洛文尼亚语, 不区分大小写');
    $arrValueArrays[] = array('ucs2_spanish2_ci', 'ucs2_spanish2_ci', '传统西班牙语, 不区分大小写');
    $arrValueArrays[] = array('ucs2_spanish_ci', 'ucs2_spanish_ci', '西班牙语, 不区分大小写');
    $arrValueArrays[] = array('ucs2_swedish_ci', 'ucs2_swedish_ci', '瑞典语, 不区分大小写');
    $arrValueArrays[] = array('ucs2_turkish_ci', 'ucs2_turkish_ci', '土耳其语, 不区分大小写');
    $arrValueArrays[] = array('ucs2_unicode_ci', 'ucs2_unicode_ci', 'Unicode (多语言), 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'ucs2', 'UCS-2 Unicode');
    $arrValueArrays[] = array($group.'_begin', 'ujis', 'EUC-JP Japanese');
    $arrValueArrays[] = array('ujis_bin', 'ujis_bin', '日语, 二进制');
    $arrValueArrays[] = array('ujis_japanese_ci', 'ujis_japanese_ci', '日语, 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'ujis', 'EUC-JP Japanese');
    $arrValueArrays[] = array($group.'_begin', 'utf16', 'UTF-16 Unicode');
    $arrValueArrays[] = array('utf16_bin', 'utf16_bin', '未知, 二进制');
    $arrValueArrays[] = array('utf16_czech_ci', 'utf16_czech_ci', '捷克语, 不区分大小写');
    $arrValueArrays[] = array('utf16_danish_ci', 'utf16_danish_ci', '丹麦语, 不区分大小写');
    $arrValueArrays[] = array('utf16_esperanto_ci', 'utf16_esperanto_ci', '世界语, 不区分大小写');
    $arrValueArrays[] = array('utf16_estonian_ci', 'utf16_estonian_ci', '爱沙尼亚语, 不区分大小写');
    $arrValueArrays[] = array('utf16_general_ci', 'utf16_general_ci', '未知, 不区分大小写');
    $arrValueArrays[] = array('utf16_hungarian_ci', 'utf16_hungarian_ci', '匈牙利语, 不区分大小写');
    $arrValueArrays[] = array('utf16_icelandic_ci', 'utf16_icelandic_ci', '冰岛语, 不区分大小写');
    $arrValueArrays[] = array('utf16_latvian_ci', 'utf16_latvian_ci', '拉脱维亚语, 不区分大小写');
    $arrValueArrays[] = array('utf16_lithuanian_ci', 'utf16_lithuanian_ci', '立陶宛语, 不区分大小写');
    $arrValueArrays[] = array('utf16_persian_ci', 'utf16_persian_ci', '波斯语, 不区分大小写');
    $arrValueArrays[] = array('utf16_polish_ci', 'utf16_polish_ci', '波兰语, 不区分大小写');
    $arrValueArrays[] = array('utf16_roman_ci', 'utf16_roman_ci', '西欧, 不区分大小写');
    $arrValueArrays[] = array('utf16_romanian_ci', 'utf16_romanian_ci', '罗马尼亚语, 不区分大小写');
    $arrValueArrays[] = array('utf16_sinhala_ci', 'utf16_sinhala_ci', '未知, 不区分大小写');
    $arrValueArrays[] = array('utf16_slovak_ci', 'utf16_slovak_ci', '斯洛伐克语, 不区分大小写');
    $arrValueArrays[] = array('utf16_slovenian_ci', 'utf16_slovenian_ci', '斯洛文尼亚语, 不区分大小写');
    $arrValueArrays[] = array('utf16_spanish2_ci', 'utf16_spanish2_ci', '传统西班牙语, 不区分大小写');
    $arrValueArrays[] = array('utf16_spanish_ci', 'utf16_spanish_ci', '西班牙语, 不区分大小写');
    $arrValueArrays[] = array('utf16_swedish_ci', 'utf16_swedish_ci', '瑞典语, 不区分大小写');
    $arrValueArrays[] = array('utf16_turkish_ci', 'utf16_turkish_ci', '土耳其语, 不区分大小写');
    $arrValueArrays[] = array('utf16_unicode_ci', 'utf16_unicode_ci', 'Unicode (多语言), 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'utf16', 'UTF-16 Unicode');
    $arrValueArrays[] = array($group.'_begin', 'utf32', 'UTF-32 Unicode');
    $arrValueArrays[] = array('utf32_bin', 'utf32_bin', '未知, 二进制');
    $arrValueArrays[] = array('utf32_czech_ci', 'utf32_czech_ci', '捷克语, 不区分大小写');
    $arrValueArrays[] = array('utf32_danish_ci', 'utf32_danish_ci', '丹麦语, 不区分大小写');
    $arrValueArrays[] = array('utf32_esperanto_ci', 'utf32_esperanto_ci', '世界语, 不区分大小写');
    $arrValueArrays[] = array('utf32_estonian_ci', 'utf32_estonian_ci', '爱沙尼亚语, 不区分大小写');
    $arrValueArrays[] = array('utf32_general_ci', 'utf32_general_ci', '未知, 不区分大小写');
    $arrValueArrays[] = array('utf32_hungarian_ci', 'utf32_hungarian_ci', '匈牙利语, 不区分大小写');
    $arrValueArrays[] = array('utf32_icelandic_ci', 'utf32_icelandic_ci', '冰岛语, 不区分大小写');
    $arrValueArrays[] = array('utf32_latvian_ci', 'utf32_latvian_ci', '拉脱维亚语, 不区分大小写');
    $arrValueArrays[] = array('utf32_lithuanian_ci', 'utf32_lithuanian_ci', '立陶宛语, 不区分大小写');
    $arrValueArrays[] = array('utf32_persian_ci', 'utf32_persian_ci', '波斯语, 不区分大小写');
    $arrValueArrays[] = array('utf32_polish_ci', 'utf32_polish_ci', '波兰语, 不区分大小写');
    $arrValueArrays[] = array('utf32_roman_ci', 'utf32_roman_ci', '西欧, 不区分大小写');
    $arrValueArrays[] = array('utf32_romanian_ci', 'utf32_romanian_ci', '罗马尼亚语, 不区分大小写');
    $arrValueArrays[] = array('utf32_sinhala_ci', 'utf32_sinhala_ci', '未知, 不区分大小写');
    $arrValueArrays[] = array('utf32_slovak_ci', 'utf32_slovak_ci', '斯洛伐克语, 不区分大小写');
    $arrValueArrays[] = array('utf32_slovenian_ci', 'utf32_slovenian_ci', '斯洛文尼亚语, 不区分大小写');
    $arrValueArrays[] = array('utf32_spanish2_ci', 'utf32_spanish2_ci', '传统西班牙语, 不区分大小写');
    $arrValueArrays[] = array('utf32_spanish_ci', 'utf32_spanish_ci', '西班牙语, 不区分大小写');
    $arrValueArrays[] = array('utf32_swedish_ci', 'utf32_swedish_ci', '瑞典语, 不区分大小写');
    $arrValueArrays[] = array('utf32_turkish_ci', 'utf32_turkish_ci', '土耳其语, 不区分大小写');
    $arrValueArrays[] = array('utf32_unicode_ci', 'utf32_unicode_ci', 'Unicode (多语言), 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'utf32', 'UTF-32 Unicode');
    $arrValueArrays[] = array($group.'_begin', 'utf8', 'UTF-8 Unicode');
    $arrValueArrays[] = array('utf8_bin', 'utf8_bin', 'Unicode (多语言), 二进制');
    $arrValueArrays[] = array('utf8_czech_ci', 'utf8_czech_ci', '捷克语, 不区分大小写');
    $arrValueArrays[] = array('utf8_danish_ci', 'utf8_danish_ci', '丹麦语, 不区分大小写');
    $arrValueArrays[] = array('utf8_esperanto_ci', 'utf8_esperanto_ci', '世界语, 不区分大小写');
    $arrValueArrays[] = array('utf8_estonian_ci', 'utf8_estonian_ci', '爱沙尼亚语, 不区分大小写');
    $arrValueArrays[] = array('utf8_general_ci', 'utf8_general_ci', 'Unicode (多语言), 不区分大小写', '1');
    $arrValueArrays[] = array('utf8_hungarian_ci', 'utf8_hungarian_ci', '匈牙利语, 不区分大小写');
    $arrValueArrays[] = array('utf8_icelandic_ci', 'utf8_icelandic_ci', '冰岛语, 不区分大小写');
    $arrValueArrays[] = array('utf8_latvian_ci', 'utf8_latvian_ci', '拉脱维亚语, 不区分大小写');
    $arrValueArrays[] = array('utf8_lithuanian_ci', 'utf8_lithuanian_ci', '立陶宛语, 不区分大小写');
    $arrValueArrays[] = array('utf8_persian_ci', 'utf8_persian_ci', '波斯语, 不区分大小写');
    $arrValueArrays[] = array('utf8_polish_ci', 'utf8_polish_ci', '波兰语, 不区分大小写');
    $arrValueArrays[] = array('utf8_roman_ci', 'utf8_roman_ci', '西欧, 不区分大小写');
    $arrValueArrays[] = array('utf8_romanian_ci', 'utf8_romanian_ci', '罗马尼亚语, 不区分大小写');
    $arrValueArrays[] = array('utf8_sinhala_ci', 'utf8_sinhala_ci', '未知, 不区分大小写');
    $arrValueArrays[] = array('utf8_slovak_ci', 'utf8_slovak_ci', '斯洛伐克语, 不区分大小写');
    $arrValueArrays[] = array('utf8_slovenian_ci', 'utf8_slovenian_ci', '斯洛文尼亚语, 不区分大小写');
    $arrValueArrays[] = array('utf8_spanish2_ci', 'utf8_spanish2_ci', '传统西班牙语, 不区分大小写');
    $arrValueArrays[] = array('utf8_spanish_ci', 'utf8_spanish_ci', '西班牙语, 不区分大小写');
    $arrValueArrays[] = array('utf8_swedish_ci', 'utf8_swedish_ci', '瑞典语, 不区分大小写');
    $arrValueArrays[] = array('utf8_turkish_ci', 'utf8_turkish_ci', '土耳其语, 不区分大小写');
    $arrValueArrays[] = array('utf8_unicode_ci', 'utf8_unicode_ci', 'Unicode (多语言), 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'utf8', 'UTF-8 Unicode');
    $arrValueArrays[] = array($group.'_begin', 'utf8mb4', 'UTF-8 Unicode');
    $arrValueArrays[] = array('utf8mb4_bin', 'utf8mb4_bin', '未知, 二进制');
    $arrValueArrays[] = array('utf8mb4_czech_ci', 'utf8mb4_czech_ci', '捷克语, 不区分大小写');
    $arrValueArrays[] = array('utf8mb4_danish_ci', 'utf8mb4_danish_ci', '丹麦语, 不区分大小写');
    $arrValueArrays[] = array('utf8mb4_esperanto_ci', 'utf8mb4_esperanto_ci', '世界语, 不区分大小写');
    $arrValueArrays[] = array('utf8mb4_estonian_ci', 'utf8mb4_estonian_ci', '爱沙尼亚语, 不区分大小写');
    $arrValueArrays[] = array('utf8mb4_general_ci', 'utf8mb4_general_ci', '未知, 不区分大小写');
    $arrValueArrays[] = array('utf8mb4_hungarian_ci', 'utf8mb4_hungarian_ci', '匈牙利语, 不区分大小写');
    $arrValueArrays[] = array('utf8mb4_icelandic_ci', 'utf8mb4_icelandic_ci', '冰岛语, 不区分大小写');
    $arrValueArrays[] = array('utf8mb4_latvian_ci', 'utf8mb4_latvian_ci', '拉脱维亚语, 不区分大小写');
    $arrValueArrays[] = array('utf8mb4_lithuanian_ci', 'utf8mb4_lithuanian_ci', '立陶宛语, 不区分大小写');
    $arrValueArrays[] = array('utf8mb4_persian_ci', 'utf8mb4_persian_ci', '波斯语, 不区分大小写');
    $arrValueArrays[] = array('utf8mb4_polish_ci', 'utf8mb4_polish_ci', '波兰语, 不区分大小写');
    $arrValueArrays[] = array('utf8mb4_roman_ci', 'utf8mb4_roman_ci', '西欧, 不区分大小写');
    $arrValueArrays[] = array('utf8mb4_romanian_ci', 'utf8mb4_romanian_ci', '罗马尼亚语, 不区分大小写');
    $arrValueArrays[] = array('utf8mb4_sinhala_ci', 'utf8mb4_sinhala_ci', '未知, 不区分大小写');
    $arrValueArrays[] = array('utf8mb4_slovak_ci', 'utf8mb4_slovak_ci', '斯洛伐克语, 不区分大小写');
    $arrValueArrays[] = array('utf8mb4_slovenian_ci', 'utf8mb4_slovenian_ci', '斯洛文尼亚语, 不区分大小写');
    $arrValueArrays[] = array('utf8mb4_spanish2_ci', 'utf8mb4_spanish2_ci', '传统西班牙语, 不区分大小写');
    $arrValueArrays[] = array('utf8mb4_spanish_ci', 'utf8mb4_spanish_ci', '西班牙语, 不区分大小写');
    $arrValueArrays[] = array('utf8mb4_swedish_ci', 'utf8mb4_swedish_ci', '瑞典语, 不区分大小写');
    $arrValueArrays[] = array('utf8mb4_turkish_ci', 'utf8mb4_turkish_ci', '土耳其语, 不区分大小写');
    $arrValueArrays[] = array('utf8mb4_unicode_ci', 'utf8mb4_unicode_ci', 'Unicode (多语言), 不区分大小写');
    $arrValueArrays[] = array($group.'_end', 'utf8mb4', 'UTF-8 Unicode');
    return $arrValueArrays;
  }

  /**
   *
   * @access public
   * @static
   * @param string $separator Defaults to an string '\0'.
   * @param string $group Defaults to an string ''.
   * @return array Each element is still a array with one to four elements($value[, $name, $title, $selected])
   */
  public static function HtmlEditors($separator = '\0', $group = ''){
    $arrValueArrays = array();
    $arrValueArrays[] = array($group.'_begin', '[基本单项控件][basic single editors]', '');
    $arrValueArrays[] = array('text', '[输入控件][单行输入][text]', '', '1');
    $arrValueArrays[] = array('hidden', '[输入控件][隐藏输入][hidden]', '');
    $arrValueArrays[] = array('password', '[输入控件][密码输入][password]', '');
    $arrValueArrays[] = array('radio', '[输入控件][单选按钮][radio]', '');
    $arrValueArrays[] = array('checkbox', '[输入控件][单复选框][checkbox]', '');
    $arrValueArrays[] = array('select', '[选择列表][单选菜单][select]', '');
    $arrValueArrays[] = array('textarea', '[多行文本][textarea]', '');
    $arrValueArrays[] = array('file', '[输入控件][单个文件][file]', '');
    $arrValueArrays[] = array($group.'_end', '[基本单项控件][basic single editors]', '');
    $arrValueArrays[] = array($group.'_begin', '[基本多项控件][basic multiple editors]', '');
    $arrValueArrays[] = array('texts', '[输入控件][多行输入][texts]', '');
    $arrValueArrays[] = array('checkboxes', '[输入控件][多复选框][checkboxes]', '');
    $arrValueArrays[] = array('selects', '[选择列表][多选菜单][selects]', '');
    $arrValueArrays[] = array('files', '[输入控件][多个文件][files]', '');
    $arrValueArrays[] = array($group.'_end', '[基本多项控件][basic multiple editors]', '');
    $arrValueArrays[] = array($group.'_begin', '[扩展单项控件][extend single editors]', '');
    $arrValueArrays[] = array('textselect', '[文本选填][textselect]', '');
    $arrValueArrays[] = array('texttags', '[文本标签][texttags]', '');
    $arrValueArrays[] = array('pcas', '[地址联动][pcas]', 'Province City Area Selector 省、市、地区联动选择');
    $arrValueArrays[] = array('pcasdict', '[地址联动字典][pcasdict]', 'Province City Area Selector 省、市、地区联动选择');
    $arrValueArrays[] = array('datepicker', '[日期选择][datepicker][Y-m-d H:i:s]', '');
    $arrValueArrays[] = array('farbtastic', '[颜色选择][farbtastic]', '');
    $arrValueArrays[] = array('baidumappoint', '[百度地图][坐标点][baidumappoint]', '');
    $arrValueArrays[] = array('browserfile', '[浏览文件][单个文件][browserfile]', '');
    $arrValueArrays[] = array('ckeditor', '[富编辑器][CKEditor]', '');
    $arrValueArrays[] = array('ueditor', '[富编辑器][UEditor]', '');
    //$arrValueArrays[] = array('kindeditor', '[富编辑器][kindeditor]', '');
    $arrValueArrays[] = array($group.'_end', '[扩展单项控件][extend single editors]', '');
    $arrValueArrays[] = array($group.'_begin', '[扩展多项控件][extend multiple editors]', '');
    $arrValueArrays[] = array($group.'_end', '[扩展多项控件][extend multiple editors]', '');
    return $arrValueArrays;
  }

  /**
   *
   * @access public
   * @static
   * @param string $separator Defaults to an string '\0'.
   * @param string $group Defaults to an string ''.
   * @return array Each element is still a array with one to four elements($value[, $name, $title, $selected])
   */
  public static function FieldTypes($separator = '\0', $group = ''){
    $arrValueArrays = array();
    $arrValueArrays[] = array($group.'_begin', 'STRING', '');
    $arrValueArrays[] = array('char', 'char', '');
    $arrValueArrays[] = array('varchar', 'varchar', '', '1');
    $arrValueArrays[] = array('text', 'text', '');
    $arrValueArrays[] = array('blob', 'blob', '');
    $arrValueArrays[] = array($group.'_end', 'STRING', '');
    $arrValueArrays[] = array($group.'_begin', 'NUMERIC', '');
    $arrValueArrays[] = array('int', 'int', '');
    $arrValueArrays[] = array('float', 'float', '');
    $arrValueArrays[] = array('numeric', 'numeric', '');
    $arrValueArrays[] = array($group.'_end', 'NUMERIC', '');
    //$arrValueArrays[] = array($group.'_begin' . $separate . 'DATE AND TIME' . $separate . '');
    //$arrValueArrays[] = array('datetime' . $separate . 'datetime' . $separate . '');
    //$arrValueArrays[] = array($group.'_end' . $separate . 'DATE AND TIME' . $separate . '');
    //$arrValueArrays[] = array($group.'_begin' . $separate . 'OTHER' . $separate . '');
    //$arrValueArrays[] = array('serial' . $separate . 'serial' . $separate . 'auto incrementing fields');
    //$arrValueArrays[] = array($group.'_end' . $separate . 'OTHER' . $separate . '');
    return $arrValueArrays;
  }

  /**
   *
   * @access public
   * @static
   * @param string $separator Defaults to an string '\0'.
   * @param string $group Defaults to an string ''.
   * @return array Each element is still a array with one to four elements($value[, $name, $title, $selected])
   */
  public static function FieldSizes($separator = '\0', $group = ''){
    $arrValueArrays = array();
    $arrValueArrays[] = array('tiny', 'tiny', '');
    $arrValueArrays[] = array('small', 'small', '');
    $arrValueArrays[] = array('medium', 'medium', '');
    $arrValueArrays[] = array('normal', 'normal', '', '1');
    $arrValueArrays[] = array('big', 'big', '');
    return $arrValueArrays;
  }

  /**
   *
   * @access public
   * @static
   * @param string $separator Defaults to an string '\0'.
   * @param string $group Defaults to an string ''.
   * @return array Each element is still a array with one to four elements($value[, $name, $title, $selected])
   */
  public static function RegularExpressions($separator = '\0', $group = ''){
    $arrValueArrays = array();
    $arrValueArrays[] = array('', '', '无匹配', '1');
    $arrValueArrays[] = array($group.'_begin', '空匹配', '');
    $arrValueArrays[] = array('/^\s*$/', '空', '匹配空');
    $arrValueArrays[] = array('/[^\s]+/', '非空', '匹配非空');
    $arrValueArrays[] = array($group.'_end', '空匹配', '');
    $arrValueArrays[] = array($group.'_begin', '数值匹配[非空]', '');
    $arrValueArrays[] = array('/^-?\d+(\.\d+)?$/', '数值[正/负/零]', '匹配有符号数值');
    $arrValueArrays[] = array('/^\d+(\.\d+)?$/', '非负数值[正/零]', '匹配非负数值');
    $arrValueArrays[] = array('/^(-\d+(\.\d+)?)|(0+(\.0+)?)$/', '非正数值[负/零]', '匹配非正数值');
    $arrValueArrays[] = array('/^-?(([0-9]+\.[0-9]*[1-9][0-9]*)|([0-9]*[1-9][0-9]*\.[0-9]+)|([0-9]*[1-9][0-9]*))$/', '非零数值[正/负]', '匹配非零数值');
    $arrValueArrays[] = array('/^(([0-9]+\.[0-9]*[1-9][0-9]*)|([0-9]*[1-9][0-9]*\.[0-9]+)|([0-9]*[1-9][0-9]*))$/', '正数值[正]', '匹配正数值');
    $arrValueArrays[] = array('/^-(([0-9]+\.[0-9]*[1-9][0-9]*)|([0-9]*[1-9][0-9]*\.[0-9]+)|([0-9]*[1-9][0-9]*))$/', '负数值[负]', '匹配负数值');
    $arrValueArrays[] = array($group.'_end', '数值匹配[非空]', '');
    $arrValueArrays[] = array($group.'_begin', '整数匹配[非空]', '');
    $arrValueArrays[] = array('/^-?\d+$/', '整数[正/负/零]', '匹配有符号整数');
    $arrValueArrays[] = array('/^\d+$/', '非负整数[正/零]', '匹配非负整数');
    $arrValueArrays[] = array('/^(-\d+)|(0+)$/', '非正整数[负/零]', '匹配非正整数');
    $arrValueArrays[] = array('/^-?[0-9]*[1-9][0-9]*$/', '非零整数[正/负]', '匹配非零整数');
    $arrValueArrays[] = array('/^[0-9]*[1-9][0-9]*$/', '正整数[正]', '匹配正整数');
    $arrValueArrays[] = array('/^-[0-9]*[1-9][0-9]*$/', '负整数[负]', '匹配负整数');
    $arrValueArrays[] = array($group.'_end', '整数匹配[非空]', '');
    $arrValueArrays[] = array($group.'_begin', '英文字母匹配[非空]', '');
    $arrValueArrays[] = array('/^[A-Za-z]+$/', '英文字母[大写/小写]', '匹配英文字母，不区分大小写');
    $arrValueArrays[] = array('/^[A-Z]+$/', '大写英文字母[大写]', '匹配大写英文字母');
    $arrValueArrays[] = array('/^[a-z]+$/', '小写英文字母[小写]', '匹配小写英文字母');
    $arrValueArrays[] = array('/^[A-Za-z]$/', '单个英文字母[大写/小写]', '匹配单个英文字母，不区分大小写');
    $arrValueArrays[] = array('/^[A-Z]$/', '单个大写英文字母[大写]', '匹配单个大写英文字母');
    $arrValueArrays[] = array('/^[a-z]$/', '单个小写英文字母[小写]', '匹配单个小写英文字母');
    $arrValueArrays[] = array($group.'_end', '英文字母匹配[非空]', '');
    $arrValueArrays[] = array($group.'_begin', '汉字匹配(非空)', '');
    $arrValueArrays[] = array('/^[\x{4e00}-\x{9fa5}]+$/u', '汉字[UTF-8编码]', '匹配UTF-8编码汉字');
    $arrValueArrays[] = array('/^[\x{4e00}-\x{9fa5}]$/u', '单个汉字[UTF-8编码]', '匹配单个UTF-8编码汉字');
    //$arrValueArrays[] = array('/^['.chr(0xa1).'-'.chr(0xff).']+$/', '汉字[GB2312编码]', '匹配GB2312编码汉字');
    //$arrValueArrays[] = array('/^['.chr(0xa1).'-'.chr(0xff).']$/', '单个汉字[GB2312编码]', '匹配单个GB2312编码汉字');
    $arrValueArrays[] = array($group.'_end', '汉字匹配(非空)', '');
    $arrValueArrays[] = array($group.'_begin', '其他匹配(非空)', '');
    $arrValueArrays[] = array('/^\w+$/', '任意单词字符[英文字母/数字/下划线]', '匹配英文字母、数字、下划线');
    $arrValueArrays[] = array('/^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*$/', 'Email地址', '匹配Email地址');
    $arrValueArrays[] = array('/(^\d{15}$)|(^\d{17}([0-9]|X)$)/', '中国身份证号', '匹配中国身份证号');
    $arrValueArrays[] = array('/^(\+?0?86-)?1[3458]{1}\d{9}$/', '中国手机号码[限制号段]', '匹配中国手机号码[限制号段]');
    $arrValueArrays[] = array('/^(\+?0?86-)?1\d{10}$/', '中国手机号码', '匹配中国手机号码');
    $arrValueArrays[] = array('/^(\+?0?86-)?(\d{3,4}-)?(\d{7,8})$/', '中国电话号码', '匹配中国电话号码');
    $arrValueArrays[] = array('/^\d{6}$/', '中国邮政编码', '匹配中国邮政编码');
    $arrValueArrays[] = array($group.'_end', '其他匹配(非空)', '');
    return $arrValueArrays;
  }
  
  /**
   *
   * @access public
   * @static
   * @param string  $editor html editor
   * @return array() The loaded css and js tag.
   */
  public static function HtmlCssJsOfEditor($editor = ''){
    $arrHtmlCssJs = array(
      'js_jquery' => '<script type="text/javascript" src="'.base_path().'misc/resources/jquery/jquery-1.7.2.min.js"></script>',
      'js_jqueryui' => '<script type="text/javascript" src="'.base_path().'misc/resources/jquery/jqueryui/js/jquery-ui-1.8.18.custom.min.js"></script>',
      'css_jqueryui' => '<link rel="stylesheet" type="text/css" href="'.base_path().'misc/resources/jquery/jqueryui/css/ui-lightness/jquery-ui-1.8.18.custom.css">',
      'js_jqueryui-datepicker-zh-CN' => '<script type="text/javascript" src="'.base_path().'misc/resources/jquery/jqueryui/development-bundle/ui/i18n/jquery.ui.datepicker-zh-CN.js"></script>',
      'js_jqueryui-datepicker-zh-TW' => '<script type="text/javascript" src="'.base_path().'misc/resources/jquery/jqueryui/development-bundle/ui/i18n/jquery.ui.datepicker-zh-TW.js"></script>',
      'js_jqueryui-datepicker-en-GB' => '<script type="text/javascript" src="'.base_path().'misc/resources/jquery/jqueryui/development-bundle/ui/i18n/jquery.ui.datepicker-en-GB.js"></script>',
      'js_pcas' => '<script type="text/javascript" src="'.base_path().'misc/resources/pcas/PCASClass.js"></script>',
      'js_pcasdict' => '<script type="text/javascript" src="'.base_path().'misc/resources/jquery/pcasdict/pcasdict.js"></script>',
      'js_farbtastic' => '<script type="text/javascript" src="'.base_path().'misc/resources/jquery/farbtastic/farbtastic.js"></script>',
      'css_farbtastic' => '<link rel="stylesheet" type="text/css" href="'.base_path().'misc/resources/jquery/farbtastic/farbtastic.css">',
      'js_baidumappoint' => '<script type="text/javascript" src="http://api.map.baidu.com/api?v=1.3"></script>',
      'js_ckeditor' => '<script type="text/javascript" src="'.base_path().'misc/resources/ckeditor/ckeditor.js"></script>',
      'js_ckfinder' => '<script type="text/javascript" src="'.base_path().'misc/resources/ckfinder/ckfinder.js"></script>',
      'js_ueditor_config' => '<script type="text/javascript" src="'.base_path().'misc/resources/ueditor/ueditor.config.js"></script>',
      'js_ueditor_all' => '<script type="text/javascript" src="'.base_path().'misc/resources/ueditor/ueditor.all.min.js"></script>'
    );
    switch ($editor) {
      case 'checkbox':
      case 'file':
      case 'files':
      case 'textselect':
      case 'texttags':
        return array('js_jquery' => $arrHtmlCssJs['js_jquery']);
        break;
      case 'pcas':
        return array('js_pcas' => $arrHtmlCssJs['js_pcas']);
        break;
      case 'pcasdict':
        return array(
          'js_jquery' => $arrHtmlCssJs['js_jquery'],
          'js_pcasdict' => $arrHtmlCssJs['js_pcasdict']
        );
        break;
      case 'datepicker':
        return array(
          'js_jquery' => $arrHtmlCssJs['js_jquery'],
          'js_jqueryui' => $arrHtmlCssJs['js_jqueryui'],
          'css_jqueryui' => $arrHtmlCssJs['css_jqueryui'],
          'js_jqueryui-datepicker-'.ts('zh-CN', '', 'default') => $arrHtmlCssJs['js_jqueryui-datepicker-'.ts('zh-CN', '', 'default')]
        );
        break;
      case 'farbtastic':
        return array(
          'js_jquery' => $arrHtmlCssJs['js_jquery'],
          'js_farbtastic' => $arrHtmlCssJs['js_farbtastic'],
          'css_farbtastic' => $arrHtmlCssJs['css_farbtastic']
        );
        break;
      case 'baidumappoint':
        return array(
          'js_jquery' => $arrHtmlCssJs['js_jquery'],
          'js_baidumappoint' => $arrHtmlCssJs['js_baidumappoint']
        );
        break;
      case 'browserfile':
        return array(
          'js_jquery' => $arrHtmlCssJs['js_jquery'],
          'js_ckfinder' => $arrHtmlCssJs['js_ckfinder']
        );
        break;
      case 'ckeditor':
        return array(
          'js_jquery' => $arrHtmlCssJs['js_jquery'],
          'js_ckeditor' => $arrHtmlCssJs['js_ckeditor']
        );
        break;
      case 'ueditor':
        return array(
          'js_jquery' => $arrHtmlCssJs['js_jquery'],
          'js_ueditor_config' => $arrHtmlCssJs['js_ueditor_config'],
          'js_ueditor_all' => $arrHtmlCssJs['js_ueditor_all']
        );
        break;
      default:
        return array();
        break;
    }
  }
}