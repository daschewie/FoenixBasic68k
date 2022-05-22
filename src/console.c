#include "console.h"
#include "mcp/syscalls.h"

void console_border(uint8_t visible, short red, short green, short blue) {

}

void console_color(uint8_t foreground, uint8_t background) {
    sys_txt_set_color(0, foreground, background);
}

void console_locate(uint8_t row, uint8_t col) {
    sys_txt_set_xy(0, col, row);
}