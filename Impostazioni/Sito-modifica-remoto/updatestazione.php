<?php
	session_start();
	mysql_connect("127.0.0.1", "root", "PASSWORD") or die("Connection Failed");
	mysql_select_db("dati_produzione")or die("Connection Failed");
	
	$idstazione = $_POST['idstazione'];
	$lineaproduzione = $_POST['lineaproduzione'];
	$timeout = $_POST['timeout'];

	$_SESSION["success"] = 0;

	$query = "UPDATE dati_produzione.settings SET Lineaproduzione = $lineaproduzione, Barcodetimeout = $timeout WHERE IDstazione = $idstazione;";
	if(mysql_query($query)){
		echo "<script language=javascript>window.location.href='homepage.php';</script>";
 		$_SESSION["success0"] = 1;
	} else {
		echo "Operazione fallita";
	}
?>
