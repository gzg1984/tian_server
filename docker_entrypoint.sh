#!/bin/sh
#Start Mysql via the ubuntu_dev script
mysql_count=`ps aux|grep mysql|grep -v grep|wc -l`
if [ "$mysql_count" = 0 ]
then
#echo "Change the privilage of mysql folders..."
#chown -R mysql:mysql /var/lib/mysql /var/run/mysqld
sleep 3
echo "Starting Mysql..."
service mysql start
sleep 3
fi

#Start sshd server
echo "Starting sshd..."
service ssh start

#Start My Game Server and never quit
# Others can only access this docker via ssh
echo "Starting ServerApp..."
/tian_server/ServerApp
