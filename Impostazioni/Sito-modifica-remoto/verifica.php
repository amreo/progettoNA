<?php
	session_start(); //inizio la sessione
	//includo i file necessari a collegarmi al db con relativo script di accesso
	include("config.php"); 	
	include("connessione_db.php");
	
	$conn_string = "host=127.0.0.1 port=3360 dbname=dati_produzione user=root password=PASSWORD";
	$conn = pg_connect($conn_string);

	//variabili POST con anti sql Injection
	$username=pg_escape_string($_POST['username']); //faccio l'escape dei caratteri dannosi
	$password=pg_escape_string(sha1($_POST['password'])); //sha1 cifra la password anche qui in questo modo corrisponde con quella del db

	$query = "SELECT * FROM login WHERE username = '$username' AND password = '$password' ";
	$ris = pg_query($query, $conn) or die (pg_last_error());
	$riga=pg_fetch_array($ris);  

	/*Prelevo l'identificativo dell'utente */
	$cod=$riga['username'];

	/* Effettuo il controllo */
	if ($cod == NULL)
		$trovato = 0;
	else 
		$trovato = 1;  
s
	/* Username e password corrette */
	if($trovato == 1)
	{
		/*Registro la sessione*/
		//session_register("autorizzato");
		$_SESSION["autorizzato"] = 1;
		/*Registro il codice dell'utente*/
		$_SESSION['cod'] = $cod;

		/*Redirect alla pagina riservata*/
		echo "<script language=javascript>window.location.href='homepage.php';</script>"; 
	} else {
		/*Username e password errati, redirect alla pagina di login*/
		echo '<script language=javascript>window.location.href="index.php";</script>';
	}
?>
