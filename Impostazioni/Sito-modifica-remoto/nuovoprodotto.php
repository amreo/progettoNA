<?php include("radioButton.php");

session_start();

$conn_string = "host=127.0.0.1 port=3360 dbname=dati_produzione.output_catena user=root password=PASSWORD";
$conn = pg_connect($conn_string);
$idstazione = $_POST['linea'];
$descrizione = $_POST['descrizione'];
$idprodotto = $_POST['idprodotto'];
$nome = $_POST['nome'];

$_SESSION["success3"]= 0;

$query = "INSERT INTO dati_produzione.output_catena (Linea, ID_prodotto, Nome,Descrizione,numProdotti)
VALUES ($idstazione,$idprodotto,'$nome','$descrizione',0)";
if(pg_query($conn, $query))
{
	echo "<script language=javascript>window.location.href='homepage.php';</script>"; ;
	$_SESSION["success3"]= 1;
} else{
	echo "Operazione fallita";
}

?>
