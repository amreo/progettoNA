CREATE TABLE settings (

arduinoid INTEGER,
lineaproduzione INTEGER,
barcodetimeout INTEGER,
descrizione TEXT,
);

INSERT INTO settings (arduinoid, lineaproduzione, barcodetimeout,descrizione)
VALUES ('arduino','arduino4you',3306,'192.168.1.101',3,'11 22 33 44 55 66', '192.168.1.105', 1, 5000);
