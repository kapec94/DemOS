/*
 * cmd.h
 *
 *  Created on: 1 cze 2014
 *      Author: Stasiu
 */

#ifndef CMD_H_
#define CMD_H_

#define DECL_CMD(n) { name: #n, function: cmd_ ## n }

typedef int (*cmdf)(int, char**);
struct cmd_t
{
	const char* name;
	cmdf function;
};

int cmd_call(const char* cmd, int argc, char** argv);
int cmd_help(int argc, char** argv);

#endif /* CMD_H_ */
