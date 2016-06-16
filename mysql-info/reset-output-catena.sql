TRUNCATE TABLE dati_produzione.output_catena; -- elimina dati
INSERT INTO dati_produzione.output_catena (ID_prodotto, Nome, Descrizione, numProdotti)
VALUES (1, "Fallita_Lettura_Barcode", "Questo prodotti indica il numeri di prodotti in cui il barcode non è stato letto", 0);
INSERT INTO dati_produzione.log_eventi (Linea, Posizione, Info)
VALUES (0, -1, "I dati della tabella di output_catena sono stati resettati");
