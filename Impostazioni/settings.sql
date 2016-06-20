CREATE TABLE settings (
username TEXT, 
password TEXT, 
serverport INTEGER,
serverip TEXT,
maxretry INTEGER,
mac TEXT,
localip TEXT,
lineaproduzione INTEGER,
barcodetimeout INTEGER,
);

INSERT INTO settings (username, password, serverport, serverip, maxretry, mac, localip, lineaproduzione, barcodetimeout)
VALUES ('arduino','arduino4you',3306,'192.168.1.101',3,'11 22 33 44 55 66', '192.168.1.105', 1, 5000);