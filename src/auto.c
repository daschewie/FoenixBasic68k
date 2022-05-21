#include "auto.h"

static bool _auto_enabled = false;
static int _auto_cur = 0;
static int _auto_step = 0;

void auto_on(int start, int step) {
    _auto_cur = start;
    _auto_step = step;
    _auto_enabled = true;
}

int auto_line() {
    return _auto_cur;
}

int auto_inc() {
    _auto_cur += _auto_step;
}

int auto_off() {
    _auto_enabled = false;
}

bool auto_enabled() {
    return _auto_enabled;
}