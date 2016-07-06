<?php
	session_start();
	if($_SESSION["autorizzato"] == 0)
	{
		echo '<script language=javascript>window.location.href="nonautorizzato.html"</script>';
	}
	if($_SESSION["success0"] == 1 || $_SESSION["success1"] == 1 || $_SESSION["success2"] == 1 || $_SESSION["success3"] == 1)
	{
		echo "<script language=javascript>window.alert(\"Operazione completata con successo\");</script>";
		$_SESSION["success0"] = 0;
		$_SESSION["success1"] = 0;
		$_SESSION["success2"] = 0;
		$_SESSION["success3"] = 0;	  
	}
?>

<html>
	<head>
		<meta name="viewport" content="width=device-width, user-scalable=no">
		<style>
			body{
				background-color:#ebeaea;
			}
			.button {
				display: inline-block;
				border-radius: 4px;
				background-color: #f4511e;
				border: none;
				color: #FFFFFF;
				text-align: center;
				font-size: 28px;
				padding: 20px;
				width: 200px;
				transition: all 0.5s;
				cursor: pointer;
				margin: 5px;
			}
			.button span {
				cursor: pointer;
				display: inline-block;
				position: relative;
				transition: 0.5s;
			}
			.button span:after {
				content: '»';
				position: absolute;
				opacity: 0;
				top: 0;
				right: -20px;
				transition: 0.5s;
			}
			.button:hover span {
				padding-right: 25px;
			}
			.button:hover span:after {
				opacity: 1;
				right: 0;
			}
			  
			.button1 {
				display: inline-block;
				border-radius: 4px;
				background-color: #32CD32;
				border: none;
				color: #FFFFFF;
				text-align: center;
				font-size: 28px;
				padding: 20px;
				width: 200px;
				transition: all 0.5s;
				cursor: pointer;
				margin: 5px;
			}
			.button1 span {
				cursor: pointer;
				display: inline-block;
				position: relative;
				transition: 0.5s;
			}
			.button1 span:after {
				content: '»';
				position: absolute;
				opacity: 0;
				top: 0;
				right: -20px;
				transition: 0.5s;
			}
			.button1:hover span {
				padding-right: 25px;
			}
			.button1:hover span:after {
				opacity: 1;
				right: 0;
			}
			.button2 {
				display: inline-block;
				border-radius: 4px;
				background-color: #0073e6;
				border: none;
				color: #FFFFFF;
				text-align: center;
				font-size: 28px;
				padding: 20px;
				width: 200px;
				transition: all 0.5s;
				cursor: pointer;
				margin: 5px;
			}
			.button2 span {
				cursor: pointer;
				display: inline-block;
				position: relative;
				transition: 0.5s;
			}
			.button2 span:after {
				content: '»';
				position: absolute;
				opacity: 0;
				top: 0;
				right: -20px;
				transition: 0.5s;
			}
			.button2:hover span {
				padding-right: 25px;
			}
			.button2:hover span:after {
				opacity: 1;
				right: 0;
			}
			.button3 {
				display: inline-block;
				border-radius: 4px;
				background-color:  #ff9900;
				border: none;
				color: #FFFFFF;
				text-align: center;
				font-size: 28px;
				padding: 20px;
				width: 200px;
				transition: all 0.5s;
				cursor: pointer;
				margin: 5px;
			}
			.button3 span {
				cursor: pointer;
				display: inline-block;
				position: relative;
				transition: 0.5s;
			}
			.button3 span:after {
				content: '»';
				position: absolute;
				opacity: 0;
				top: 0;
				right: -20px;
				transition: 0.5s;
			}
			.button3:hover span {
				padding-right: 25px;
			}
			.button3:hover span:after {
				opacity: 1;
				right: 0;
			}
			.button4 {
				display: inline-block;
				border-radius: 4px;
				background-color:  #993399;
				border: none;
				color: #FFFFFF;
				text-align: center;
				font-size: 28px;
				padding: 20px;
				width: 200px;
				transition: all 0.5s;
				cursor: pointer;
				margin: 5px;
			}
			.button4 span {
				cursor: pointer;
				display: inline-block;
				position: relative;
				transition: 0.5s;
			}
			.button4 span:after {
				content: '»';
				position: absolute;
				opacity: 0;
				top: 0;
				right: -20px;
				transition: 0.5s;
			}
			.button4:hover span {
				padding-right: 25px;
			}
			.button4:hover span:after {
				opacity: 1;
				right: 0;
			}
			 
			.button5 {
				display: inline-block;
				border-radius: 4px;
				background-color:  #00cc7a;
				border: none;
				color: #FFFFFF;
				text-align: center;
				font-size: 28px;
				padding: 20px;
				width: 200px;
				transition: all 0.5s;
				cursor: pointer;
				margin: 5px;
			}
			.button5 span {
				cursor: pointer;
				display: inline-block;
				position: relative;
				transition: 0.5s;
			}
			.button5 span:after {
				content: '»';
				position: absolute;
				opacity: 0;
				top: 0;
				right: -20px;
				transition: 0.5s;
			}
			.button5:hover span {
				padding-right: 25px;
			}
			.button5:hover span:after {
				opacity: 1;
				right: 0;
			}
			.button6 {
				display: inline-block;
				border-radius: 4px;
				background-color:  #86592d;
				border: none;
				color: #FFFFFF;
				text-align: center;
				font-size: 28px;
				padding: 20px;
				width: 200px;
				transition: all 0.5s;
				cursor: pointer;
				margin: 5px;
			}
			.button6 span {
				cursor: pointer;
				display: inline-block;
				position: relative;
				transition: 0.5s;
			}
			.button6 span:after {
				content: '»';
				position: absolute;
				opacity: 0;
				top: 0;
				right: -20px;
				transition: 0.5s;
			}
			.button6:hover span {
				padding-right: 25px;
			}
			.button6:hover span:after {
				opacity: 1;
				right: 0;
			} 
			 .button7 {
				display: inline-block;
				border-radius: 4px;
				background-color:  #ac3939;
				border: none;
				color: #FFFFFF;
				text-align: center;
				font-size: 28px;
				padding: 20px;
				width: 200px;
				transition: all 0.5s;
				cursor: pointer;
				margin: 5px;
			}
			.button7 span {
				cursor: pointer;
				display: inline-block;
				position: relative;
				transition: 0.5s;
			}
			.button7 span:after {
				content: '»';
				position: absolute;
				opacity: 0;
				top: 0;
				right: -20px;
				transition: 0.5s;
			}
			.button7:hover span {
				padding-right: 25px;
			}
			.button7:hover span:after {
				opacity: 1;
				right: 0;
			} 
			.button8 {
				display: inline-block;
				border-radius: 4px;
				background-color:  #99cc00;
				border: none;
				color: #FFFFFF;
				text-align: center;
				font-size: 28px;
				padding: 20px;
				width: 200px;
				transition: all 0.5s;
				cursor: pointer;
				margin: 5px;
			}
			.button8 span {
				cursor: pointer;
				display: inline-block;
				position: relative;
				transition: 0.5s;
			}
			.button8 span:after {
				content: '»';
				position: absolute;
				opacity: 0;
				top: 0;
				right: -20px;
				transition: 0.5s;
			}
			.button8:hover span {
				padding-right: 25px;
			}
			.button8:hover span:after {
				opacity: 1;
				right: 0;
			} 
            		.button9 {
				display: inline-block;
				border-radius: 4px;
				background-color:  #CD853F;
				border: none;
				color: #FFFFFF ;
				text-align: center;
				font-size: 28px;
				padding: 20px;
				width: 200px;
				transition: all 0.5s;
				cursor: pointer;
				margin: 5px;
			}
			.button9 span {
				cursor: pointer;
				display: inline-block;
				position: relative;
				transition: 0.5s;
			}
			.button9 span:after {
				content: '»';
				position: absolute;
				opacity: 0;
				top: 0;
				right: -20px;
				transition: 0.5s;
			}
			.button9:hover span {
				padding-right: 25px;
			}
			.button9:hover span:after {
				opacity: 1;
				right: 0;
			}
			h2 {
				font-family: calibri;
			}
			#btn{
				font-family:calibri;
			}
			#logout{
				position: absolute;
				top: 85%;
			}
			#logout:link, #logout:visited {
				background-color: #9A9A9A;
				color: white;
				padding: 14px 25px;
				text-align: center;
				text-decoration: none;
				display: inline-block;
				font-family:calibri;
			}
			#logout:hover, #logout:active {
				background-color: grey;
			}
			#sinistra {
				position: absolute;
				top: 0%;
			}
			#centro {
				position: absolute;
				top: 0%;
				right: 56%;
			}
			#destra {
			position: absolute;
			top: 0%;
			right: 31%;
			}
			@media screen and (max-width: 1187px) {
				#logout{
					position: static;
					top: 85%;
				}
				#sinistra {
					position: static;
					top: 50%;
				}
				#centro {
					position: static;
					top: 0%;
					right: 56%;
				}
				#destra {
					position: static;
					top: 0%;
					right: 31%;
				}
			}
		</style>
	</head>
	<body>
		<section id="sinistra">
			<h2>Gestione Stazioni</h2>

			<a id="btn" href="impostazioni.php"><button class="button" style="vertical-align:middle"><span>Modifica</span></button></a>
			<br></br>
			<a id="btn" href="nuova.php"><button class="button1" style="vertical-align:middle"><span>Nuova</span></button></a>
			<br></br>
		</section>
		<section id="centro">
			<h2>Gestione Prodotti</h2>
			<a id="btn" href="modificaprodotti.php"><button class="button5" style="vertical-align:middle"><span>Modifica</span></button></a>
			<br></br>
			<a id="btn" href="prodotti.php"><button class="button2" style="vertical-align:middle"><span>Nuovo</span></button></a>
			<br></br>
            		<a id="btn" href="StampaEtichette.php"><button class="button9" style="vertical-align:middle"><span>Stampa etichette</span></button></a>
		    <br></br>
        </section>
		<section id="destra">
			<h2>Visualizza Database</h2>
			<a id="btn" href="produzione.php"><button class="button3" style="vertical-align:middle"><span>Prodotti</span></button></a>
			<br></br>
			<a id="btn" href="stazioni.php"><button class="button6" style="vertical-align:middle"><span>Stazioni</span></button></a>
			<br></br>
			<a id="btn" href="logeventi.php"><button class="button4" style="vertical-align:middle"><span>Log Eventi</span></button></a>
			<br></br>
			<a id="btn" href="logproduzione.php"><button class="button8" style="vertical-align:middle"><span>Log Produzione</span></button></a>
			<br></br>
			<a id="btn" href="graficoproduzione.php"><button class="button7" style="vertical-align:middle"><span>Grafico Prodotti</span></button></a>
		</section>
		<br></br>
		<a id="logout" href="logout.php">Logout</a>
	</body>
</html>
