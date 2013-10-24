#ifndef LISTENER_H
#define LISTENER_H

#include <stdio.h>

#include "string.hpp"


class Listener {
	protected:
		FILE* device;
		static const int KEY_PRESS = 1;
		static const int KEY_RELEASE = 0;
		static const int KEY_REPEAT = 2;

	public:
		Listener(Vita::string path);
		~Listener();
		void read(bool countRepeats);
};
//
#endif
