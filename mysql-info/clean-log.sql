TRUNCATE TABLE dati_produzione.log_eventi; -- elimina dati
INSERT INTO dati_produzione.log_eventi (Posizione, Info)
VALUES (-1, "Sono stati rimossi tutti i log degli eventi");
