# TODO
The following routines are extensions to the Original RetroBASIC project

## MATH
- [ ] Hardware accelerated Floats

## File IO
- [ ] `BLOAD "FILENAME", &HFFFFFFFF`
- [x] `OPEN #1, "O", "FILENAME"`
- [x] `CLOSE #1`
- [ ] `WRITECSV #1, "Hello", 10, 15, 100`
- [ ] `WRITELINE #1, "Hello World"`
- [ ] `READCSV #1, n$, a, b, c`
- [ ] `READLINE #1, line$`

## DOS Commands
- [x] `DIR`
- [x] `CHDIR "path"`
- [ ] `DEL "path"`


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

## Foenix
- [ ] `BITMAP <plane>, <visible>, <lut> [, <address>]`
- [ ] `CLRBITMAP <plane>`
- [ ] `FILL <plane>, <x0>, <y0>, <x1>, <y1>, <color>`
- [ ] `GRAPHICS <mode>`
- [ ] `LINE <plane>, <x0>, <y0>, <x1>, <y1>, <color>`
- [ ] `LOCATE <column>, <row>`
- [ ] `MEMCOPY <source> TO <destination>`
  - `MEMCOPY LINEAR &h010000, 1024 TO RECT &hB00000,32,32,640`

- [ ] `PLOT <plane>, <column>, <row>, <color>`
- [ ] `SETBGCOLOR <red>, <green>, <blue>`
- [ ] `SETBORDER <visible> [, <red>, <green>, <blue> ]`
- [ ] `SETDATE <day>, <month>, <year>`
- [ ] `SETCOLOR <lut>, <color>, <red>, <green>, <blue>`
- [ ] `SETTIME <hour>, <minute>, <second>`
- [ ] `SPRITE <number>, <lut> [, <address> ]`
- [ ] `SPRITEAT <number>, <column>, <row>`
- [ ] `SPRITESHOW <number>, <visible> [, <layer> ]`
- [ ] `TEXTCOLOR <foreground>, <background>`
- [ ] `TILEAT <number>, <x>, <y>`
- [ ] `TILESET <number>, <lut>, <is_square>, <address>`
- [ ] `TILESET <number>, <lut>, <is_square>, <address>`
- [ ] `TILESHOW <number>, <is_visible>`
- [ ] `GETDATE$()`
- [ ] `GETTIME$()`
