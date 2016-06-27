<?PHP

$mysql_status = 'checked';
$pgsql_status = 'unchecked';

if (isset($_POST['Submit1'])) {

$selected_radio = $_POST['gender'];

if ($selected_radio = = 'mysql') {

$mysql_status = 'checked';

}
else if ($selected_radio = = 'pgsql') {

$pgsql_status = 'checked';

}

}

?>