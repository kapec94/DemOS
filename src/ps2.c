/*
 * ps2.c - PS/2 routines
 *
 */

#include <stddef.h>
#include <conio.h>
#include "ps2.h"

u32 has_port1;
u32 has_port2;

u32 ps2_init()
{
	u8 ccb = 0;
	u32 result = 0;

	has_port1 = has_port2 = 1;

	/* Disable both ports not to be disturbed during initialization */
	cprintf("  Disabling ports...\n");
	ps2_write_cmd(PS2_CMD_DISABLE_PORT1);
	ps2_write_cmd(PS2_CMD_DISABLE_PORT2);

	/* Flush output buffer to ensure valid device state */
	in(PS2_DATA_PORT);

	result = ps2_read_ccb(&ccb);
	if (result == E_NO) {
		cprintf("read_ccb error\n");
		return E_NO;
	}

	if (get_bit(ccb, PS2_CCB_FLAGS_PORT2_CLOCK) == 0) {
		has_port2 = 0;
	}

	/* Temporarily write null CCB. */
	cprintf("  Writing null CCB...\n");
	ps2_write_ccb(ps2_null_ccb());

	/* Perform self-check */
	cprintf("  Self-check...\n");

	ps2_write_cmd(PS2_CMD_SELF_TEST);
	result = ps2_read_data();
	if (result == PS2_SELF_TEST_ERROR) {
		return E_NO | (result << 16);
	}

	/* Perform interface test */
	cprintf("  Port 1 test...\n");
	ps2_write_cmd(PS2_CMD_PORT1_TEST);
	result = ps2_read_data();
	if (result != PS2_PORT_TEST_OK) {
		has_port1 = 0;
	}
	if (has_port2 == 1) {
		cprintf("  Port 2 test...\n");
		ps2_write_cmd(PS2_CMD_PORT2_TEST);
		result = ps2_read_data();
		if (result != PS2_PORT_TEST_OK) {
			has_port2 = 0;
		}
	}

	if (!has_port1 && !has_port2) {
		cprintf("  No ports available!\n");
		return E_NO;
	}

	/*
	if (has_port1) {
		rvid_printf("  Enabling port 1...\n");
		ps2_write_cmd(PS2_CMD_ENABLE_PORT1);
	}
	if (has_port2) {
		rvid_printf("  Enabling port 2...\n");
		ps2_write_cmd(PS2_CMD_ENABLE_PORT2);
	}
	*/

	return S_OK;
}

u32 ps2_detect(u32 port, u32* type)
{
	if (port != PS2_PORT1 && port != PS2_PORT2) return E_NO;

	u32 i = 0, n = 0;
	u8 result = 0;
	u8 response[2];

	if (ps2_send_data(port, PS2_DEVICE_DISABLE_SCANNING) == E_NO) {
		return E_NO;
	}
	if (ps2_recv_data(&result) == E_NO || result != PS2_DEVICE_RESPONSE_ACK) {
		return E_NO | (result << 16);
	}

	if (ps2_send_data(port, PS2_DEVICE_IDENTIFY) == E_NO) {
		return E_NO;
	}
	if (ps2_recv_data(&result) == E_NO || result != PS2_DEVICE_RESPONSE_ACK) {
		return E_NO | (result << 16);
	}

	for (i = 0; i < 2; i++) {
		if (ps2_recv_data(&result) == E_NO) continue;
		else {
			response[n++] = result;
		}
	}

	switch (n) {
	case 0:
		*type = PS2_DEVTYPE_KBD_ANCIENT;
		break;
	case 1:
		switch (response[0]) {
		case 0x00:
			*type = PS2_DEVTYPE_MOUSE;
			break;
		case 0x03:
			*type = PS2_DEVTYPE_MOUSE_SCROLL;
			break;
		case 0x04:
			*type = PS2_DEVTYPE_MOUSE_5BUT;
			break;
		default:
			return E_NO;
		}
		break;
	case 2:
		switch (response[1]) {
		case 0x41:
		case 0xC1:
			*type = PS2_DEVTYPE_KBD_TRANS;
			break;
		case 0x83:
			*type = PS2_DEVTYPE_KBD;
			break;
		default:
			return E_NO;
		}
		break;
	}
	return S_OK;
}

