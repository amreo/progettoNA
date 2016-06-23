<?php
mysql_connect("127.0.0.1", "root", "PASSWORD") or die("Connection Failed");

mysql_select_db("dati_produzione")or die("Connection Failed");


$idstazione = $_POST['idstazione'];
$descrizione = $_POST['descrizione'];
$idprodotto = $_POST['idprodotto'];
$nome = $_POST['nome'];

$query = "INSERT INTO dati_produzione.output_catena (IDstazione, ID_prodotto, Nome,Descrizione,numProdotti)
VALUES ($idstazione,$idprodotto,$nome,$descrizione,0)";

if(mysql_query($query))
{
echo "updated";}

else{
echo "fail";}
?>
