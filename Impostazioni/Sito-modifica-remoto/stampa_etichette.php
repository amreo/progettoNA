	<?php
		session_start();
		if($_SESSION["autorizzato"] == 0)
		{
			echo '<script language=javascript>window.location.href="nonautorizzato.html"</script>';
		}
	?>
	
	<html>
		<link rel="stylesheet" href="stile.css" type="text/css" />
		<form method="post" name="Update" action="stampa.php" />
			<h3>Stampa</h3>
			
			<p>Prodotto<p>
			<select name="prodotti">
	   			<option value="Mouse">Mouse</option>
	   			<option value="Alimentatore">Alimentatore</option>
	   			<option value="Tastiera">Tastiera</option>
	  		</select>
			<br></br>
			<input type="submit" name="Update" value="Stampa"/>
			<br></br>
			<a href="homepage.php">Home</a>
		</form> 
	</html>
