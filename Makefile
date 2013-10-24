# Installation targets
# Note: If you change these variables, you will need to edit the configuration file AND the /etc/init.d/abacus script!

BIN_TARGET = /usr/sbin
DB_TARGET = /var/lib/abacus
CONF_TARGET = /etc
INIT_TARGET = /etc/init.d



# File names

BIN = abacus
DB = abacus.db
CONF = abacus.conf
INIT = init.d-script



# Check, build, install, uninstall, clean

all: check build

check:
	./check.sh

build: abacus db
	@echo Done. Run \"make install\" as root to install.

install: all
	install $(BIN) -t $(BIN_TARGET)
	install --directory $(DB_TARGET)
	install --mode=0644 $(DB) -t $(DB_TARGET)
	install --mode=0644 $(CONF) -t $(CONF_TARGET)
	install $(INIT) $(INIT_TARGET)/abacus
	update-rc.d abacus defaults 99 01

uninstall:
	-rm $(BIN_TARGET)/$(BIN)
	-rm $(DB_TARGET)/$(DB)
	-rmdir $(DB_TARGET)
	-rm $(CONF_TARGET)/$(CONF)
	-rm $(INIT_TARGET)/abacus
	-update-rc.d abacus remove

clean:
	-rm *.o $(BIN) $(DB)


# The sources

abacus: main.o listener.o db.o string.o config.o abacus.h
	g++ -o $(BIN) $+ `pkg-config --libs sqlite3`

main.o: main.cpp exceptions.h

listener.o: listener.cpp listener.h exceptions.h

db.o: db.cpp db.h exceptions.h

string.o: string.cpp

config.o: config.cpp



# Database

db:
	sqlite3 $(DB) < schema.sql
