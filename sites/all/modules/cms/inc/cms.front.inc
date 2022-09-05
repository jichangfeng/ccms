<?php
function cms_front(){
  return MiniFieldCommon::Render(CMS_MODULE_PATH . '/tpl/cms-front.tpl.php', array());
}