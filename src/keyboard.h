/*
 * keyboard.h
 *
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

/* Safe to run before kbd_init was called. Checks if the keyboard is connected
 * to the specified PS/2 port. */
u32 kbd_iskbd(u32 ps2port);

u32 kbd_init(u32 ps2port);

u32 kbd_ispress(u32 scan);
u32 kbd_isrelease(u32 scan);

u32 kbd_tocode(u32 scan);

u32 kbd_scan_nopoll();
u32 kbd_code_nopoll();

#endif /* KEYBOARD_H_ */
