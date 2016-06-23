<?php
session_start();
$_SESSION = array();
session_destroy(); //distruggo tutte le sessioni
 
//creo una varibiale con un messaggio
$msg = "Informazioni: log-out effettuato con successo.";
 
//la codifico via urlencode informazioni-logout-effettuato-con-successo
$msg = urlencode($msg); // invio il messaggio via get
 
//ritorno a index.php usando GET posso recuperare e stampare a schermo il messaggio di avvenuto logout
header("location: index.php?msg=$msg");
exit();
?>