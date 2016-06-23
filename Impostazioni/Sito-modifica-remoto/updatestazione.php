<<<<<<< HEAD:Impostazioni/Sito Modifica Remoto/updatestazione.php
<?php include("radioButton.php");
session_start();
$conn_string = "host=127.0.0.1 port=3360 dbname=dati_produzione.settings user=root password=PASSWORD";
$conn = pg_connect($conn_string);

$idstazione = $_POST['idstazione'];
$lineaproduzione = $_POST['lineaproduzione'];
$timeout = $_POST['timeout'];

$_SESSION["success"] = 0;


$query = "UPDATE dati_produzione.settings SET Lineaproduzione = $lineaproduzione, Barcodetimeout = $timeout WHERE IDstazione = $idstazione;";

if (pg_query($conn, $query))
{
	echo "<script language=javascript>window.location.href='homepage.php';</script>"; ;
  	$_SESSION["success0"] = 1;
} else {	
	echo "Operazione fallita";}
}
?>
=======
<?php

mysql_connect("127.0.0.1", "root", "PASSWORD") or die("Connection Failed");

mysql_select_db("dati_produzione")or die("Connection Failed");

$idstazione = $_POST['idstazione'];

$lineaproduzione = $_POST['lineaproduzione'];

$timeout = $_POST['timeout'];



$query = "UPDATE dati_produzione.settings SET Lineaproduzione = $lineaproduzione, Barcodetimeout = $timeout WHERE IDstazione = $idstazione;";
if(mysql_query($query)){
}
else{
echo "Operazione fallita";}
?>
>>>>>>> master:Impostazioni/Sito-modifica-remoto/updatestazione.php
