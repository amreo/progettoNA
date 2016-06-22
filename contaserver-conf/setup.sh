#!/bin/bash
apt-get install mono mono-service
cp contaserver.conf /etc/contaserver.conf
cp ../contaserver/bin/contaserver.exe /usr/bin/local/contaserver.exe
cp ../contaserver/contaserver /etc/init.d/contaserver
chmod +x /etc/init.d/contaserver
chmod +x /usr/bin/local/contaserver.exe