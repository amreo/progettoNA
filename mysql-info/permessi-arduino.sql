CREATE USER 'arduino'@'127.0.0.1' IDENTIFIED BY 'arduino4you';
GRANT INSERT ON dati_produzione.log_eventi TO 'arduino'@'127.0.0.1';
GRANT SELECT, UPDATE ON dati_produzione.output_catena TO 'arduino'@'127.0.0.1';
GRANT SELECT ON dati_produzione.settings TO 'arduino'@'127.0.0.1';
