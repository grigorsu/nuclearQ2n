<?php
 ob_start();

include 'gFuns.php';

// create $_SESSION['code']
  $post_code = random_int( 11111 , 99999 );
  create_image($post_code);

  function  create_image($_code){
       $im = @imagecreate(80, 30)or die("Cannot Initialize new GD image stream");
       $background_color = imagecolorallocate($im, 225, 200, 100);  // yellow
       $red = imagecolorallocate($im, 55, 110, 10);     // red
       imagestring($im, 7, 10, 8, $_code, $red);
       imageellipse($im, 40, 25, 60, 20, $red);
       imageellipse($im, 20, 15, 60, 20, $red);
       imagepng($im,"image.png");
       imagedestroy($im);
  }

  $_SESSION['code'] = $post_code; 

g_html_head();



// print $_SESSION['message'] and $_SESSION['message01']
  echo "<tr><td>";
    if(isset($_SESSION['message']) and $_SESSION['message'] != 'nan' ) {
      echo "<p style='color : red;' >  ".str_replace("_"," ", $_SESSION['message']) ."</p>";
      $_SESSION['message'] = 'nan';
    }
    if(isset($_SESSION['message01']) and $_SESSION['message01'] != 'nan' ) {
      echo "<p style='color : green;' >  ".str_replace("_"," ", $_SESSION['message01'] )."</p>";
      $_SESSION['message01'] = 'nan';
    }
  echo "</td></tr>";
              
// form A1, A2, Z1, Z2, 
          echo "<tr><td><form action='handler.php' method='post'>";
            echo "<br>";
            echo "<br>";
            echo "<table><tr >";
              echo "<td>A<sub>1</sub> : <input maxlength='3' size='3' name='A1' value='".g_session_get("A1")."'>   </td>";
              echo "<td>Z<sub>1</sub> : <input maxlength='3' size='3' name='Z1' value='".g_session_get("Z1")."'>   </td>";
              echo "<td>A<sub>2</sub> : <input maxlength='3' size='3' name='A2' value='".g_session_get("A2")."'>   </td>";
              echo "<td>Z<sub>2</sub> : <input maxlength='3' size='3' name='Z2' value='".g_session_get("Z2")."'>   </td>";
              echo "<td >enter the number: </td><td> <img src=image.png?".date("U")."> </td><td> <input maxlength='5' size='5' name='post_code' value=''>   </td>";
              echo "<td colspan='2' style='text-align: left;' ><button value='step_00_01' name='button'>next</button></td>";
            echo "</tr></table>";
          echo "</form></td></tr>";

g_html_tail();

ob_flush();
?>