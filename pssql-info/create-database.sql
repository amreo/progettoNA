CREATE DATABASE dati_produzione; -- Crea il database
CREATE TABLE dati_produzione.output_catena 
(
	ID_prodotto INTEGER NOT NULL UNIQUE PRIMARY KEY, 	-- barcode
	Nome VARCHAR(40),					-- nome prodotto
	Descrizione VARCHAR(255),			-- descrizione del prodotto
	numProdotti INTEGER 				-- quantità prodotta	
); -- Tabella di output
CREATE TABLE dati_produzione.log_eventi
(
	ID INTEGER NOT NULL PRIMARY KEY,
	Linea INTEGER,			-- Linea di produzione del contatore
	Posizione INTEGER,		-- posizione dell'ultimo pacchetto ricevuto nell'ordine di produzione
	Date TIMESTAMP,			-- data dell'evento
	Info VARCHAR(255),		-- descrizione dell'evento

	PRIMARY KEY(ID)	
);
CREATE TABLE dati_produzione.contatori_dati
(
	ID INTEGER NOT NULL,		-- Linea di produzione 
	ID_prodotto INTEGER NOT NULL,	-- ID del prodotto
	numProdotti INTEGER			-- Quantità prodotta
);
CREATE TABLE dati_produzione.settings 
(
	IDstazione INTEGER UNIQUE NOT NULL,
	Lineaproduzione INTEGER NOT NULL,
	Barcodetimeout INTEGER NOT NULL,
	Descrizione TEXT
);

--parte relativa agli utenti autorizzati a accedere al sito

CREATE TABLE `login` (
`id` INTEGER( 11 ) NOT NULL AUTO_INCREMENT ,
`username` VARCHAR( 64 ) NOT NULL ,
`password` VARCHAR( 64 ) NOT NULL ,
PRIMARY KEY ( `id` )
) ENGINE = MYISAM;
 
INSERT INTO `login` (
`id` ,
`username` ,
`password`
)
VALUES (
NULL , 'tecnicoautorizzato', sha1( 'pwddifficile' )
);

---------------------------

INSERT INTO dati_produzione.output_catena (ID_prodotto, Nome, Descrizione, numProdotti)
VALUES (1, 'Fallita_Lettura_Barcode', 'Questo prodotti indica il numeri di prodotti in cui il barcode non è stato letto', 0);
INSERT INTO dati_produzione.log_eventi (Linea, Posizione, Info)
VALUES (0, -1, 'sono stati inseriti i dati di default nel server');



