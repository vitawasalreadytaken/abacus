#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "abacus.h"
#include "config.hpp"
#include "db.h"
#include "exceptions.h"
#include "listener.h"
#include "string.hpp"

Db* database;
unsigned long counter = 0;

void terminate(int signal) {
	database->commit(counter);
	delete database;
	exit(0);
}

void daemonize() {
	if (daemon(0, 0) != 0) {
		throw Exception(Exception::DAEMON_ERROR, "Failed to daemonize");
	}
}

int main(int argc, char** argv) {
	if (argc > 2) {
		std::cerr << "Usage: " << argv[0] << " [config file]\n";
		return 1;
	}

	try {
		Vita::string configPath(argc == 2 ? argv[1] : DEFAULT_CONFIG);

		Vita::config config;
		if (!config.loadFile(configPath)) {
			throw Exception(Exception::CONFIG_NOT_FOUND, "Failed to load configuration.");
		}

		if (config.get("daemonize").convert<bool>()) {
			daemonize();
		}

		Listener listener(config.get("device"));
		database = new Db(config.get("database"));
		unsigned int commitPeriod = config.get("commitPeriod").convert<unsigned int>();

		signal(SIGINT, terminate);
		signal(SIGTERM, terminate);

		while (THE_WORLD_IS_NOT_COMING_TO_AN_END) {
			listener.read(config.get("countRepeats").convert<bool>());
			counter++;

			if (commitPeriod > 0 && counter % commitPeriod == 0) {
				database->commit(counter);
				counter = 0;
			}
		}
	}
	catch (Exception e) {
		delete database;
		std::cerr << e.getStrError() << std::endl;
		return e.getCode();
	}
}
