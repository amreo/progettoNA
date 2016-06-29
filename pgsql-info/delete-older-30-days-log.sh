#!/bin/sh
/usr/bin/psql -qd dati_produzione -c "DELETE FROM dati_produzione.log_eventi WHERE Date < DATE_SUB( CURRENT_TIME(), INTERVAL 30 DAY )"
/usr/bin/psql -qd dati_produzione -c "DELETE FROM dati_produzione.log_produzione WHERE Date < DATE_SUB( CURRENT_TIME(), INTERVAL 30 DAY )"
