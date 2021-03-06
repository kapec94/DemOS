/*
 * keyboard.c
 *
 */

#include <ps2.h>
#include <pic.h>
#include "keyboard.h"

#define KBD_PRESSED		0x01
#define KBD_RELEASED	0x00

#define KBD_CMD_SCANCODE	0xF0
#define KBD_SCANCODE_GET	0x00
#define KBD_SCANCODE_1		0x01
#define KBD_SCANCODE_2		0x02
#define KBD_SCANCODE_3		0x03

#define KBD_SCP_ESCAPE '\x1B'
#define KBD_SCP_BACKSPACE '\x08'

#define KBD_SCP_LCTRL '\x11'
#define KBD_SCP_LALT '\x12'
#define KBD_SCP_RCTRL '\x13'
#define KBD_SCP_RALT '\x14'
#define KBD_SCP_LSHIFT '\x0E'
#define KBD_SCP_RSHIFT '\x0F'

#define KBD_SCP_PAD_STAR '\xFA'
#define KBD_SCP_PAD_SLASH '\xFB'
#define KBD_SCP_PAD_DASH '\xFC'
#define KBD_SCP_PAD_PLUS '\xFD'
#define KBD_SCP_PAD_RET '\xFE'
#define KBD_SCP_PAD_DOT '\xFF'
#define KBD_SCP_PAD_0 '\x8C'
#define KBD_SCP_PAD_1 '\x8D'
#define KBD_SCP_PAD_2 '\x8E'
#define KBD_SCP_PAD_3 '\x8F'
#define KBD_SCP_PAD_4 '\x90'
#define KBD_SCP_PAD_5 '\x91'
#define KBD_SCP_PAD_6 '\x92'
#define KBD_SCP_PAD_7 '\x93'
#define KBD_SCP_PAD_8 '\x94'
#define KBD_SCP_PAD_9 '\x95'

#define KBD_SCP_CAPS '\x0B'
#define KBD_SCP_NUM '\x0C'

#define KBD_SCP_F1 '\x80'
#define KBD_SCP_F2 '\x81'
#define KBD_SCP_F3 '\x82'
#define KBD_SCP_F4 '\x83'
#define KBD_SCP_F5 '\x84'
#define KBD_SCP_F6 '\x85'
#define KBD_SCP_F7 '\x86'
#define KBD_SCP_F8 '\x87'
#define KBD_SCP_F9 '\x88'
#define KBD_SCP_F10 '\x89'
#define KBD_SCP_F11 '\x8A'
#define KBD_SCP_F12 '\x8B'

static u32 _ps2_port;
static const char* _codepage = \
		"\0\x1B"		/* 0 - 1 */
		"1234567890"	/* 2 - B */
		"-=\x08\t"		/* C - F */
		"qwertyuiop[]"	/* 10 - 1B */
		"\n\x11"		/* 1C - 1D */
		"asdfghjkl;'`"	/* 1E - 29 */
		"\x0E\\"		/* 2A - 2B */
		"zxcvbnm,./"	/* 2C - 35 */
		"\x1F\xFA\x12"	/* 36 - 38 */
		" \x0B"			/* 39 - 3A */
		"\x80\x81\x82"	/* 3B - 3D */
		"\x83\x84\x85"	/* 3E - 40 */
		"\x86\x87\x88"	/* 41 - 43 */
		"\x89\x0C\0"	/* 44 - 46 */
		"\x93\x94\x95"	/* 47 - 49 */
		"\xFC\x90\x91"	/* 4A - 4C */
		"\x92\xFD\x8D"	/* 4D - 4F */
		"\x8E\x8F\x8C"	/* 50 - 52 */
		"\xFF\0\0"		/* 53 - 55 */
		"\0\x8A\x8B"	/* 56 - 58 */;

static volatile u32 _codes_pressed[0x100];

static volatile u32 _capslock;

static volatile u32 _wait_next;
static volatile u32 _next_char;

static u32 _isletter(u32 code);
static u32 _hasupper(u32 code);
static u32 _uplow(u32 code);

