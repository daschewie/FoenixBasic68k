#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "line_edit.h"
#include "mcp/syscalls.h"

/*
 * CLI special key code definitions
 */

#define CLI_FLAG_CTRL   0x0100  /* Flag indicating CTRL is pressed */
#define CLI_FLAG_SHIFT  0x0200  /* Flag indicating SHIFT is pressed */
#define CLI_FLAG_ALT    0x0400  /* Flag indicating ALT is pressed */
#define CLI_FLAG_OS     0x0800  /* Flag indicating OS key is pressed */
#define CLI_FLAG_FUNC   0x4000  /* Function keys: 0x4001 - 0x400C */
#define CLI_FLAG_SPEC   0x8000  /* Special keys: */
#define CLI_KEY_LEFT    0x8001
#define CLI_KEY_RIGHT   0x8002
#define CLI_KEY_UP      0x8003
#define CLI_KEY_DOWN    0x8004
#define CLI_KEY_DEL     0x8005
#define CLI_KEY_MONITOR 0x8010  /* A2560K Monitor key */
#define CLI_KEY_CTX     0x8011  /* A2560K CTX Switch key */
#define CLI_KEY_HELP    0x8012  /* A2560K Menu/Help key */

#define CHAR_ESC    '\x1B'  /* Escape character */
#define CHAR_TAB    '\t'    /* Vertical tab */
#define CHAR_CR     '\x0D'  /* Carriage return */
#define CHAR_NL     '\x0A'  /* Linefeed */
#define CHAR_BS     '\b'    /* Backspace */


/*
 * States to interpret ANSI escape codes
 */
typedef enum {
    CLI_ES_BASE = 0,    // Base state
    CLI_ES_ESC,         // "ESC" seen
    CLI_ES_CSI,         // "ESC[" has been seen
    CLI_ES_SEMI         // Semicolon has been seen
} cli_state;

/** The history of previous commands issued */
static char cli_history[CLI_MAX_HISTORY_DEPTH][CLI_MAX_COMMAND_SIZE];

static void print(short channel, char *str) {
    sys_chan_write(channel, (unsigned char*)str, strlen(str));
}

static void cli_history_push(char *command_line) {
    for (int i = CLI_MAX_HISTORY_DEPTH - 1; i > 0; i--) {
        // Copy previous commands down
        strcpy(cli_history[i], cli_history[i-1]);
    }
    strcpy(cli_history[0], command_line);
}

/**
 * Decode ANSI modifier codes
 *
 * @param modifiers the ANSI modifier codes from an escape sequence
 * @return CLI modifier flags
 */
static short cli_translate_modifiers(short modifiers) {
    char buffer[10];
    short flags = 0;

    if (modifiers > 0) {
        modifiers--;
    }

    if (modifiers & 0x01) flags |= CLI_FLAG_SHIFT;
    if (modifiers & 0x02) flags |= CLI_FLAG_ALT;
    if (modifiers & 0x04) flags |= CLI_FLAG_CTRL;
    if (modifiers & 0x08) flags |= CLI_FLAG_OS;

    return flags;
}

/**
 * Translate escape sequences that end in a letter code
 *
 * @param modifiers optional parameter
 * @code the letter code of the key
 */
static short cli_translate_alpha(short modifiers, char code) {
    short key_code = 0;

    key_code = cli_translate_modifiers(modifiers);

    switch (code) {
        case 'A':
            key_code |= CLI_KEY_UP;
            break;

        case 'B':
            key_code |= CLI_KEY_DOWN;
            break;

        case 'C':
            key_code |= CLI_KEY_RIGHT;
            break;

        case 'D':
            key_code |= CLI_KEY_LEFT;
            break;

        default:
            return 0;
    }

    return key_code;
}

/**
 * Translate escape sequences that end in a numeric code
 *
 * @param modifiers optional parameter
 * @code the numeric code of the key
 */
static short cli_translate_numeric(short modifiers, short code) {
    short key_code = 0;

    key_code = cli_translate_modifiers(modifiers);

    if ((code >= 11) && (code <= 15)) {
        // Function keys 1 - 5
        key_code |= CLI_FLAG_FUNC | (code - 10);

    } else if ((code >= 17) && (code <= 21)) {
        // Function keys 6 - 10
        key_code |= CLI_FLAG_FUNC | (code - 11);

    } else if (code == 30) {
        // MONITOR key
        key_code |= CLI_KEY_MONITOR;

    } else if (code == 31) {
        // CTX SWITCH key
        key_code |= CLI_KEY_CTX;

    } else if (code == 32) {
        // MENU HELP key
        key_code |= CLI_KEY_HELP;

    } else if (code == 3) {
        // DELETE key
        key_code |= CLI_KEY_DEL;

    } else {
        // Unknown escape code
        key_code = 0;
    }

    return key_code;
}

/**
 * Get a character from the console, processing recognized escape sequences
 *
 * @param channel the number of the input channel
 * @return the 16-bit functional character code
 */
