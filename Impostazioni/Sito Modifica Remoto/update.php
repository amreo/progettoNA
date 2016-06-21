<?php
mysql_connect("Host Name", "User Name", "User Password") or die("Connection Failed");
mysql_select_db("DataBase Name")or die("Connection Failed");
$idstazione = $_POST['idstazione'];
$lineaproduzione = $_POST['lineaproduzione'];
$timeout = $_POST['timeout'];
$descrizione = $_POST['descrizione'];
$query = "UPDATE test SET Lineaproduzione = '$lineaproduzione',Barcodetimeout = '$timeout',Descrizione= '$descrizione',WHERE IDStazione = '$idstazione'";
if(mysql_query($query)){
echo "updated";}
else{
echo "fail";}
?>
