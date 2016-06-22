<?php include("radioButton.php");
if ($mysql_status = 'checked')
{

mysql_connect("127.0.0.1", "root", "PASSWORD") or die("Connection Failed");


mysql_select_db("dati_produzione")or die("Connection Failed");


$idstazione = $_POST['idstazione'];


$lineaproduzione = $_POST['lineaproduzione'];


$timeout = $_POST['timeout'];



$query = "UPDATE dati_produzione.settings SET Lineaproduzione = $lineaproduzione, Barcodetimeout = $timeout WHERE IDstazione = $idstazione;";

if(mysql_query($query)){

echo "updated";
}
else{
echo "fail";
}


}

if ($pgsql_status = 'checked')
{

$conn_string = "host=127.0.0.1 port=3360 dbname=dati_produzione.settings user=root password=PASSWORD";
$conn = pg_connect($conn_string);

$idstazione = $_POST['idstazione'];


$lineaproduzione = $_POST['lineaproduzione'];


$timeout = $_POST['timeout'];



$query = "UPDATE dati_produzione.settings SET Lineaproduzione = $lineaproduzione, Barcodetimeout = $timeout WHERE IDstazione = $idstazione;";

pg_query($conn, $query);
}

?>
