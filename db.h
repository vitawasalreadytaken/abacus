#ifndef DB_H
#define DB_H

#include <sqlite3.h>

#include "string.hpp"


class Db {
	protected:
		Vita::string path;
		sqlite3* handle;
		void open();
		void close();

	public:
		Db(Vita::string path): path(path) {};
		void commit(unsigned long int counter);
};

#endif
