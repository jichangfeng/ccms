<?php
function minifield_file_del($fid){
  $file = file_load($fid);
  if($file && $file->fid){
    echo file_delete($file, true) ? 'success' : 'failed';exit;
  }
  echo 'success';exit;
}

function minifield_file_download($fid){
  MiniFieldFile::FileDownloadByFid($fid);
}