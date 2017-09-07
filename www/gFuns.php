<?php 

$g_home_url = "http://".$_SERVER['SERVER_NAME'].":8080/external/www5/";
$g_home_dir = $_SERVER['DOCUMENT_ROOT']."/external/www5/";
$g_home_dir_closed = $_SERVER['DOCUMENT_ROOT']."/external/www5/bin/";
$g_home_dir_closed = '/home/u178766/intel-robotru/cgi/bin/';
// $g_home_dir_closed = $_SERVER['DOCUMENT_ROOT']."/external/www3_closed/p/nuclear/bin/";
// $g_home_dir_closed = $_SERVER['DOCUMENT_ROOT']."/external/www3_closed/p/nuclear/bin/";
$ERROR_FILE = "$g_home_dir/php.log"; shell_exec("rm ".$ERROR_FILE); error_log("Error messages\n", 3, $ERROR_FILE);

// echo $g_home_dir . "/SESSIONS/";
// ini_set('session.save_path',$g_home_dir . "/SESSIONS/");
session_start();


// ini_set('display_errors',1);
function    g_session_get($str_var) { if(isset($_SESSION[$str_var])) { return $_SESSION[$str_var]; } else return ""; }



function  checkpasswd($login, $passw) {
  // echo " <br> $login <br> $passw ";
  //                 user                    password         uid
  if     ($login == "nuser"  && $passw == "ybrjkfq") return 1;    // user nuclear
  else return 0;
}

function is_in_group($current_user, $some_group) {
  $groups = array(
    "admin"    => array("gsu"),                     // everywhere
    "guest"    => array(""),                        // any guest
    "nuclear"  => array("nuser"),                 // calculations in jinr
    "student"  => array(""),                        // education
    "family"   => array("gsu", "kats"),             // family 
    "all"      => array("nuclear", "kats", "gsu")
  );
  if( array_key_exists($some_group, $groups) && in_array($current_user, $groups[$some_group], true) ) return true;
  return false;
}

function g_split( $_s, $c, $option = "all") {



  if(strlen($c) == 0) {
    echo "ERROR: ".__FILE__.": ".__LINE__.": undefined delimiter ... ";  error_log(" ",3,$ERROR_FILE); exit;
  }
  
  // remove END OF LINE
  $s="";
  for($i = 0; $i < strlen($_s); $i = $i + 1 ){
    if($_s[$i]==PHP_EOL) break;
    $s = $s.$_s[$i];
  }

  $v = array();
  if(strlen($s) == 0 ) return $v; 


  // aaa and bbb are all coordinates for delimiters
  $aaa = array(-1); // left boundary
  $bbb = array();       // right boundary
  
  for($indx = 0; $indx < strlen($s); $indx = $indx + 1) {
    $is_delimiter = ($c[0] == $s[$indx]);
    for( $ii = 0; $ii < strlen($c); ++$ii) {
      $is_delimiter = $is_delimiter or ($c[$ii] == $s[$indx]);
    }
    if( $is_delimiter ) {
      array_push($aaa, $indx); 
      array_push($bbb, $indx);
    }
  }
  array_push($bbb , strlen($s) );

  for( $i = 0; $i < count($bbb); ++$i) {
    if      ($bbb[$i] == $aaa[$i] + 1 ) {
      if($option == "all") array_push($v, "" );
    } elseif ($bbb[$i] > $aaa[$i] + 1) {
      array_push( $v, (substr($s, $aaa[$i] + 1, $bbb[$i] - $aaa[$i] - 1 )) );
    } else {
        echo "ERROR: ".__FILE__.": ".__LINE__.": aaa and bbb have incorrect values ... ";
        echo "<br>";
        print_r($aaa);
        echo "<br>";
        print_r($bbb);
       error_log(" ",3,$ERROR_FILE); exit;
    }
  }
  return $v;
}

function g_array_merge($arr1, $arr2) {
  for( $i = 0; $i < count($arr2); ++$i) {
    array_push($arr1, $arr2[$i]);
  }
  return $arr1;
}

function g_readdir($path = '.'){
  $files = array();
  if ($handle = opendir($path)) {
      while (false !== ($entry = readdir($handle))) {
          if ($entry != "." && $entry != "..") {
              array_push($files,$entry);
              // echo "$entry\n";
          }
      }
      closedir($handle);
  }
  return $files;
}

class gConf {
  var $_values;
  var $_keys;
  var $_filename;
  var $_s_values;  // for beauty printing

  function get_string($name) {
    $this->_error_if_is_not_present($name);
    for($ii = 0; $ii < count($this->_s_values); ++$ii) {
      if($this->_s_values[$ii] == $name) return $this->_s_values[$ii];
    }
  }
  function _ii_keys($name) {
    for($ii = count($this->_keys) - 1; $ii >= 0; --$ii) {
      if($name == $this->_keys[$ii]) return $ii;
    }
    return -1;
  }