u32 ps2_recv_data(u8* data)
{
	if (ps2_wait_outbuf(100000)) {
		*data = ps2_read_data();
		return S_OK;
	} else {
		return E_NO;
	}
}

u32 ps2_send_data(u32 port, u8 data)
{
	if (port == PS2_PORT2) {
		ps2_write_cmd(PS2_CMD_WRITE_PORT2);
	}

	if (ps2_wait_inbuf(100000)) {
		ps2_write_data(data);
		return S_OK;
	} else {
		return E_NO;
	}
}

u32 ps2_wait_inbuf(u32 timeout)
{
	u32 ticks = getticks();
	while (getticks() < ticks + timeout) {
		u8 status = ps2_read_status();
		if (get_bit(status, PS2_STATUS_INB_FULL) == 0) {
			return 1;
		} else {
			io_wait();
		}
	}

	return 0;
}

u32 ps2_wait_outbuf(u32 timeout)
{
	u32 ticks = getticks();
	while (getticks() < ticks + timeout) {
		u8 status = ps2_read_status();
		if (get_bit(status, PS2_STATUS_OUTB_FULL) == 1) {
			return 1;
		} else {
			io_wait();
		}
	}

	return 0;
}

u32 ps2_read_ccb(u8* ccb)
{
	ps2_write_cmd(PS2_CMD_READ_CCB);
	return ps2_recv_data(ccb);
}

u32 ps2_write_ccb(u8 ccb)
{
	ps2_write_cmd(PS2_CMD_WRITE_CCB);
	return ps2_send_data(PS2_NO_PORT, ccb);
}

u8 ps2_null_ccb()
{
	return 0b00110100; /* Default CCB with everything disabled */
}

u32 ps2_enable(u32 port)
{
	if (port == PS2_PORT1) {
		ps2_write_cmd(PS2_CMD_ENABLE_PORT1);
		return S_OK;
	}
	if (port == PS2_PORT2) {
		ps2_write_cmd(PS2_CMD_ENABLE_PORT2);
		return S_OK;
	}

	return E_NO;
}

u32 ps2_disable(u32 port)
{
	if (port == PS2_PORT1) {
		ps2_write_cmd(PS2_CMD_DISABLE_PORT1);
		return S_OK;
	}
	if (port == PS2_PORT2) {
		ps2_write_cmd(PS2_CMD_DISABLE_PORT2);
		return S_OK;
	}

	return E_NO;
}

u32 ps2_enable_int(u32 port)
{
	u8 ccb = 0;
	if (ps2_read_ccb(&ccb) == E_NO) {
		return E_NO;
	}

	if (port == PS2_PORT1) {
		set_bit(ccb, PS2_CCB_FLAGS_PORT1_INT);
	}
	if (port == PS2_PORT2) {
		set_bit(ccb, PS2_CCB_FLAGS_PORT2_INT);
	}

	return ps2_write_ccb(ccb);
}

u32 ps2_disable_int(u32 port)
{
	u8 ccb = 0;
	if (ps2_read_ccb(&ccb) == E_NO) {
		return E_NO;
	}

	if (port == PS2_PORT1) {
		clear_bit(ccb, PS2_CCB_FLAGS_PORT1_INT);
	}
	if (port == PS2_PORT2) {
		clear_bit(ccb, PS2_CCB_FLAGS_PORT2_INT);
	}

	return ps2_write_ccb(ccb);
}

u32 ps2_reset_port(u32 port)
{
	u32 result = 0;
	u8 data = 0;

	ps2_send_data(port, PS2_DEVICE_RESET);
	result = ps2_recv_data(&data);
	if (result == E_NO || data != PS2_DEVICE_RESPONSE_ACK) {
		return E_NO;
	} else {
		return S_OK;
	}
}

u32 ps2_has_port1()
{
	return has_port1;
}

u32 ps2_has_port2()
{
	return has_port2;
}
