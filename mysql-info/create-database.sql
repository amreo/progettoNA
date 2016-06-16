CREATE DATABASE dati_produzione; -- Crea il database
CREATE TABLE dati_produzione.output_catena 
(
	ID_prodotto INT NOT NULL UNIQUE, 	-- barcode
	Nome VARCHAR(40),			-- nome prodotto
	Descrizione VARCHAR(255),		-- descrizione del prodotto
	numProdotti INT, 			-- quantità prodotta
	
	PRIMARY KEY(ID_prodotto)	
); -- Tabella di output
CREATE TABLE dati_produzione.log_eventi
(
	ID INT NOT NULL AUTO_INCREMENT,
	Linea INT,			-- Linea di produzione del contatore
	Posizione INT,			-- posizione dell'ultimo pacchetto ricevuto nell'ordine di produzione
	Date TIMESTAMP,			-- data dell'evento
	Info VARCHAR(255),		-- descrizione dell'evento

	PRIMARY KEY(ID)	
);
CREATE TABLE dati_produzione.contatori_dati
(
	ID INT NOT NULL,		-- Linea di produzione 
	ID_prodotto INT NOT NULL,	-- ID del prodotto
	numProdotti INT			-- Quantità prodotta
);

INSERT INTO dati_produzione.output_catena (ID_prodotto, Nome, Descrizione, numProdotti)
VALUES (1, "Fallita_Lettura_Barcode", "Questo prodotti indica il numeri di prodotti in cui il barcode non è stato letto", 0);
INSERT INTO dati_produzione.log_eventi (Posizione, Info)
VALUES (-1, "sono stati inseriti i dati di default nel server");

