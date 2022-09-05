<?php
if(isset($_GET['uid']) && $account = user_load($_GET['uid'])){
  include 'resetpass-step2.tpl.php';
}else{
  include 'resetpass-step1.tpl.php';
}