/*
- [ ] `COLOR <foreground>, <background>`
- [ ] `BORDER <visible> [, <red>, <green>, <blue> ]`
*/
#include <stdint.h>

void console_border(uint8_t visible, short red, short green, short blue);
void console_color(uint8_t foreground, uint8_t background);
void console_locate(uint8_t row, uint8_t col);
