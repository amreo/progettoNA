<?php
session_start();
mysql_connect("127.0.0.1", "root", "PASSWORD") or die("Connection Failed");
mysql_select_db("dati_produzione")or die("Connection Failed");
$lineaproduzione = $_POST['lineaproduzione'];
$barcode = $_POST['barcode'];
$nome = $_POST['nome'];
$descrizione = $_POST['descrizione'];
$_SESSION["success1"]= 0;
$query = "UPDATE dati_produzione.output_catena SET ID_prodotto = $barcode, Nome = '$nome', Descrizione = '$descrizione' WHERE Linea = $lineaproduzione;";
if(mysql_query($query)){
echo "<script language=javascript>window.location.href='homepage.php';</script>"; 
  $_SESSION["success1"]= 1;
}
else{
echo "Operazione fallita";}
?>
