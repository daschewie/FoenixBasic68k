#ifndef __VKY_CHAN_B_H
#define __VKY_CHAN_B_H

/** Master Control Register for Channel B, and its supported bits */
#define VKY3_B_MCR          ((volatile unsigned long *)0xFEC80000)
#define VKY3_B_MCR_TEXT     0x00000001  /**< Text mode enable bit */
#define VKY3_B_MCR_TXT_OVR  0x00000002  /**< Text overlay enable bit */
#define VKY3_B_MCR_GRAPHICS 0x00000004  /**< Graphics mode enable bit */
#define VKY3_B_MCR_BITMAP   0x00000008  /**< Bitmap engineg enable bit */
#define VKY3_B_MCR_TILE     0x00000010  /**< Tile engine enable bit */
#define VKY3_B_MCR_SPRITE   0x00000020  /**< Sprite engine enable bit */
#define VKY3_B_MCR_BLANK    0x00000080  /**< Disable display engine enable bit */
#define VKY3_B_MODE0        0x00000100  /**< Video Mode Bit 0 */
#define VKY3_B_MODE1        0x00000200  /**< Video Mode Bit 1 */
#define VKY3_B_DOUBLE       0x00000400  /**< Pixel Double Enable bit */
#define VKY3_B_MCR_SLEEP    0x00040000  /**< Monitor sleep (synch disable) bit */

/** Border control register for Channel B */
#define VKY3_B_BCR          ((volatile unsigned long *)0xFEC80004)
#define VKY3_B_BCR_ENABLE   0x00000001  /**< Bit to enable the display of the border */

/** Border color register for Channel B */
#define VKY3_B_BRDCOLOR     ((volatile unsigned long *)0xFEC80008)

/** Cursor Control Register for Channel B */
#define VKY3_B_CCR          ((volatile unsigned long *)0xFEC80010)
#define VKY3_B_CCR_ENABLE   0x00000001  /**< Bit to enable the display of the cursor */
#define VKY3_B_CCR_RATE0    0x00000002  /**< Bit0 to specify the blink rate */
#define VKY3_B_CCR_RATE1    0x00000004  /**< Bit1 to specify the blink rate */

/** Cursor Position Register for Channel B */
#define VKY3_B_CPR          ((volatile unsigned long *)0xFEC80014)

#define VKY3_B_BMAP_LAYER0_CTRL  ((volatile unsigned long *)0xFEC80100)
#define VKY3_B_BMAP_LAYER0_VRAM  ((volatile unsigned long *)0xFEC80104)

#define VKY3_B_BMAP_LAYER1_CTRL  ((volatile unsigned long *)0xFEC80108)
#define VKY3_B_BMAP_LAYER1_VRAM  ((volatile unsigned long *)0xFEC8010C)

/** Font memory block for Channel B */
#define VKY3_B_FONT_MEMORY  ((volatile unsigned char *)0xFEC88000)

/** Text Matrix for Channel B */
#define VKY3_B_TEXT_MATRIX  ((volatile unsigned char *)0xFECA0000)

/** Color Matrix for Channel B */
#define VKY3_B_COLOR_MATRIX ((volatile unsigned char *)0xFECA8000)

/* Text Color LUTs for Channel B */
#define VKY3_B_LUT_SIZE     16
#define VKY3_B_TEXT_LUT_FG  ((volatile unsigned long *)0xFECAC400)  /**< Text foreground color look up table for channel B */
#define VKY3_B_TEXT_LUT_BG  ((volatile unsigned long *)0xFECAC440)  /**< Text background color look up table for channel B */

#define VKY3_B_COLOR_LUT0   ((volatile unsigned long *)0xFEC82000)
#define VKY3_B_COLOR_LUT1   ((volatile unsigned long *)0xFEC82400)
#define VKY3_B_COLOR_LUT2   ((volatile unsigned long *)0xFEC82800)
#define VKY3_B_COLOR_LUT3   ((volatile unsigned long *)0xFEC82C00)
#define VKY3_B_COLOR_LUT4   ((volatile unsigned long *)0xFEC83000)
#define VKY3_B_COLOR_LUT5   ((volatile unsigned long *)0xFEC83400)
#define VKY3_B_COLOR_LUT6   ((volatile unsigned long *)0xFEC83800)
#define VKY3_B_COLOR_LUT7   ((volatile unsigned long *)0xFEC83C00)

void gfx_mode(uint32_t mode);
void gfx_bitmap(uint8_t plane, uint8_t visible, uint8_t lut, uint32_t address);
void gfx_clrbitmap(uint8_t plane);
void gfx_plot(uint8_t plane, int16_t x, int16_t y, uint8_t color);
void gfx_fill(uint8_t plane, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);
void gfx_line(uint32_t plane, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color);
void gfx_setcolor(uint8_t lut, uint8_t color, uint8_t red, uint8_t green, uint8_t blue);
void gfx_line4(uint32_t plane, uint32_t x1, uint32_t y1, uint32_t x2);

#endif
