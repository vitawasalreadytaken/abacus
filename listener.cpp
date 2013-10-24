#include <linux/input.h>

#include "abacus.h"
#include "exceptions.h"
#include "listener.h"

Listener::Listener(Vita::string path) {
	device = fopen(path.c_str(), "r");
	if (device == NULL) {
		throw IOException();
	}
}

Listener::~Listener() {
	if (device != NULL) {
		fclose(device);
	}
}

void Listener::read(bool countRepeats) {
	if (device == NULL) {
		throw Exception(Exception::INIT_ERROR, "Device not initialized");
	}

	struct input_event event;

	while (THE_WORLD_IS_NOT_COMING_TO_AN_END) {
		if (fread(&event, sizeof(event), 1, device) != 1) {
			throw Exception(Exception::READ_ERROR, "Error reading the device");
		}

		if (event.type == EV_KEY && (event.value == KEY_PRESS || (countRepeats && event.value == KEY_REPEAT))) {
			return;
		}
	}
}
