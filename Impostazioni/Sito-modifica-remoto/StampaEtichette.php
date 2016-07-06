<!DOCTYPE html>

<html lang="en">
    <head>
        <meta charset="utf-8" />
        <title>Progetto Nexapp</title>
    </head>
    <body>
       
      <form> 
         
         <legend>Selezionare l'oggetto di cui stampare l'etichetta</legend>
        <br>
            <select name="Oggetti" >
                <option value="Mouse">Mouse  </option>
                <option value="Alimentatore">Alimentatore  </option>
                <option value="Tastiera">Tastiera  </option>
            </select>
        
    </form> 
        <br>
        <form> 
         
         <legend>Selezionare il numero di fotocopie (6 etichette per ogni foglio)</legend>
        <br>
            <select name="Numero" >
                <option value="1">1  </option>
                <option value="2">2  </option>
                <option value="3">3  </option>
                <option value="3">4  </option>
                <option value="3">5  </option>
                <option value="3">6  </option>
                <option value="3">7  </option>
                <option value="3">8  </option>
                <option value="3">9  </option>
                <option value="3">10  </option>

            </select>
        
    </form>
        <br>
        <button type="button" onclick="alert('Fotocopie in coda di stampa')">Stampa</button>
        
    </body>
</html>
