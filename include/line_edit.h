#ifndef _LINE_EDIT_H
#define _LINE_EDIT_H

#define CLI_MAX_HISTORY_DEPTH   5   /* Maximum number of commands we'll record */
#define CLI_MAX_COMMAND_SIZE    128 /* Maximum number of characters in a command line */

/**
 * Read a line of input from the channel, allowing for editing of the line
 *
 * @param channel the input channel to use
 * @param command_line a character buffer in which to store the line
 * @return 0 on success, -1 to swap screens (if applicable), -2 to request command help
 */
short cli_readline(short channel, char * command_line);

/**
 * Get a character from the console, processing recognized escape sequences
 *
 * @param channel the number of the input channel
 * @return the 16-bit functional character code
 */
short cli_getchar(short channel);

#endif