  function __construct($filename) {
    // print "Конструктор класса BaseClass\n";
    $this->_values = array();
    $this->_s_values = array();
    $this->_keys = array();
    if($filename != "") $this->read_from_file($filename);
    $this->_filename = $filename;
  }
  function read_from_file($filename) {
    $fp = file($filename); 
    $this->_filename = $filename;
    for($i = 0; $i < count($fp); ++$i) 
      $this->add($fp[$i]); 
  }
  function get($name, $i = 0) {
    $this->_error_if_is_not_present($name, $i);
    return $this->_values[$name][$i];
  }
  function get_all($name) {
    $this->_error_if_is_not_present($name);
    return $this->_values[$name];      
  }
  function length($name) {
    $this->_error_if_is_not_present($name);
    return count($this->_values[$name]);      
  }
  function is_present($name) {
    return array_key_exists($name, $this->_values);
  }
  function is_true($name) {
    $this->_error_if_is_not_present($name);
    return $this->_values[$name] != "0";
  }
  function change($name,$new_value,$i=0) {
    $this->_error_if_is_not_present($name,$i);

    $this->_values[$name][$i] = $new_value;

    $message="";
    for($ii = 0; $ii < count($this->_values[$name]); $ii = $ii + 1 ) {
      $message = $message."   ".$this->_values[$name][$ii];
    }
    $iii = $this->_ii_keys($name);
    if($iii >=0 ) {
      $this->_s_values[$iii] = $message;
    }

  }
  function add($line) {
    // echo "<br>".$line;
    if( strlen($line) <= 1 || ( strlen($line) > 0 and $line[0] == '/' ) ) {
    // echo "<br>".$this->_filename." ".(strlen($line));
      array_push($this->_s_values,"");
      if(strlen($line) <= 1)
        array_push($this->_keys, "");
      else
        array_push($this->_keys, $line);
      return "";
    }

    $words = g_split($line, " \t", "");
    if(count($words) == 0 ) return 0;
    if($words[0] == "/") return 0;
    if(count($words) <= 1 ) {

    } 
    if( ! array_key_exists($words[0], $this->_values) ) {
      $this->_values[$words[0]] = array();
      array_push($this->_s_values,"");
      array_push($this->_keys, $words[0]);
    }
    for($i = 1; $i < count($words); $i = $i + 1) {
      array_push($this->_values[$words[0]],$words[$i]);
    }
    $i = ($this->_ii_keys($words[0]));
    if($i >= 0) {
      for($ii = strlen($words[0]); $ii < strlen($line); $ii=$ii+1) {
        if($line[$ii] == PHP_EOL ) break;
        $this->_s_values[$i] = $this->_s_values[$i].$line[$ii];
      }
    }

  }
  // function add($name, $value) {    }
  function print() {
    for($ii = 0; $ii < count($this->_keys); $ii=$ii+1 ) {
      echo "<br> ".$this->_keys[$ii]."  ".$this->_s_values[$ii]." " ;
    }
    // foreach ($this->_values as $key => $value) {
    //   echo "<br> $key   ";
    //   for($i=0; $i < count($value); $i = $i + 1 ) 
    //     echo " $value[$i]   ";
    //   echo " \n ";
    // }
  }
  function save($filename="") {
    if($filename=="") $filename = $this->_filename;
    @ $fp = fopen($filename, "w");
    if(!$fp) {
      echo "ERROR: ".__FILE__.": ".__LINE__.": can't open file ".$_filename;  error_log(" ",3,$ERROR_FILE); exit;
    }
    flock($fp, LOCK_EX);

    for($i = 0; $i < count($this->_keys); $i=$i+1 ) {
      $message = $this->_keys[$i].$this->_s_values[$i]."\n";
      fwrite($fp, $message );
    }
    flock($fp, LOCK_UN);    
    fclose($fp); 

    // if($filename=="") $filename = $this->_filename;
    // @ $fp = fopen($filename, "w");
    // if(!$fp) {
    //   echo "ERROR: ".__FILE__.": ".__LINE__.": can't open file ".$filename;  error_log(" ",3,$ERROR_FILE); exit;
    // }
    // flock($fp, LOCK_EX);
    // foreach ($this->_values as $key => $value) {
    //   // print_r($this->_values[$key]);
    //   $message = $key;
    //   // fwrite($fp, $key );
    //   for($i=0; $i < count($value); $i = $i + 1 ) {
    //     $message = $message."   ".$value[$i];
    //     // fwrite($fp, "   ".$value[$i] );
    //   }
    //   $message = $message."\n";
    //   // echo " $message";
    //   // fwrite($fp, " \n   " );
    //   fwrite($fp, $message );
    // }
    // flock($fp, LOCK_UN);    
    // fclose($fp);
  }
  function _error_if_is_not_present($name, $i = 0) {
    $result = array_key_exists($name, $this->_values);
    if($result) 
      $result = isset($this->_values[$name][$i]);
    if(!$result) {
      echo "ERROR: ".__FILE__.": ".__LINE__.": conf ".$this->_filename." has no variable $name with the index $i "; // error_log(" ",3,$ERROR_FILE); exit;
    } 
  }
}

function g_html_head() {
  // header
  echo "<html>"; 
  echo "<head>"; 
  echo "  <meta content='text/html; charset=ISO-8859-1' http-equiv='content-type'>"; 
  echo "  <title></title>"; 
  echo "</head>"; 
  echo "<body>"; 
  echo "<center>";
  echo "  <table style='text-align: left; width: 1210;' border='1' cellpadding='2' cellspacing='2'>";
  echo "    <tbody>";
  echo "      <tr><td style='text-align: center;'><img src='./img/row13.jpg'></td></tr>";
  echo "      <tr><td style='text-align: center;'><a href='./local_run/nuclear.zip'> download file for local run [ ~ 12 M ] </a></td></tr>";
}

function g_html_tail() {
  // tail
  echo "    </tbody>";
  echo "  </table style='text-align: left; width: 1210;' border='1' cellpadding='2' cellspacing='2'>";
  echo "</center>";
  echo "</body>"; 
}

?>
