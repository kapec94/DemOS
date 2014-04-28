/*
 * gdb.h
 *
 *  Created on: 24 kwi 2014
 *      Author: Mateusz
 */

#ifndef GDB_IMPL_H_
#define GDB_IMPL_H_

extern int stdout;
extern int stderr;

#define printf rvid_printf
int fprintf(int fd, const char* f, ...);

int gdb_stub_init();

#endif /* GDB_IMPL_H_ */
