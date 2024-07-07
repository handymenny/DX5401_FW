#!/bin/sh /etc/rc.common

START=51

export PATH=/home/bin:/home/scripts:/opt/bin:/bin:/sbin:/usr/bin:/usr/sbin
export LD_LIBRARY_PATH=/lib/public:/lib/private:/usr/lib:/lib:/usr/lib/ebtables:/usr/lib/iptables

EXEC_NAME=rport
EXEC_PATH=/usr/sbin
EXEC_CONF=/etc/rport.conf
DATA_DIR=/var/rport/

start_rport()
{
	if [ ! -f /etc/machine-id ] ; then
		uuidgen > /etc/machine-id
	fi
	PID=$(pgrep $EXEC_NAME)
	if [ "$PID" == "" ] ; then
		$EXEC_PATH/$EXEC_NAME -c $EXEC_CONF &
	else
		echo 'rport is already running !!!'
	fi
}

stop_rport()
{
	killall -9 $EXEC_NAME
}

start()
{
	start_rport
}

stop()
{
	stop_rport
}

restart()
{
	stop_rport
	start_rport
}
