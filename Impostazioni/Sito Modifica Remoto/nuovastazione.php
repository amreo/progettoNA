<?php include("radioButton.php");

$conn_string = "host=127.0.0.1 port=3360 dbname=dati_produzione.settings user=root password=PASSWORD";
$conn = pg_connect($conn_string);
$idstazione = $_POST['idstazione'];
$lineaproduzione = $_POST['lineaproduzione'];
$timeout = $_POST['timeout'];
$query = "INSERT INTO dati_produzione.settings (IDstazione, Lineaproduzione, Barcodetimeout)
VALUES ($idstazione,$lineaproduzione,$timeout)";

if(pg_query($conn, $query))
{
	echo "<script language=javascript>window.location.href='homepage.php';</script>"; ;
  	$b = 1;
} else{
	echo "Operazione fallita";
}
?>
