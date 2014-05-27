/*
 * ps2.c - PS/2 routines
 *
 */

#include <stddef.h>
#include "ps2.h"

u32 has_port1;
u32 has_port2;

u32 ps2_init()
{
	u8 ccb = 0;
	u32 result = 0;

	has_port1 = has_port2 = 1;

	/* Disable both ports not to be disturbed during initialization */
	ps2_write_cmd(PS2_CMD_DISABLE_PORT1);
	ps2_write_cmd(PS2_CMD_DISABLE_PORT2);

	/* Flush output buffer to ensure valid device state */
	in(PS2_DATA_PORT);

	ccb = ps2_read_ccb();
	if (get_bit(ccb, PS2_CCB_FLAGS_PORT2_CLOCK) == 0) {
		has_port2 = 0;
	}

	/* Temporarily write null CCB. */
	ps2_write_ccb(ps2_null_ccb());

	/* Perform self-check */
	ps2_write_cmd(PS2_CMD_SELF_TEST);
	result = ps2_read_data();
	if (result == PS2_SELF_TEST_ERROR) {
		return E_NO | (result << 16);
	}

	/* Perform interface test */
	ps2_write_cmd(PS2_CMD_PORT1_TEST);
	result = ps2_read_data();
	if (result != PS2_PORT_TEST_OK) {
		has_port1 = 0;
	}
	if (has_port2 == 1) {
		ps2_write_cmd(PS2_CMD_PORT2_TEST);
		result = ps2_read_data();
		if (result != PS2_PORT_TEST_OK) {
			has_port2 = 0;
		}
	}

	if (!has_port1 && !has_port2) {
		return E_NO;
	}

	/* Reenable ports */
	if (has_port1) {
		ps2_write_cmd(PS2_CMD_ENABLE_PORT1);
	}
	if (has_port2) {
		ps2_write_cmd(PS2_CMD_ENABLE_PORT2);
	}

	return S_OK;
}

u8 ps2_null_ccb()
{
	return 0b00110100; /* Default CCB with everything disabled */
}
