<?php
mysql_connect("127.0.0.1", "root", "PASSWORD") or die("Connection Failed");

mysql_select_db("dati_produzione")or die("Connection Failed");

$idstazione = $_POST['idstazione'];

$lineaproduzione = $_POST['lineaproduzione'];

$timeout = $_POST['timeout'];


$query = "UPDATE dati_produzione.settings SET Lineaproduzione = $lineaproduzione, Barcodetimeout = $timeout WHERE IDstazione = $idstazione;";
if(mysql_query($query)){
echo "<script language=javascript>window.location.href='homepage.php';</script>"; ;}
else{
echo "fail";}
?>
