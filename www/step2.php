<?php 
 ob_start();

include 'gFuns.php';

// if(isset($_SESSION['is_handler']) )  { unset($_SESSION['is_handler']); } else {  header("Location: step0.php"); die(); };
if(!isset( $_SESSION["code"] ) and ($_SESSION["code"] != "passed") )  { header("Location: step0.php"); die(); };

g_html_head();
// for debug:
  // echo '<tr><td> $_POST: <br>';
  //   print_r($_POST);
  // echo "</td></tr>";
  // echo '<tr><td> $_SESSION: <br>';
  //   print_r($_SESSION);
  // echo "</td></tr>";
  // echo '<tr><td> $conf: <br>';
  //   $_SESSION['conf']->print();
  // echo "</td></tr>";


// define $conf_run
  $tail = "";
  $conf_run = new gConf("./conf/conf.INP.default.txt");
  if(  ($_SESSION['conf']->get('is_Q2_channel') == 'yes') ) {
    $tail = "_new";
  }

  $conf_run->change("A1"       , $_SESSION['conf']->get("A1".$tail      ) );
  $conf_run->change("A2"       , $_SESSION['conf']->get("A2".$tail      ) );
  $conf_run->change("m1"       , $_SESSION['conf']->get("m1".$tail      ) );
  $conf_run->change("m2"       , $_SESSION['conf']->get("m2".$tail      ) );
  $conf_run->change("beta1"    , $_SESSION['conf']->get("beta1".$tail   ) );
  $conf_run->change("beta2"    , $_SESSION['conf']->get("beta2".$tail   ) );
  $conf_run->change("d1"       , $_SESSION['conf']->get("d1".$tail      ) );
  $conf_run->change("d2"       , $_SESSION['conf']->get("d2".$tail      ) );
  $conf_run->change("Element1" , $_SESSION['conf']->get("Element1".$tail) );
  $conf_run->change("Element2" , $_SESSION['conf']->get("Element2".$tail) );
  $conf_run->change("reaction" , $_SESSION['conf']->get("reaction".$tail) );

  $conf_run->change("gamma" , $_SESSION['conf']->get("gamma") );
  $conf_run->change("is_Q2_channel" , $_SESSION['conf']->get("is_Q2_channel") );
  $conf_run->change("Q" , $_SESSION['conf']->get("Q") );
  $conf_run->change("Z1" , $_SESSION['conf']->get("Z1") );
  $conf_run->change("Z2" , $_SESSION['conf']->get("Z2") );
  if($_SESSION['conf']->is_present("E2")) 
    $conf_run->change("E2" , $_SESSION['conf']->get("E2") );
  // } else {
    // $conf_run->change("E2" , "nan") );
  // }

// define $conf_server_status
  $conf_server_status = new gConf("./conf.server.status.txt"); 
    if(   !($conf_server_status->is_present("01STATUS") ) 
       or !($conf_server_status->is_present("SESSION" ) ) 
       )  {
      header("Refresh: 1");
      exit;
    }

//main operations
  $ready  = ($conf_server_status -> get('01STATUS')  == 'ready');           // server is not busy (finished tasks)
  $lock         = ($conf_server_status -> get('SESSION')   != 'open');            // step2.php has not locked the server  
  $long_time    = ( (time() - $conf_server_status->get("TIME")) > 600  ) ;           // lock time is too long - more than 10 * 60 seconds
  $is_me_lock = ( $conf_server_status -> get('SESSION') == 'lock'.session_id() );    // if this is me who locked the server 
  if( $ready and (!$lock) ) {
    $conf_server_status -> change('01STATUS','locked.by.step2.php');
    $conf_server_status -> change('SESSION','lock'.session_id() );
    $conf_server_status -> change('TIME',   time() );
    $conf_server_status->save();
    $conf_run->save("./conf/conf.INP.2.txt");
    $conf_server_status -> change('01STATUS','all_run');
    $conf_server_status->save();
    echo "<tr><td>";
      echo "<br><center><img src='./img/page-loader.gif'></center>";
    echo "</td></tr>";
    header("Refresh: 5");
  } elseif(  $ready             and $lock and $long_time )   { // $ready, locked, too long time ago 
    $conf_server_status -> change('SESSION',  'open');          // unlock
    $conf_server_status -> change('TIME', '0');
    $conf_server_status -> save();
    header("Location: step2.php"); die();
  } elseif((!$ready )           and $lock and $is_me_lock )        { // not $ready, me locked
    echo "<tr><td>";
      if($conf_server_status->get("10MESSAGE")   != 'None')
        echo "<p style='color : red;' >  ".str_replace("_"," ",$conf_server_status->get('10MESSAGE'))."</p>";
      if($conf_server_status->get("10MESSAGE01") != 'None')
        echo "<p style='color : blue;'>  ".str_replace("_"," ",$conf_server_status->get('10MESSAGE01'))."</p>";
      if($conf_server_status->get("10MESSAGE02") != 'None')
              echo "<p style='color : blue;'>  ".str_replace("_"," ",$conf_server_status->get('10MESSAGE02'))."</p>";
      echo "<br><center><img src='./img/page-loader.gif'></center>";
    echo "</td></tr>";
    header("Refresh: 5");
  // } elseif( $lock and !$long_time and !$is_me_lock  ) {
  } elseif( $lock and !$long_time and !$is_me_lock  ) {
    echo "<tr><td><form action='handler.php' method='post'>";
      echo "<br> Server is busy, Would You try a little later, please?";
      echo "&nbsp&nbsp&nbsp <button value='step_01_02' name='button'> Update </button>";
      echo "&nbsp&nbsp&nbsp <button value='step_01_00' name='button'> Back </button>";
      echo "&nbsp&nbsp&nbsp <button value='reset' name='button'>      Reset </button>";
    echo "</form></td></tr>";
  } elseif( $ready  and lock and !$long_time  ) {
    $conf_prob          = new gConf("./conf/conf.prob.txt");
    $conf_not_spherical = new gConf("./conf/conf.not_spherical.txt");
    $conf_makeplot      = new gConf("./conf/conf.makeplot.txt");
    
    $_SESSION['conf_prob']          = $conf_prob;
    $_SESSION['conf_not_spherical'] = $conf_not_spherical;
    $_SESSION['conf_run']           = $conf_run;
    $_SESSION['conf_makeplot']      = $conf_makeplot;

    $conf_server_status -> change('SESSION',  ' open');
    $conf_server_status -> change('TIME',  ' 0');
    $conf_server_status -> save();
    header("Location: step3.php"); die();
  }

g_html_tail();

ob_flush();
?>