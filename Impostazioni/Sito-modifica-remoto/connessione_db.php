<?php include("config.php");//includo file configurazione per recupero variabili
$connessione = mysql_connect("$host","$db_user","$db_psw");
if(!$connessione) { die("Errore critico di Connessione al Database" . mysql_error()); } ?>
echo "ciao e test";
