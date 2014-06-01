/*
 * cmd.c
 *
 */

#include <stddef.h>
#include <string.h>
#include <conio.h>

#include "cmd.h"

static struct cmd_t _commands[] = {
		DECL_CMD(help),
		{ name: NULL, function: NULL }
};

int cmd_call(const char* cmd, int argc, char** argv)
{
	struct cmd_t* command = _commands;
	while (command->name != NULL) {
		if (strcmp(command->name, cmd) == 0) {
			return command->function(argc, argv);
		}
		command++;
	}

	cprintf("Command '%s' not found.\n", cmd);
	return 1;
}

int cmd_help(int argc, char** argv)
{
	cputs("This is help.\n");
	return 0;
}

