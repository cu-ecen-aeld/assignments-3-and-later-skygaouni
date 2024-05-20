#!/bin/sh

### BEGIN INIT INFO
# Provides:          aesdsocket
# Required-Start:    $remote_fs $syslog
# Required-Stop:     $remote_fs $syslog
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: AESD Socket Daemon
# Description:       This script starts and stops the AESD Socket Daemon.
### END INIT INFO

# Change the following variables according to your setup
AEDS_SOCKET_DIR="."
AEDS_SOCKET_EXEC="./aesdsocket"

# Load the functions from /etc/init.d/functions (if available)
[ -r /etc/init.d/functions ] && . /etc/init.d/functions

start() {
    echo -n "Starting aesdsocket: "
    start-stop-daemon --start --quiet --exec aesdsocket -- -d
    echo "aesdsocket started."
}

stop() {
    echo -n "Stopping aesdsocket: "
    start-stop-daemon --stop --quiet --retry=TERM/30/KILL/5 --exec aesdsocket
    echo "aesdsocket stopped."
}

case "$1" in
    start)
        start
        ;;
    stop)
        stop
        ;;
    restart)
        stop
        start
        ;;
    *)
        echo "Usage: $0 {start|stop|restart}"
        exit 1
esac

exit 0
