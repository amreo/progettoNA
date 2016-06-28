<?php
mysql_connect("127.0.0.1", "root", "PASSWORD") or die("Connection Failed");
mysql_select_db("dati_produzione")or die("Connection Failed");
$lineaproduzione = $_POST['lineaproduzione'];
$barcode = $_POST['barcode'];
$nome = $_POST['nome'];
$descrizione = $_POST['descrizione'];
$query = "UPDATE dati_produzione.output_catena SET ID_prodotto = $barcode, Nome = $nome, Descrizione = $descrizione,WHERE Linea = $lineaproduzione;";
if(mysql_query($query)){
echo "updated";}
else{
echo "fail";}
?>