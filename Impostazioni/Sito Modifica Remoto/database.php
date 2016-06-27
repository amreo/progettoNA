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
<link rel="stylesheet" href="stiledatabase.css" type="text/css" />
<form name ="form1" method ="post" action ="radioButton.php">

<Input type = 'Radio' Name ='gender' value= 'mysql'
<?PHP print $mysql_status; ?>
>Male

<Input type = 'Radio' Name ='gender' value= 'pgsql' 
<?PHP print $pgsql_status; ?>
>Female

<P>
<Input type = "Submit" Name = "Submit1" VALUE = "Invio">

</form>
</html>