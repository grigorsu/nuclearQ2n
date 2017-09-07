<?php
 ob_start();

include 'gFuns.php';
if(!isset( $_SESSION["code"] ) and ($_SESSION["code"] != "passed") )  { header("Location: step0.php"); die(); };
g_html_head();

$conf_prob = $_SESSION['conf_prob']          ;
$conf_not_spherical = $_SESSION['conf_not_spherical'] ;
$conf_run = $_SESSION['conf_run']           ;
$conf_makeplot = $_SESSION['conf_makeplot']           ;

$energies = $conf_prob->get_all("Esp");
$PELSS    = $conf_prob->get_all("pelss");
$sigma_sp = $conf_prob->get_all("sigma_sp");
$energies_d = $conf_not_spherical->get_all("vEd(ss24)");
$sigma_d    = $conf_not_spherical->get_all("ss24");
$reaction_html = "<sup>".$conf_run->get("A1")."</sup>".$conf_run->get("Element1")
                 ."+<sup>".$conf_run->get("A2")."</sup>".$conf_run->get("Element2");

$UBar = $conf_makeplot->get("UBar");
$_SESSION['UBar'] = $UBar;
if(!isset($_SESSION['UBar_shift']) ) 
  $_SESSION['UBar_shift'] = $UBar + 0.0; 

  // plot script
    echo " <script type='text/javascript' src='https://www.gstatic.com/charts/loader.js'></script> ";
    echo " <script type='text/javascript'> ";
    echo " google.charts.load('current', {'packages':['corechart']}); ";
    echo " google.charts.setOnLoadCallback(drawChart); ";
    echo " function drawChart() {
            var data = google.visualization.arrayToDataTable([
              ['Energy', 'Sigma'],";
              for ($i = 0; $i < count($energies_d); $i = $i +1) 
                echo "[ ".($energies_d[$i] + $_SESSION['UBar_shift'] - $UBar).", ".$sigma_d[$i]." ],";
    echo "  ]);
            var options = {
              title: '  ',
              hAxis: {title: 'Energy, (MeV)',  titleTextStyle: {color: '#333'}},
              vAxis: {title: 'Sigma (mb)', logScale: true }
              
            };
            var chart = new google.visualization.LineChart(document.getElementById('chart_div'));
            chart.draw(data, options);
          }
          </script>";
  // form
          echo "<tr><td><form action='handler.php' method='post'><center>";
            echo "<br> reaction: $reaction_html, ";
            echo "     UBar: ".sprintf("%.2f", $UBar)."MeV";
            echo "     &nbsp &nbsp &nbsp <button value='step_03_00' name='button'> back </button>";
            echo "     &nbsp &nbsp &nbsp <button value='reset' name='button'> reset </button>";
            echo "<br><br> &nbsp &nbsp &nbsp You may change U<sub>bar</sub> in the range U<sub>bar</sub> ± 10 MeV: ";
            echo "            <input maxlength='5' size='5' name='UBar_shift' value=''>   ";
            echo "      &nbsp <button value='UBar_shift'   name='button'>change</button>";
            if($_SESSION['UBar_shift']!=$UBar) 
              echo " <b> U<sub>bar</sub><sup>NEW</sup> = " .sprintf("%.2f", ( $_SESSION['UBar_shift'] ) )."MeV </b>";

            echo "<table border='1'>";
            echo "<tr>";
            // echo "<td>";
            //   echo "<center><h3> spherical </h3></center>";
            //   echo "<table><tr><td width='100'>Ecm</td> <td  width='120'>PELLS(L=0)</td><td  width='120'>sigma</td></tr>";
            //   for ($i = 0; $i < count($energies); $i = $i +1) {
            //     // echo
            //     echo "<tr>";
            //     echo "<td> ".$energies[$i]." </td> ";
            //     echo "<td> ". $PELSS[$i]." </td> ";
            //     echo "<td> ".$sigma_sp[$i]." </td> ";
            //     echo "</tr>" ;
            //     # code...
            //   }
            //   echo "</table>";
            // echo "</td>";
            echo "<td width='225'>";
              // echo "<center><h3> deformed </h3></center>";
              echo "<table><tr><td width='100'>Ecm</td> <td  width='120'>sigma</td></tr>";
              for ($i = 0; $i < count($energies_d); $i = $i +1) {
                // echo
                echo "<tr>";
                // echo "<td> ".($energies_d[$i] )." </td> ";
                // echo "<td> ".($_SESSION['UBar_shift'] )." </td> ";
                // echo "<td> ".($UBar)." </td> ";
                echo "<td> ".($energies_d[$i] + $_SESSION['UBar_shift'] - $UBar)." </td> ";
                echo "<td> ".$sigma_d[$i]." </td> ";
                echo "</tr>" ;
                # code...
              }
              echo "</table>";
            echo "</td>";
              
            echo "<td valign='top' width='600'>";
              echo "<br><div id='chart_div' style=' width : 100%; height: 400px;'></div>";
            echo "</td>";
            echo "</td></tr></table>";
          echo "</center></form></td></tr>";

  g_html_tail();

ob_flush();
?>

