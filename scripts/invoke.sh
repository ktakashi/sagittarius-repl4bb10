# -*- mode:shell; coding:utf-8-unix; -*-
# NOT USED
SASH=./app/native/lib/sash

if [ -x $SASH ]; then
	$SASH -Dapp/native/modules -Lapp/native/scheme -d app/native/repl.scm
fi

