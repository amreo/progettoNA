<?php
mysql_connect("127.0.0.1", "root", "PASSWORD") or die("Connection Failed");
mysql_select_db("dati_produzione")or die("Connection Failed");
$idstazione = $_POST['idstazione'];
$lineaproduzione = $_POST['lineaproduzione'];
$timeout = $_POST['timeout'];
$descrizione = $_POST['descrizione'];
$query = "UPDATE settings (IDstazione, Lineaproduzione, Barcodetimeout, Descrizione) SET Lineaproduzione = $lineaproduzione,Barcodetimeout = $timeout,Descrizione='$descrizione',WHERE IDstazione = $idstazione";
if(mysql_query($query)){
echo "updated";}
else{
echo "fail";}
?>
