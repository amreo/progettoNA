<?php
 $con = mysqli_connect('localhost','root','PASSWORD','dati_produzione');
?>
<!DOCTYPE HTML>
<html>
<head>
 <meta charset="utf-8">
 <title>
 Grafico Produzione
 </title>
 <script type="text/javascript" src="https://www.google.com/jsapi"></script>
 <script type="text/javascript">
 google.load("visualization", "1", {packages:["corechart"]});
 google.setOnLoadCallback(drawChart);
 function drawChart() {

 var data = google.visualization.arrayToDataTable([
 ['Nome', 'numProdotti'],
 <?php 
 $query = "SELECT sum(numProdotti) AS numProdotti, Nome FROM dati_produzione.output_catena GROUP BY Nome;";

 $exec = mysqli_query($con,$query);
 while($row = mysqli_fetch_array($exec)){

 echo "['".$row['Nome']."',".$row['numProdotti']."],";
 }
 ?>
 ]);

 var options = {
 title: 'Statistiche di produzione'
 };

 var chart = new google.visualization.PieChart(document.getElementById('piechart'));

 chart.draw(data, options);
 }
 </script>
</head>
<body>
 <div id="piechart" style="width: 1200px; height: 800px;"></div>
</body>
</html>
