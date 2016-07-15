<?php
	session_start();
	if($_SESSION["autorizzato"] == 0)
	{
		echo '<script language=javascript>window.location.href="nonautorizzato.html"</script>';
	}
?>

<html>
	<link rel="stylesheet" href="stile.css" type="text/css" />
	<form method="post" name="Update" action="updatestazione.php" />
		<h3>Stampa</h3>
		
		<p>Prodotto<p>
		<input type="text" name="idstazione" />
		<br></br>
		<input type="submit" name="Print" value="Stampa" href="stampa.php"/>
		<br></br>
		<a href="homepage.php">Home</a>
	</form> 
</html>
