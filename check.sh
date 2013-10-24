#!/bin/bash

fail() {
	echo
	echo Error: $1 not found. That would be a problem...
	echo
	exit 1
}

warn() {
	echo Warning: $1 not found. It\'s not required, though...
}

echo Checking for libsqlite3...
if pkg-config --libs --cflags sqlite3 >/dev/null; then
	echo OK.
else
	fail libsqlite3
fi

echo
echo Checking for gnuplot...
if [ -z "$(which gnuplot)" ]; then
	warn gnuplot
else
	echo OK.
fi

exit 0
