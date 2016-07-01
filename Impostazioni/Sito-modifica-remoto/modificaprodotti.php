<?php
	session_start();
	if($_SESSION["autorizzato"] == 0)
	{
		echo '<script language=javascript>window.location.href="nonautorizzato.html"</script>';
	}
?>

<html>
	<link rel="stylesheet" href="stilemodificaprodotto.css" type="text/css" />
	<form method="post" name="Update" action="updateprodotto.php">
		<h3>Impostazioni</h3>
		<p>Linea Produzione<p>
 		<input type="text" name="lineaproduzione" />
		<p>Barcode Prodotto<p>
		<input type="text" name="barcode" />
		<p>Nome Prodotto<p>
 		<input type="text" name="nome" />
		<p>Descrizione Prodotto<p>
		<input type="text" name="descrizione" />
		<br></br>
		<input type="submit" name="Submit" value="Update" />
		<br></br>
		<a href="logout.php">Logout</a>
	</form> 
</html>
