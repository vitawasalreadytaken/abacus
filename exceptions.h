#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <errno.h>
#include <string.h>

#include "string.hpp"


class Exception {
	protected:
		int code;
		Vita::string error;

	public:
		static const int INIT_ERROR = -1;
		static const int READ_ERROR = -2;
		static const int CONFIG_NOT_FOUND = -3;
		static const int DB_ERROR = -4;
		static const int DAEMON_ERROR = -5;

		Exception(int acode, Vita::string aerror): code(acode), error(aerror) {};
		int getCode() { return code; };
		Vita::string getStrError() { return error; };
};

class IOException : public Exception {
	public:
		IOException(): Exception(errno, strerror(errno)) {};
};

#endif
