VPATH = src

FOENIX = module/Calypsi-m68k-Foenix

BUILD_VER = 0.2.0 ($(shell git branch --show-current), $(shell date +"%b %d %Y %H:%M"))

# Common source files
ASM_SRCS =
C_SRCS = a2560_arch.c \
  a2560_error.c \
  a2560_kbhit.c \
  a2560_main.c \
  array.c \
  dictionary.c \
  hexdump.c \
  io.c \
  lines.c \
  parser.c \
  tokenizer.c \
  variables.c \
  clock.c \
  console.c \
  auto.c \
  line_edit.c \
  graphics.c
 
MODEL = --code-model=large --data-model=small
LIB_MODEL = lc-sd
C_FLAGS = -Iinclude -DBUILD_VER="\"$(BUILD_VER)\""

FOENIX_LIB = lib/foenix-$(LIB_MODEL).a
A2560K_RULES = lib/a2560k-simplified.scm

# Object files
OBJS = $(ASM_SRCS:%.s=obj/%.o) $(C_SRCS:%.c=obj/%.o)
OBJS_DEBUG = $(ASM_SRCS:%.s=obj/%-debug.o) $(C_SRCS:%.c=obj/%-debug.o)

all: basick.pgz

obj/%.o: %.s
	as68k --core=68000 $(MODEL) --debug --list-file=$(@:%.o=%.lst) -o $@ $<

obj/%.o: %.c
	cc68k $(C_FLAGS) --core=68000 $(MODEL) --debug --list-file=$(@:%.o=%.lst) -o $@ $<

obj/%-debug.o: %.s
	as68k --core=68000 $(MODEL) --debug --list-file=$(@:%.o=%.lst) -o $@ $<

obj/%-debug.o: %.c
	cc68k $(C_FLAGS) --core=68000 $(MODEL) --debug --list-file=$(@:%.o=%.lst) -o $@ $<

basick.pgz:  $(OBJS)
	ln68k -o $@ $^ $(A2560K_RULES) clib-68000-$(LIB_MODEL).a $(FOENIX_LIB) --output-format=pgz --list-file=basick.lst --cross-reference --rtattr printf=float --rtattr scanf=float --rtattr cstartup=Foenix_user --stack-size=4096 --heap-size=20000

clean:
	-rm $(OBJS) $(OBJS:%.o=%.lst) $(OBJS_DEBUG) $(OBJS_DEBUG:%.o=%.lst)
	-rm basick.pgz basick.lst 
