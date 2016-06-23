<?php
mysql_connect("127.0.0.1", "root", "PASSWORD") or die("Connection Failed");

mysql_select_db("dati_produzione")or die("Connection Failed");

$idstazione = $_POST['idstazione'];

$lineaproduzione = $_POST['lineaproduzione'];

$timeout = $_POST['timeout'];

$query = "INSERT INTO dati_produzione.settings (IDstazione, Lineaproduzione, Barcodetimeout)
VALUES ($idstazione,$lineaproduzione,$timeout)";

if(mysql_query($query))
{
echo "updated";}

else{
echo "fail";}
?>
