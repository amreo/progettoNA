TRUNCATE dati_produzione.log_eventi; -- elimina dati
INSERT INTO dati_produzione.log_eventi (Linea, Posizione, Info)
VALUES (Linea, -1, 'Sono stati rimossi tutti i log degli eventi');