u32 kbd_init(u32 port)
{
	if (!kbd_iskbd(port)) return E_NO;

	u8 data = 0;
	u32 result = 0;

	_ps2_port = port;

	ps2_enable(port);
	ps2_enable_int(port);
	ps2_reset_port(port);

	ps2_send_data(_ps2_port, KBD_CMD_SCANCODE);
	ps2_send_data(_ps2_port, KBD_SCANCODE_1);

	result = ps2_recv_data(&data);
	if (result == E_NO || data != PS2_DEVICE_RESPONSE_ACK) {
		return E_NO;
	}

	return S_OK;
}

u32 kbd_scan_nopoll()
{
	return ps2_read_data();
}

u32 kbd_code_nopoll()
{
	u32 scan = kbd_scan_nopoll();
	return _codepage[scan % 0x80];
}

u32 kbd_nextpress()
{
	u32 c = 0;

	_wait_next = 1;
	while (_next_char == '\0') nop();

	c = _next_char;
	_next_char = '\0';

	return c;
}

u32 kbd_getch()
{
	u32 code = 0;

	do {
		code = kbd_nextpress();
	} while (!kbd_isprintable(code));

	if (kbd_ispressed(KBD_SCP_LSHIFT) || kbd_ispressed(KBD_SCP_RSHIFT)) {
		if (_capslock == KBD_RELEASED && _hasupper(code)) {
			code = _uplow(code);
		}
	} else {
		if (_capslock == KBD_PRESSED && _hasupper(code)) {
			code = _uplow(code);
		}
	}

	return code;
}

u32 kbd_iskbd(u32 port)
{
	u32 result, type;

	result = ps2_detect(port, &type);
	if (result == E_NO) {
		return 0;
	}

	return type == PS2_DEVTYPE_KBD ||
			type == PS2_DEVTYPE_KBD_ANCIENT ||
			type == PS2_DEVTYPE_KBD_TRANS;
}

u32 kbd_scan_ispress(u32 scan)
{
	return scan < 0x80 && _codepage[scan] != '\0';
}

u32 kbd_scan_isrelease(u32 scan)
{
	return scan >= 0x80 && _codepage[scan] != '\0';
}

u32 kbd_ispressed(u32 code)
{
	return _codes_pressed[code & 0xFF] == KBD_PRESSED;
}

u32 kbd_isprintable(u32 code)
{
	return (code >= 32 && code < 128) || code == '\n' || code == '\r'
			|| code == '\t';
}

u32 kbd_tocode(u32 scan)
{
	return _codepage[scan % 0x80];
}

void int_keyboard()
{
	u32 scan = kbd_scan_nopoll();
	u32 code = kbd_tocode(scan);

	if (kbd_scan_ispress(scan)) {
		_codes_pressed[code] = KBD_PRESSED;
		if (_wait_next) {
			_next_char = code;
			_wait_next = 0;
		}

		switch (code) {
		case KBD_SCP_CAPS:
			_capslock = (_capslock == KBD_PRESSED ? KBD_RELEASED : KBD_PRESSED);
			break;
		}
	}
	else if (kbd_scan_isrelease(scan)) {
		_codes_pressed[code] = KBD_RELEASED;
	}

	pic_eoi(1);
}

u32 _isletter(u32 code)
{
	return code >= 'a' && code <= 'z';
}

u32 _isdigit(u32 code)
{
	return code >= '0' && code <= '9';
}

u32 _hasupper(u32 code)
{
	u32 ret = _isletter(code) || _isdigit(code);
	if (!ret) {
		switch (code) {
		case '`':
		case '_':
		case '=':
		case '[':
		case ']':
		case ';':
		case '\'':
		case ',':
		case '.':
		case '/':
			return 1;
		default:
			return 0;
		}
	} else {
		return 1;
	}
}

static const char* _uplowcodes = \
								/* These codes are in ASCII */
		" 1'3456\"908=<_>?"	/* 20 - 2F */
		")!@#$%^&*(;:,+./"		/* 30 - 3F */
		"2abcdefghijklmno"		/* 40 - 4F */
		"pqrstuvwxyz{|}6-"		/* 50 - 5F */
		"~ABCDEFGHIJKLMNO"		/* 60 - 6F */
		"PQRSTUVWXYZ[\\]`\0"	/* 70 - 7F */;

u32 _uplow(u32 code) {
	u32 new = code;
	if (code >= 32) {
		new = _uplowcodes[code - 32];
	}
	if (new == '\0') return code;
	else return new;
}
