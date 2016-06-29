
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
