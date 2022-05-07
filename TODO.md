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
- [ ] `SETDATE <day>, <month>, <year>`
- [ ] `SETTIME <hour>, <minute>, <second>`
- [ ] `GETDATE$()`
- [ ] `GETTIME$()`
- [ ] `TICKS()`

## Video
BitMaps
- [ ] `BITMAP <plane>, <visible>, <lut> [, <address>]`
- [ ] `CLRBITMAP <plane>`
- [ ] `FILL <plane>, <x0>, <y0>, <x1>, <y1>, <color>`
- [ ] `LINE <plane>, <x0>, <y0>, <x1>, <y1>, <color>`
- [ ] `PLOT <plane>, <column>, <row>, <color>`

Colors
- [ ] `SETBGCOLOR <red>, <green>, <blue>`
- [ ] `SETBORDER <visible> [, <red>, <green>, <blue> ]`
- [ ] `SETCOLOR <lut>, <color>, <red>, <green>, <blue>`
- [ ] `TEXTCOLOR <foreground>, <background>`

Sprites
- [ ] `SPRITE <number>, <lut> [, <address> ]`
- [ ] `SPRITEAT <number>, <column>, <row>`
- [ ] `SPRITESHOW <number>, <visible> [, <layer> ]`

Tilemaps
- [ ] `TILEAT <number>, <x>, <y>`
- [ ] `TILESET <number>, <lut>, <is_square>, <address>`
- [ ] `TILESET <number>, <lut>, <is_square>, <address>`
- [ ] `TILESHOW <number>, <is_visible>`

Misc
- [ ] `GRAPHICS <mode>`
- [ ] `LOCATE <column>, <row>`

## Audio
- [ ] `VGM PLAY "FILENAME"`
- [ ] `VGM STOP`