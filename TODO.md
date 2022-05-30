# TODO
The following routines are extensions to the Original RetroBASIC project

## MATH
- [ ] Hardware accelerated Floats

## File IO
- [x] `BLOAD "FILENAME", &HFFFFFFFF`
- [x] `OPEN #1, "O", "FILENAME"`
- [x] `CLOSE #1`
- [ ] `EOF(1)`
- [x] `WRITE #1, string-expression [;]`
- [ ] `READLN #1, line$`

## DOS Commands
- [x] `DIR`
- [x] `CHDIR "path"`
- [x] `DELETE "path"`

## Memory Routines
8 Bit Access
- [x] `n = PEEK(&HFFFFFFFF)`
- [x] `POKE &HFFFFFFFF, &HFF `

16 Bit Access
- [x] `n = PEEKW(&HFFFFFFFF)`
- [x] `POKEW &HFFFFFFFF, &HFFFF`

32 Bit Access
- [x] `n = PEEKL(&HFFFFFFFF)`
- [x] `POKEL &HFFFFFFFF, &HFFFFFFFF`

Block Copies
- [ ] `MEMCOPY LINEAR &h010000, 1024 TO RECT &hB00000,32,32,640`

## Clock
- [x] `SETDATE <year>, <month>, <day>`
- [x] `SETTIME <hour>, <minute>, <second>`
- [x] `DATE$()`
- [x] `TIME$()`
- [x] `TICKS()`

## Video
BitMaps
- [ ] `BITMAP <plane>, <visible>, <lut> [, <address>]`
- [ ] `CLRBITMAP <plane>`
- [ ] `FILL <plane>, <x0>, <y0>, <x1>, <y1>, <color>`
- [ ] `LINE <plane>, <x0>, <y0>, <x1>, <y1>, <color>`
- [ ] `PLOT <plane>, <column>, <row>, <color>`

Colors
- [ ] `BGCOLOR <red>, <green>, <blue>`
- [ ] `BORDER <visible> [, <red>, <green>, <blue> ]`
- [ ] `PALETTE <lut>, <color>, <red>, <green>, <blue>`
- [x] `COLOR <foreground>, <background>`

Sprites
- [ ] `SPRITE <number>, <lut> [, <address> ]`
- [ ] `SPRITEAT <number>, <column>, <row>`
- [ ] `SPRITESHOW <number>, <visible> [, <layer> ]`

Tilemaps
- [ ] `TILEAT <number>, <x>, <y>`
- [ ] `TILESET <number>, <lut>, <is_square>, <address>`
- [ ] `TILESHOW <number>, <is_visible>`

Misc
- [x] `GRAPHICS <mode>`
- [x] `LOCATE <column>, <row>`

## Audio
- [ ] `VGM PLAY "FILENAME"`
- [ ] `VGM STOP`