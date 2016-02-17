/*
 * CMPT.cpp
 *
 *  Created on: 2016年1月29日
 *      Author: peng
 */
#include "CMPT.h"

int CMPT::check_miui_version(void) {

	char propValue[92] = { 0 };
	int code = 0;
	int name = 0;
	code = __system_property_get("ro.miui.ui.version.name", propValue);
	name = __system_property_get("ro.miui.ui.version.code", propValue);

	if (code == 0 && name == 0) {
		return 0;
	}
	return 1;
}

