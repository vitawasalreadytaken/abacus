#include <sqlite3.h>

#include "db.h"
#include "exceptions.h"


void Db::open() {
	int ret = sqlite3_open(path.c_str(), &handle);

	if (ret != SQLITE_OK) {
		throw Exception(Exception::DB_ERROR, sqlite3_errmsg(handle));
	}
}

void Db::close() {
	if (handle != NULL) {
		sqlite3_close(handle);
		#if (SQLITE_VERSION_NUMBER > 3006000)
			sqlite3_shutdown();
		#endif
		handle = NULL;
	}
}

void Db::commit(unsigned long int counter) {
	open();

	Vita::string sql = "INSERT INTO counter(stamp, value) VALUES(DATETIME('now', 'localtime'), ";
	sql += Vita::string::toStr<unsigned long int>(counter) + ")";

	int ret = sqlite3_exec(handle, sql.c_str(), NULL, NULL, NULL);
	if (ret != SQLITE_OK) {
		throw Exception(Exception::DB_ERROR, sqlite3_errmsg(handle));
	}

	close();
}
