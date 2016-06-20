<?php
mysql_connect("Host Name", "User Name", "User Password") or die("Connection Failed");
mysql_select_db("DataBase Name")or die("Connection Failed");
$arduinoip = $_POST['arduinoip'];
$lineaproduzione = $_POST['lineaproduzione'];
$timeout = $_POST['timeout'];
$query = "UPDATE test SET lineaproduzione = '$lineaproduzione' SET timeout = '$timeout' WHERE name = '$arduinoip'";
if(mysql_query($query)){
echo "updated";}
else{
echo "fail";}
?>