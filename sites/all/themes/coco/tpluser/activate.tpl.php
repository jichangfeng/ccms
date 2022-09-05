<?php
if(isset($_GET['uid']) && $account = user_load($_GET['uid'])){
  include 'activate-step2.tpl.php';
}else{
  include 'activate-step1.tpl.php';
}