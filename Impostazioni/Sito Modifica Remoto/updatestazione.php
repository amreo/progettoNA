<?php include("radioButton.php");
$conn_string = "host=127.0.0.1 port=3360 dbname=dati_produzione.settings user=root password=PASSWORD";
$conn = pg_connect($conn_string);

$idstazione = $_POST['idstazione'];


$lineaproduzione = $_POST['lineaproduzione'];


$timeout = $_POST['timeout'];



$query = "UPDATE dati_produzione.settings SET Lineaproduzione = $lineaproduzione, Barcodetimeout = $timeout WHERE IDstazione = $idstazione;";

if (pg_query($conn, $query))
{
	echo "<script language=javascript>window.location.href='homepage.php';</script>"; ;
  	$d = 1;
} else {	
	echo "Operazione fallita";}
}
?>
