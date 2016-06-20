<?php
mysql_connect("Host Name", "User Name", "User Password") or die("Connection Failed");
mysql_select_db("DataBase Name")or die("Connection Failed");
$arduinoid = $_POST['arduinoid'];
$lineaproduzione = $_POST['lineaproduzione'];

$timeout = $_POST['timeout'];
$descrizione = $_POST['descrizione'];
$query = "INSERT INTO settings (arduinoid, lineaproduzione, timeout,descrizione)
VALUES ($arduinoid,$lineaproduzione,$timeout,'$descrizione')";
if(mysql_query($query)){
echo "updated";}
else{
echo "fail";}
?>