<?php include("config.php");//includo file configurazione per recupero variabili
include("radioButton.php");
if ($mysql_status = 'checked')
{
$connessione = mysql_connect("$host","$db_user","$db_psw");

}

if ($pgsql_status = 'checked')
{

$conn = pg_connect("$host","$db_user","$db_psw");
}
 ?>
