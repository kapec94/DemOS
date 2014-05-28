/*
 * ps2.h - PS/2 routines
 *
 */

#ifndef PS2_H_
#define PS2_H_

#include "cpu.h"

#define PS2_DATA_PORT	0x60

#define PS2_CMD_PORT	0x64
#define PS2_STATUS_PORT	0x64

/* Output buffer is full (must be set before attempting to read data from
 * PS2_DATA_PORT */
#define PS2_STATUS_OUTB_FULL	0x00
/* Input buffer is full (must be clear before attempting to write data to
 * PS2_DATA_PORT or PS2_CMD_PORT) */
#define PS2_STATUS_INB_FULL		0x01
/* Cleared on reset and set by BIOS during POST */
#define PS2_STATUS_SYSTEM		0x02
/* Data written to input buffer is for PS/2 controller command */
#define PS2_STATUS_DATA_CMD		0x03
/* Time-out error */
#define PS2_STATUS_TIMEOUT		0x06
/* Parity error */
#define PS2_STATUS_PARITYE		0x07

/* PS/2 commands */
#define PS2_CMD_READ_CCB		0x20
#define PS2_CMD_WRITE_CCB		0x60
#define PS2_CMD_DISABLE_PORT1	0xAD
#define PS2_CMD_ENABLE_PORT1	0xAE
#define PS2_CMD_DISABLE_PORT2	0xA7
#define PS2_CMD_ENABLE_PORT2	0xA8
#define PS2_CMD_SELF_TEST		0xAA
#define PS2_CMD_PORT1_TEST		0xAB
#define PS2_CMD_PORT2_TEST		0xA9
#define PS2_CMD_WRITE_PORT2		0xD4

#define PS2_SELF_TEST_OK		0x55
#define PS2_SELF_TEST_ERROR		0xFC

#define PS2_PORT_TEST_OK		0x00
#define PS2_PORT_TEST_CH		0x01	/* Clock stuck high */
#define PS2_PORT_TEST_CL		0x02	/* Clock stuck low */
#define PS2_PORT_TEST_DH		0x03	/* Data... */
#define PS2_PORT_TEST_DL		0x04	/* Data... */

/* Flags for funtion ps2_ccb. */
#define PS2_CCB_INT		0x00
#define PS2_CCB_CLOCK	0x01
#define PS2_CCB_TRANS	0x02

/* Actual CCB register flags. */
#define PS2_CCB_FLAGS_PORT1_INT		0x00
#define PS2_CCB_FLAGS_PORT2_INT		0x01
#define PS2_CCB_FLAGS_PORT1_CLOCK	0x04
#define PS2_CCB_FLAGS_PORT2_CLOCK	0x05
#define PS2_CCB_FLAGS_PORT1_TRANS	0x06

/* For port specific ports */
#define PS2_PORT1	0x00
#define PS2_NO_PORT	PS2_PORT1
#define PS2_PORT2	0x01

#define PS2_DEVICE_IDENTIFY			0xF2
#define PS2_DEVICE_DISABLE_SCANNING	0xF5
#define PS2_DEVICE_RESET			0xFF

#define PS2_DEVICE_RESPONSE_ACK		0xFA

/* Ancient AT keyboard with translation enabled in the PS/Controller
 * (not possible for the second PS/2 port) */
#define PS2_DEVTYPE_KBD_ANCIENT		0x00
/* Standard PS/2 mouse */
#define PS2_DEVTYPE_MOUSE			0x01
/* Mouse with scroll wheel */
#define PS2_DEVTYPE_MOUSE_SCROLL	0x02
/* 5-button mouse */
#define PS2_DEVTYPE_MOUSE_5BUT		0x03
/* MF2 keyboard with translation enabled in the PS/Controller
 * (not possible for the second PS/2 port) */
#define PS2_DEVTYPE_KBD_TRANS		0x04
/* MF2 keyboard */
#define PS2_DEVTYPE_KBD				0x05

/* Read PS/2 controller configuration byte */
u32 ps2_read_ccb(u8* ccb);

/* Read PS/2 controller configuration byte */
u32 ps2_write_ccb(u8 ccb);

/* Initialize PS/2 ports for usage. Note that it actually doesn't check
 * for PS/2 existence and doesn't touch USB Legacy Support settings, so it may
 * cause system to crash or misbehave, even if everything went good.
 */
u32 ps2_init();

u8 ps2_null_ccb();

u32 ps2_has_port(u32 port);
u32 ps2_detect(u32 port, u32* type);

u32 ps2_recv_data(u8* data);
u32 ps2_send_data(u32 port, u8 data);

u32 ps2_wait_inbuf(u32 timeout);
u32 ps2_wait_outbuf(u32 timeout);

u32 ps2_enable(u32 port);
u32 ps2_disable(u32 port);
u32 ps2_enable_int(u32 port);
u32 ps2_disable_int(u32 port);

u32 ps2_reset_port(u32 port);

static inline u8 ps2_read_status()
{
	return in(PS2_STATUS_PORT);
}

static inline u8 ps2_read_data()
{
	return in(PS2_DATA_PORT);
}

static inline void ps2_write_data(u8 data)
{
	out(PS2_DATA_PORT, data);
}

static inline void ps2_write_cmd(u8 cmd)
{
	out(PS2_CMD_PORT, cmd);
}

#endif /* PS2_H_ */
