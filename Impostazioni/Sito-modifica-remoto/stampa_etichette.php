<?php
	session_start();
	if($_SESSION["autorizzato"] == 0)
	{
		echo '<script language=javascript>window.location.href="nonautorizzato.html"</script>';
	}
?>

<html>
	<link rel="stylesheet" href="stilestampaetichette.css" type="text/css" />
	<form method="post" name="Stampa" action="stampa.php">
		<h3>Stampa</h3>
		<form> 
  			<select name='prodotti'>
				 
  			</select>
		<a href="logout.php">Logout</a>
	        </form> 
	</form> 
</html>
