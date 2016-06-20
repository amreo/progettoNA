<?php
mysql_connect("Host Name", "User Name", "User Password") or die("Connection Failed");
mysql_select_db("DataBase Name")or die("Connection Failed");
$arduinoid = $_POST['arduinoid'];
$lineaproduzione = $_POST['lineaproduzione'];
$timeout = $_POST['timeout'];
$descrizione = $_POST['descrizione'];
$query = "UPDATE nomedatabase SET lineaproduzione = '$lineaproduzione' timeout = '$timeout' descrizione = '$descrizione' WHERE name = '$arduinoid'";
if(mysql_query($query)){
echo "updated";}
else{
echo "fail";}
?>
