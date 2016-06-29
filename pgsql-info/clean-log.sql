TRUNCATE log_eventi; -- elimina dati
INSERT INTO log_eventi (Linea, Posizione, Info)
VALUES (Linea, -1, 'Sono stati rimossi tutti i log degli eventi');
