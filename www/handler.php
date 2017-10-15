<?php
 ob_start();

include 'gFuns.php';

// echo session_cache_limiter(); 
  // <- session_start();
// $_SESSION["is_handler"] = 'true';

// $_POST[$str_var] -> $_SESSION[$str_var]
function post2sess($str_var, $min = 0, $max = 0) {
  $var = "";
  if(  isset($_POST[$str_var]) and  ($min == 0) and ($max == 0) ) {
    $var = $_POST[$str_var];
    $_SESSION[$str_var] = $var;
  } elseif (  isset($_POST[$str_var]) and  is_numeric( $_POST[$str_var] ) and  ($_POST[$str_var] >=$min) and   ($_POST[$str_var] <= $max)) {
    $var = $_POST[$str_var];
    $_SESSION[$str_var] = $var;
  }
  return $var;
}

function check_condition($condition, $message) {
  if($condition) {
    $_SESSION['message'] = $message;
    header("Location: step0.php");
    die();
  }
}
if      ( ! isset($_POST['button']) ) check_condition((0==1), "error: handler: no button");

if      ( isset($_POST['button']) and ( $_POST['button'] == "step_00_01" )  ) {
  // set $_SESSION['A1', 'A2', 'Z1', 'Z2'],  $_SESSION['code'] = 'passed'  
    $A1        = post2sess("A1"       ,     0,   300); 
    $A2        = post2sess("A2"       ,     0,   300); 
    $Z1        = post2sess("Z1"       ,     0,   300); 
    $Z2        = post2sess("Z2"       ,     0,   300); 
    $post_code = post2sess("post_code", 11111, 99999);
    check_condition( $A1 =="" , "<br> A1 should be numeric number between 0 and 300 ");
    check_condition( $A2 =="" , "<br> A2 should be numeric number between 0 and 300 ");
    check_condition( $Z2 =="" , "<br> Z2 should be numeric number between 0 and 300 ");
    check_condition( $Z2 =="" , "<br> Z2 should be numeric number between 0 and 300 ");
    check_condition( (!isset($_SESSION["code"])) or ($post_code == "") or ($post_code != $_SESSION["code"])  
               , " Would you enter the number at picture, please?"   );
    $_SESSION["code"] = "passed";

  // set_params.exe ->  $conf -> $_SESSION['conf']:
    $_m = "$g_home_dir_closed/set_params/set_params.php.exe -t $g_home_dir_closed/set_params/isotopes.table.2.txt ".
         "-i ./conf/conf.INP.default.txt -A1 $A1 -A2 $A2 -Z1 $Z1 -Z2 $Z2 -print conf";
    $result = shell_exec($_m);
    $v_result =  g_split( str_replace("\n","#",$result) , '#', "");
    $conf = new gConf("");
    for($i = 0; $i < count($v_result); $i = $i + 1 ) {
     $conf->add($v_result[$i]);
    }
    $_SESSION['conf'] = $conf;
  // undefined isotopes:

  // print_r($_SESSION);print_r($_POST); echo "<br><br>$_m"; $conf->print();die();
  check_condition(  ($conf->get("message") != "nan") or ($conf->get("message01") != "nan") 
             , str_replace("_"," ",$conf->get("message"))."<br>".str_replace("_"," ",$conf->get("message01"))  ); 
 
  header("Location: step1.php"); die();
} elseif( isset($_POST['button']) and ( $_POST['button'] == "step_01_00" )  ) {
  unset($_SESSION['conf']); 
  unset($_SESSION['code']); 
  header("Location: step0.php"); die();
} elseif( isset($_POST['button']) and ( $_POST['button'] == "step_03_00" )  ) {
  unset($_SESSION['conf']); 
  unset($_SESSION['code']); 
  header("Location: step0.php"); die();
} elseif( isset($_POST['button']) and ( $_POST['button'] == "UBar_shift" )  ) {
  if(isset($_SESSION['UBar'])) post2sess('UBar_shift',($_SESSION['UBar']-10.0), ($_SESSION['UBar']+10.0));
  header("Location: step3.php");
  die();
  // } elseif( isset($_POST['button']) and ( $_POST['button'] == "shift_down" )  ) {
  //   if(!isset($_SESSION['shift'])) $_SESSION['shift'] = 0;
  //   $_SESSION['shift'] = $_SESSION['shift'] - 0.5;
  //   header("Location: step3.php");
  //   die();
} elseif( isset($_POST['button']) and ( $_POST['button'] == "reset" )  ) {
  session_destroy();
  header("Location: step0.php");
  die();
} elseif( isset($_POST['button']) and isset($_SESSION['code']) and ($_SESSION['code'] == "passed") and ( $_POST['button'] == "is_Q2_channel_yes" )  ) {
  $_SESSION['conf']->change("is_Q2_channel" , "yes" );
  header("Location: step2.php"); die();
} elseif( isset($_POST['button']) and isset($_SESSION['code']) and ($_SESSION['code'] == "passed") and ( $_POST['button'] == "is_Q2_channel_no"  )  ) {
  $_SESSION['conf']->change("is_Q2_channel" , "no" );
  header("Location: step2.php"); die();
} elseif( isset($_POST['button']) and isset($_SESSION['code']) and ($_SESSION['code'] == "passed") and ( $_POST['button'] == "step_01_02" )  ) {
  header("Location: step2.php"); die();
} elseif( isset($_POST['button']) and isset($_SESSION['code']) and ($_SESSION['code'] == "passed") and ( $_POST['button'] == "step_00_01" )  ) {

//   // $conf = $_SESSION['conf'];
//   // $conf->change("A1"            , $conf->get("A1_new"      ) );
//   // $conf->change("A2"            , $conf->get("A2_new"      ) );
//   // $conf->change("m1"            , $conf->get("m1_new"      ) );
//   // $conf->change("m2"            , $conf->get("m2_new"      ) );
//   // $conf->change("beta1"         , $conf->get("beta1_new"   ) );
//   // $conf->change("beta2"         , $conf->get("beta2_new"   ) );
//   // $conf->change("d1"            , $conf->get("d1_new"      ) );
//   // $conf->change("d2"            , $conf->get("d2_new"      ) );
//   // $conf->change("Element1"      , $conf->get("Element1_new") );
//   // $conf->change("Element2"      , $conf->get("Element2_new") );
//   // $conf->change("reaction"      , $conf->get("reaction_new") );
//   // $_SESSION['conf'] = $conf;

} else {
//   //
  check_condition(1, "handler: unknown command");
}

// examples:
  // ini_set('session.save_path',realpath(dirname($_SERVER['DOCUMENT_ROOT']) . '/../session'));
  // echo session_save_path();
  //------------
  // $conf_server_status = new gConf("./conf.server.status.txt"); 
  // if( !($conf_server_status->is_present("01STATUS") ) )  {
  //   header("Refresh: 1");
  //   exit;
  // }
  //------------
  // ini_set('session.save_path',realpath(  dirname($_SERVER['DOCUMENT_ROOT']) . '/external/www5/session'  ));
  // ini_set('session.gc_probability', 1);
  // echo session_id() ;
  //------------
  // header("Refresh: 5");
  //------------
  // echo "<td> <input type='hidden' name='form' value='step0' </td>";
  //------------
  // } elseif (isset($_SESSION[$str_var]) ) {
  // unset($_SESSION[$str_var]);
  //------------
  //--------------
  // $conf = $_SESSION['conf'];  $conf->change("is_Q2_channel" , "yes" );$_SESSION['conf'] = $conf;
  //--------------
  // $conf = $_SESSION['conf']; $conf->change("is_Q2_channel" , "no" );  $_SESSION['conf'] = $conf;
  //--------------

 ob_flush();
?>