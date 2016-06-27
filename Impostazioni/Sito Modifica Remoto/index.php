<?php include('config.php'); ?>
<!DOCTYPE html>
<html>
<head>
 
    <title>Area Riservata - <?php echo $sito_internet ?></title>
 
  
    <link href="index.css" rel="stylesheet" type="text/css" />
 
</head>
<body>
   <h3>Inserisci le credenziali per continuare</h3>
    <form id="login" action="verifica.php" method="post">
        <fieldset id="inputs">
            <input id="username" name="username" type="text" placeholder="Username" autofocus required>
            <input id="password" name="password" type="password" placeholder="Password" required>
        </fieldset>
        <fieldset id="actions">
            <input type="submit" id="submit" value="Collegati">
            
        </fieldset>
    </form>
 
</body>
</html>
