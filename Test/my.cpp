#include "stdafx.h"
#include <string.h>
#include "my.h"

static callback c;

int myfunc(char *name) {
	(*c)(const_cast<char*>("received"), name);
	return strlen(name);
}

void registerCallback(callback myc) {
	c = myc;
}