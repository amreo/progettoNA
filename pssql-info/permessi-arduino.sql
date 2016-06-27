CREATE USER arduino WITH PASSWORD 'arduino4you';
GRANT INSERT ON log_eventi TO arduino;
GRANT SELECT, UPDATE ON output_catena TO arduino;
GRANT SELECT ON settings TO arduino;
