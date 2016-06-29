INSERT INTO output_catena (ID_prodotto, Nome, Descrizione, numProdotti)
VALUES (1, 'Fallita_Lettura_Barcode', 'Questo prodotti indica il numeri di prodotti in cui il barcode non è stato letto', 0);
INSERT INTO log_eventi (Linea, Posizione, Info)
VALUES (0, -1, 'sono stati inseriti i dati di default nel server');