short cli_getchar(short channel) {
    char buffer[10];
    cli_state state = CLI_ES_BASE;      // Current state of the escape sequence
    short number1 = 0, number2 = 0;     // Two numbers that can be embedded in the sequence
    char c;                             // The current character read from the console

    do {
        c = sys_chan_read_b(channel);
        switch (state) {
            case CLI_ES_BASE:
                // We are not processing a sequence...
                if (c == CHAR_ESC) {
                    // Escape has been seen
                    state = CLI_ES_ESC;

                } else {
                    // It's an ordinary character, so return it
                    return c;
                }
                break;

            case CLI_ES_ESC:
                // Escape has been seen... check for CSI
                if (c == '[') {
                    // ESC [ has been seen...
                    state = CLI_ES_CSI;

                } else {
                    // Bad escape sequence...just return the character
                    state = CLI_ES_BASE;
                    return c;
                }
                break;

            case CLI_ES_CSI:
                // ESC [ has been seen... next is either a number, a letter, a semi-colon, or a tilda
                if (isdigit(c)) {
                    // It's a number... shift it onto number1
                    number1 = number1 * 10 + (c - '0');

                } else if (isalpha(c)) {
                    // It's a letter... treat as a code
                    return cli_translate_alpha(number1, c);

                } else if (c == ';') {
                    // Got a semicolon, go to that state
                    state = CLI_ES_SEMI;

                } else if (c == '~') {
                    // Got a tilda... end of numeric code with no parameters
                    return cli_translate_numeric(0, number1);

                } else {
                    // Bad sequence... just return the current character
                    state = CLI_ES_BASE;
                    return c;
                }
                break;

            case CLI_ES_SEMI:
                // Semicolon has been seen... next is either a number or a tilda
                if (isdigit(c)) {
                    // It's a number... shift it onto number1
                    number2 = number2 * 10 + (c - '0');

                } else if (c == '~') {
                    // Got a tilda... end of numeric code with parameters
                    return cli_translate_numeric(number2, number1);

                } else {
                    // Bad sequence... just return the current character
                    state = CLI_ES_BASE;
                    return c;
                }
                break;

            default:
                state = CLI_ES_BASE;
                return c;
        }
    } while (1);
}


/**
 * Read a line of input from the channel, allowing for editing of the line
 *
 * @param channel the input channel to use
 * @param command_line a character buffer in which to store the line
 * @return 0 on success, -1 to swap screens (if applicable), -2 to request command help
 */
short cli_readline(short channel, char * command_line) {
    char buffer[10];
    unsigned short key_code = 0;
    short i = 0, j = 0;
    short history = 0;
    short result = 0;

    // Make sure key echo is turned off
    sys_chan_ioctrl(channel, 0x04, 0, 0);

    // Zero out the command line buffer
    for (i = 0; i < CLI_MAX_COMMAND_SIZE; i++) {
        command_line[i] = 0;
    }

    i = 0;

    do {
        key_code = cli_getchar(channel);

        if ((key_code & 0xF000) == 0) {
            // Ordinary key...
            char c = (char)(key_code & 0x00ff);
            if (c < 0x20) {
                if (c == 3) {
                    result = -4;
                    goto end;
                }
                // Control key...
                if (c == CHAR_CR) {
                    // Newline... we're done
                    // TODO: add the line to the command history
                    cli_history_push(command_line);
                    goto end;

                } else if (c == CHAR_ESC) {
                    // ESC pressed... clear whole line

                    // Zero out the command line buffer
                    for (i = 0; i < CLI_MAX_COMMAND_SIZE; i++) {
                        command_line[i] = 0;
                    }
                    i = 0;
                    print(channel, "\x1b[3G\x1b[K");

                } else if (c == CHAR_BS) {
                    // Backspace
                    if (i > 0) {
                        i--;
                        print(channel, "\x1b[D");
                        for (j = i; j < strlen(command_line); j++) {
                            command_line[j] = command_line[j+1];
                        }
                        print(channel, "\x1b[1P");
                    }
                }

            } else {
                // Add key to command line
                command_line[i++] = c;
                sys_chan_write_b(channel, c);
            }

        } else {
            // Special editing key
            switch (key_code & 0xF0FF) {
                case CLI_KEY_LEFT:
                    // Move cursor to the left
                    if (key_code & CLI_FLAG_CTRL) {
                        i = 0;
                        print(channel, "\x1b[3G");
                    } else {
                        if (i > 0) {
                            i--;
                            print(channel, "\x1b[D");
                        }
                    }
                    break;

                case CLI_KEY_RIGHT:
                    // Move cursor right
                    if (key_code & CLI_FLAG_CTRL) {
                        sprintf(buffer, "\x1b[%dG", (short)(3 + strlen(command_line)));
                        print(channel, buffer);
                        i = strlen(command_line);
                    } else {
                        if (command_line[i] != 0) {
                            i++;
                            print(channel, "\x1b[C");
                        }
                    }
                    break;

                case CLI_KEY_UP:
                    // Go back one command in history
                    if ((history < CLI_MAX_HISTORY_DEPTH) && (cli_history[history][0] != 0)) {
                        strcpy(command_line, cli_history[history++]);
                        print(channel, "\x1b[3G\x1b[K");
                        print(channel, command_line);
                    }
                    break;

                case CLI_KEY_DOWN:
                    // Go forward one command in history
                    if (history > 0) {
                        strcpy(command_line, cli_history[--history]);
                        print(channel, "\x1b[3G\x1b[K");
                        print(channel, command_line);
                    }
                    break;

                case CLI_KEY_DEL:
                    // Delete the character under the cursor
                    for (j = i; j < strlen(command_line); j++) {
                        command_line[j] = command_line[j+1];
                    }
                    print(channel, "\x1b[1P");
                    break;

                case CLI_KEY_CTX:
                    // Switch the console screens (if applicable)
                    result = -1;
                    goto end;

                case CLI_KEY_HELP:
                    // Request the help screen
                    if (key_code & CLI_FLAG_OS) {
                        result = -3;
                        goto end;
                    } else {
                        result = -2;
                        goto end;
                    }

                default:
                    // Unknown... do nothing
                    break;
            }
        }
    } while (1);

end:
    sys_chan_ioctrl(channel, 0x03, 0, 0);
    return result;
}


