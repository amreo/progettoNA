CREATE TABLE dati_produzione.output_catena 
(
	Linea INT NOT NULL,			-- Linea dove viene fatto il prodotto
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
CREATE TABLE dati_produzione.settings 
(
	IDstazione INTEGER UNIQUE NOT NULL,
	Lineaproduzione INTEGER NOT NULL,
	Barcodetimeout INTEGER NOT NULL,
	Successo BOOL
);
CREATE TABLE dati_produzione.log_produzione
(
	ID INT NOT NULL AUTO_INCREMENT,
	Linea INT,
	Date TIMESTAMP,
	Barcode INT,
	Successo BOOL,

	PRIMARY KEY(ID)
);
CREATE TABLE dati_produzione.etichette
(
        Nome VARCHAR(40),
        Barcode INT
);
-- parte relativa agli utenti autorizzati a accedere al sito

CREATE TABLE dati_produzione.login (
	id INT( 11 ) NOT NULL AUTO_INCREMENT ,
	username VARCHAR( 64 ) NOT NULL ,
	password VARCHAR( 64 ) NOT NULL ,
	PRIMARY KEY (id)
) ENGINE = MYISAM;
 
INSERT INTO dati_produzione.login (id, username, password)
VALUES (NULL, 'tecnicoautorizzato', sha1('pwddifficile'));

-- -------------------------

INSERT INTO dati_produzione.output_catena (ID_prodotto, Nome, Descrizione, numProdotti)
VALUES (1, "Fallita_Lettura_Barcode", "Questo prodotti indica il numeri di prodotti in cui il barcode non è stato letto", 0);
INSERT INTO dati_produzione.log_eventi (Linea, Posizione, Info)
VALUES (0, -1, "sono stati inseriti i dati di default nel server");

-- ------------------- Eventi
SET GLOBAL event_scheduler = ON;

CREATE EVENT cancella_vecchi_log_eventi
	ON SCHEDULE
		EVERY 1 DAY
	DO
		DELETE FROM dati_produzione.log_eventi WHERE Date < DATE_SUB( CURRENT_TIME(), INTERVAL 30 DAY );

CREATE EVENT cancella_vecchi_log_produzione
	ON SCHEDULE
		EVERY 1 DAY
	DO
		DELETE FROM dati_produzione.log_produzione WHERE Date < DATE_SUB( CURRENT_TIME(), INTERVAL 30 DAY );


