<?php
session_start();
//se non c'è la sessione registrata
if (!session_is_registered('autorizzato')) {
  echo "<h1>Area riservata, accesso negato.</h1>";
  echo "Per effettuare il login clicca <a href='index.php'><font color='blue'>qui</font></a>";
  die;
}
 
//Altrimenti Prelevo il codice identificatico dell'utente loggato
session_start();
$cod = $_SESSION['cod']; //id cod recuperato nel file di verifica
?>

<html>
<link rel="stylesheet" href="stile.css" type="text/css" />
<form method="post" name="Update" action="update.php" />
  <h3>Impostazioni</h3>
<p>ID Stazione<p>
 
<input type="text" name="idstazione" />
 
 
<p>Linea Produzione<p>
 
<input type="text" name="lineaproduzione" />

<p>Timeout Barcode<p>
 
<input type="text" name="timeout" />

  

  
  <br></br>

 
<input type="submit" name="Submit" value="Update" />

<br></br>
<a href="logout.php">Logout</a>
</form>


 
 
</html>