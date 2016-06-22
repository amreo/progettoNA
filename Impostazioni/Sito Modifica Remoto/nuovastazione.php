<?php
mysql_connect("127.0.0.1", "root", "PASSWORD") or die("Connection Failed");
mysql_select_db("dati_produzione")or die("Connection Failed");
$idstazione = $_POST['idstazione'];
$lineaproduzione = $_POST['lineaproduzione'];
$timeout = $_POST['timeout'];
$descrizione = $_POST['descrizione'];
$query = "INSERT INTO settings (IDstazione, Lineaproduzione, Barcodetimeout,Descrizione)
VALUES ($idstazione,$lineaproduzione,$timeout,'$descrizione')";
if(mysql_query($query)){
echo "updated";}
else{
echo "fail";}
?>
