CREATE USER arduino WITH PASSWORD 'arduino4you';
GRANT INSERT ON dati_produzione.log_eventi TO arduino;
GRANT SELECT, UPDATE ON dati_produzione.output_catena TO arduino;
GRANT SELECT ON dati_produzione.settings TO arduino;
