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
  line_edit.c
 
MODEL = --code-model=large --data-model=small
LIB_MODEL = lc-sd
C_FLAGS = -Iinclude -DBUILD_VER="\"$(BUILD_VER)\""

FOENIX_LIB = $(FOENIX)/foenix-$(LIB_MODEL).a
A2560U_RULES = $(FOENIX)/linker-files/a2560u-simplified.scm
A2560K_RULES = $(FOENIX)/linker-files/a2560k-simplified.scm

# Object files
OBJS = $(ASM_SRCS:%.s=obj/%.o) $(C_SRCS:%.c=obj/%.o)
OBJS_DEBUG = $(ASM_SRCS:%.s=obj/%-debug.o) $(C_SRCS:%.c=obj/%-debug.o)

all: basicu.pgz basick.pgz

obj/%.o: %.s
	as68k --core=68000 $(MODEL) --debug --list-file=$(@:%.o=%.lst) -o $@ $<

obj/%.o: %.c
	cc68k $(C_FLAGS) --core=68000 $(MODEL) --debug --list-file=$(@:%.o=%.lst) -o $@ $<

obj/%-debug.o: %.s
	as68k --core=68000 $(MODEL) --debug --list-file=$(@:%.o=%.lst) -o $@ $<

obj/%-debug.o: %.c
	cc68k $(C_FLAGS) --core=68000 $(MODEL) --debug --list-file=$(@:%.o=%.lst) -o $@ $<

basicu.pgz:  $(OBJS) $(FOENIX_LIB)
	ln68k -o $@ $^ $(A2560U_RULES) clib-68000-$(LIB_MODEL).a --output-format=pgz --list-file=basicu.lst --cross-reference --rtattr printf=float --rtattr scanf=float --rtattr cstartup=Foenix_user --stack-size=4096 --heap-size=20000

basick.pgz:  $(OBJS) $(FOENIX_LIB)
	ln68k -o $@ $^ $(A2560K_RULES) clib-68000-$(LIB_MODEL).a --output-format=pgz --list-file=basick.lst --cross-reference --rtattr printf=float --rtattr scanf=float --rtattr cstartup=Foenix_user --stack-size=4096 --heap-size=20000

$(FOENIX_LIB):
	(cd $(FOENIX) ; make all)

clean:
	-rm $(OBJS) $(OBJS:%.o=%.lst) $(OBJS_DEBUG) $(OBJS_DEBUG:%.o=%.lst) $(FOENIX_LIB)
	-rm basicu.pgz basick.pgz basicu.lst basick.lst 
	-(cd $(FOENIX) ; make clean)
