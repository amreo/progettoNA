<?php 
	include("config.php");
	//includo file configurazione per recupero variabili
	$connessione = pg_connect("$host","$db_user","$db_psw");
	if(!$connessione) { 
		die("Errore critico di Connessione al Database" . mysql_error()); //TODO: sostituisce mysql_error con il corrispondente per pgsql
	} 
?>
