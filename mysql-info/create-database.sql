CREATE DATABASE dati_produzione; -- Crea il database
CREATE TABLE dati_produzione.output_catena 
(
	ID_prodotto INT NOT NULL UNIQUE, 	-- barcode
	Nome VARCHAR(40),			-- nome prodotto
	Descrizione VARCHAR(255),		-- descrizione del prodotot
	numProdotti INT, 			-- numero di prodotto prodotti
	
	PRIMARY KEY(ID_prodotto)	
); -- Tabella di output

CREATE TABLE dati_produzione.log_eventi
(
	ID INT NOT NULL AUTO_INCREMENT,
	Posizione INT,			-- posizione dell'ultimo pacchetto ricevuto nell'ordine di produzione
	Date TIMESTAMP,			-- data dell'evento
	Info VARCHAR(255),		-- descrizione dell'evento

	PRIMARY KEY(ID)	
);

