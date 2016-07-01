<?php
	session_start();
	if($_SESSION["autorizzato"] == 0)
	{
		echo '<script language=javascript>window.location.href="nonautorizzato.html"</script>';
	}
?>

<html>
	<link rel="stylesheet" href="stilenuova.css" type="text/css" />
	<form method="post" name="Salva" action="nuovastazione.php">
		<h3>Nuova Stazione</h3>
		<p>ID Stazione<p>
		<input type="text" name="idstazione" />
		<p>Linea Produzione<p>
		<input type="text" name="lineaproduzione" />
		<p>Timeout Barcode<p>
		<input type="text" name="timeout" />
		<br></br>
		<input type="submit" name="Submit" value="Salva" />
		<br></br>
		<a href="logout.php">Logout</a>
	</form>
</html>
