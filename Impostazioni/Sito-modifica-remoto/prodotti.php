<?php
	session_start();
	if($_SESSION["autorizzato"] == 0)
	{
		echo '<script language=javascript>window.location.href="nonautorizzato.html"</script>';
	}
?>

<html>
	<link rel="stylesheet" href="stileprodotto.css" type="text/css" /> 
	<form method="post" name="Salva" action="nuovoprodotto.php">
		<h3>Nuovo Prodotto</h3>
		<p>Linea di produzione<p>
		<input type="text" name="linea" />
		<p>Barcode Prodotto<p>
		<input type="text" name="idprodotto" />
		<p>Nome Prodotto<p>
		<input type="text" name="nome" />
		<p>Descrizione Prodotto<p>
 		<input type="text" name="descrizione" />
		<br></br>
		<input type="submit" name="Submit" value="Salva" />
		<br></br>
		<a href="logout.php">Logout</a>
	</form> 
</html>
