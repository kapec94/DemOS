/*
 * gdb_impl.c - gdb_stub.c needed implementations.
 *
 */

#include <raw_video.h>
#include <com.h>
#include <stdarg.h>

#include "gdb_impl.h"

int stdout = 0;
int stderr = 1;

u16 _com_port = 0;

int fprintf(int fd, const char* f, ...) {
	u16 attr = rvid_getattr();
	int chars = 0;
	va_list va;

	if (fd == stderr)
		rvid_setattr(RVID_ATTR(COLOR_WHITE, COLOR_LRED));

	va_start(va, f);
	chars = rvid_vprintf(f, va);
	va_end(va);

	rvid_setattr(attr);
	return chars;
}

int gdb_stub_init()
{
	u16 ports[4];
	size_t len;

	com_get_ports(ports, &len);
	if (len > 0) {
		_com_port = ports[0];
		rvid_printf("Found port at 0x%x\n", _com_port);
	} else {
		rvid_printf("No ports found! Cannot init GDB stub!\n");
		return E_NO;
	}

	com_init_port(_com_port, 115200, COM_DATA_LEN_8, COM_STOP_ONE, COM_PARITY_NONE);
	return S_OK;
}

int getDebugChar()
{
	while (!com_data_available(_com_port)) ;
	return com_data_read(_com_port);
}

void putDebugChar(int c)
{
	while (!com_transmitter_empty(_com_port)) ;
	com_data_write(_com_port, c);
}
