#!/bin/sh
apt-get update
apt-get upgrade -y
echo "[ProgettoNA] Installando mysql"
apt-get install -y mysql-server mysql-client
echo "[ProgettoNA] Installando apache2"
apt-get install -y apache2
echo "[ProgettoNA] Installando php5"
apt-get install -y php5 php5-mysql libapache2-mod-php5
echo "[ProgettoNA] Installando mono"
apt-key adv -y --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 3FA7E0328081BFF6A14DA29AA6A19B38D3D831EF
echo "deb http://download.mono-project.com/repo/debian wheezy main" | tee /etc/apt/sources.list.d/mono-xamarin.list
echo "deb http://download.mono-project.com/repo/debian wheezy-libjpeg62-compat main" | sudo tee -a /etc/apt/sources.list.d/mono-xamarin.list
apt-get update
apt-get install -y mono-complete
echo "[ProgettoNA] Installando pagine web di 'Impostazioni sito remoto'"
cp Impostazioni/Sito\ Modifica\ Remoto/* /var/www/html/
echo "[ProgettoNA] Installanzioni librerie per contaserver"
gacutil -i librerie-esterne/mysql-data-connector-cs/mysql-connector-net-6.9.8-noinstall/v4.5/MySql.Data.Entity.EF5.dll 
gacutil -i librerie-esterne/mysql-data-connector-cs/mysql-connector-net-6.9.8-noinstall/v4.5/MySql.Data.dll 
echo "[ProgettoNA] Compilazione contaserver"
xbuild contaserver/contaserver.sln
echo "[ProgettoNA] Installazione contaserver"
cp contaserver-conf/contaserver.conf /etc/contaserver.conf
chmod 500 /etc/contaserver.conf
cp contaserver/contaserver /etc/init.d/contaserver
chmod 755 /etc/init.d/contaserver 
mkdir /usr/bin/local
cp contaserver/bin/Debug/contaserver.exe /usr/bin/local/
chmod 755 /etc/init.d/contaserver 
echo "[ProgettoNA] Creazione database mysql"
cat mysql-info/create-database.sql | mysql -u root -p
cat mysql-info/permessi-arduino.sql | mysql -u root -p
