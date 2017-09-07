<?php 
 ob_start();

include 'gFuns.php';

// echo "<br><br>"; print_r($_POST);
// echo "<br><br>"; print_r($_SESSION);
// phpinfo();
// die();


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
  //   $conf->print();
  // echo "</td></tr>";
  // echo '<tr><td> $_m: <br>';
  //   echo "<br>$_m<br>";
  // echo "</td></tr>";

// Q 2n trasfer:
  if(!isset($_SESSION['conf']) ) { header("Location: step0.php"); die(); };

  echo "<tr><td><form action='handler.php' method='post'>";
    $conf  = $_SESSION['conf'];
    $Q         = sprintf("%.2f", $conf->get("Q"));
    $beta1     = $conf->get("beta1");
    $beta2     = $conf->get("beta2");
    $A1        = $conf->get("A1");
    $A2        = $conf->get("A2");
    $Element1  = $conf->get("Element1");
    $Element2  = $conf->get("Element2");
    $reaction_html = "<sup>$A1</sup>$Element1+<sup>$A2</sup>$Element2";
    if( is_numeric($Q) and ($Q > 0) ) {
        // $beta1_new = $conf->get("beta2");
        $beta1_new = $conf->get("beta1_new");
        $beta2_new = $conf->get("beta2_new");
        $A1_new    = $conf->get("A1_new");
        $A2_new    = $conf->get("A2_new");
        echo "<center><table border = '0', width='600' > <tr><td align='justify'>";
          echo "<br> &nbsp  &nbsp  &nbsp  For the reaction ".$reaction_html." the Q-value of 2n neutron transfer is Q<sub>2n</sub>=".$Q."MeV";
          echo "<br> &nbsp  &nbsp  &nbsp  We assume that the sub-barrier capture mainly depends on the two-neutron transfer with the ";
          echo "     positive Q-value. Our assumption is that, before the projectile is captured by the target ";
          echo "     nucleus (<b>before the crossing of the Coulomb barrier</b>) which is the slow process, the two-";
          echo "     neutron transfer occurs at larger separations that can lead to the population of the first ";
          echo "     2+ state in the recipient nucleus. So, the motion to the N/Z equilibrium starts in the system";
          echo "     before the capture.";
          echo "<br>  &nbsp  &nbsp  &nbsp  The average change of mass asymmetry is connected to the two-neutron transfer. Since";
          echo "     after the two-neutron transfer the mass numbers, the deformation parameters of the";
          echo "     interacting nuclei, and, correspondingly, the height and shape of the Coulomb barrier are";
          echo "     changed, one can expect an enhancement or suppression of the capture. The including of";
          echo "     two-neutron transfer channel in entrance channel into calculations leads to changing of";
          echo "     deformation parameters β<sub>1</sub>, β<sub>2</sub>.";
          echo "<br> &nbsp  &nbsp  &nbsp   <sup>".$A1    ."</sup>".$Element1."(β<sub>2</sub>=".$beta1    
                                    .") +  <sup>".$A2    ."</sup>".$Element2."(β<sub>2</sub>=".$beta2     
                                    .") → <sup>".$A1_new."</sup>".$Element1."(β<sub>2</sub>=".$beta1_new
                                    .") +  <sup>".$A2_new."</sup>".$Element2."(β<sub>2</sub>=".$beta2_new.")";
          echo "<br> &nbsp &nbsp &nbsp Note, that for sub-barrier energies, the results of calculations are very sensitive to the";
          echo "     quadrupole deformation parameters β<sub>1</sub>, β<sub>2</sub> of the interacting nuclei.";
          echo "<br> &nbsp &nbsp &nbsp Calculations with 2n transfer:    <button value='is_Q2_channel_yes' name='button'>Do!</button>";
          echo                       " Calculations without 2n transfer: <button value='is_Q2_channel_no'  name='button'>Do!</button>";
        echo "</td></tr></table> </center>";

        // echo "<br> &nbsp&nbsp&nbsp <button value='step_01_00' name='button'> Back </button>";
        // echo      "&nbsp&nbsp&nbsp <button value='reset' name='button'> Reset </button>";
    } else {
        echo "<br> reaction: " ;
        echo " <sup>".$A1    ."</sup>".$Element1."(β<sub>1</sub>=".$beta1    .") + <sup>".$A2    ."</sup>".$Element2."(β<sub>2</sub>=".$beta2    .")"; 
        echo "&nbsp&nbsp&nbsp <button value='step_01_00' name='button'> Back </button>";
        echo "                <button value='step_01_02' name='button'> Next </button>";
        echo "&nbsp&nbsp&nbsp <button value='reset' name='button'> Reset </button>";
    }


  echo "</form></td></tr>";

g_html_tail();

ob_flush();
?>