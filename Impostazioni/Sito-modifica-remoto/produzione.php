<!DOCTYPE html >

<?php
session_start();
if($_SESSION["autorizzato"] == 0)
{
  echo '<script language=javascript>window.location.href="nonautorizzato.html"</script>';
  
}
?>

<html >
<head>
<meta charset="UTF-8">
 <link href="stileproduzione.css" rel="stylesheet" type="text/css">
<title></title>
</head>
 
<body>
<?php include("radioButton.php");

//connect to mysql server with host,username,password
$conn_string = "host=127.0.0.1 port=3360 dbname=dati_produzione.output_catena user=root password=PASSWORD";
$conn = pg_connect($conn_string);

//TODO: tradurrre mysql_query("SET character_set_results=utf8;") or die(mysql_error()); in pgsqllese

$query = "SELECT * FROM dati_produzione.output_catena";
pg_query($conn, $query);
 
 
//if we get any results we show them in table data
if(pg_num_rows($query)>0) {
 
?>
 
<table>
  <tr>
    <td align="center">ID Stazione</td>
    <td align="center">Barcode Prodotto</td>
    <td align="center">Nome Prodotto</td>
    <td align="center">Descrizione</td>
    <td align="center">Quantita'</td>
  </tr>
  <?php 
  // looping 
  while($row=pg_fetch_object($query)):?>
  <tr>
    <td align="center"><?php echo $row->IDstazione;  //row id ?></td>
    <td align="center"><?php echo $row->ID_prodotto; // row first name ?></td>
    <td align="center"><?php echo $row->Nome; //row last name  ?></td>
    <td align="center"><?php echo $row->Descrizione; //row contact number ?></td>
    <td align="center"><?php echo $row->numProdotti; //row created time ?></td>
  </tr>
  <?php endwhile;?>
</table>
<?php 
// no result show 
else: ?>
<h3>No Results found.</h3>
<?php endif; ?>



} 

<br></br>
<a href="logout.php">Logout</a>
</body>
</html>
=======


<!DOCTYPE html >
<html >
<head>
<meta charset="UTF-8">
 <link href="stileproduzione.css" rel="stylesheet" type="text/css">
<title></title>
</head>
 
<body>
 
<?php 
header('Content-Type: text/html; charset=utf-8');
//connect to mysql server with host,username,password
//if connection fails stop further execution and show mysql error
$connection=mysql_connect('localhost','root','PASSWORD') or die(mysql_error());
//select a database for given connection
//if database selection  fails stop further execution and show mysql error
mysql_select_db('dati_produzione',$connection) or die(mysql_error());
 
