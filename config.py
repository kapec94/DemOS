#!/usr/bin/env python

## CONFIGURATION 
# Contains values and definitions you are free to change.
# Be careful, though, some values may cause the kernel to (hopefully) crash
# or (if you're not so lucky) misbehave.

# BASE CONFIG
# This values will be available for the system through generated config files.
config = dict(
	# Where the kernel is to be loaded in the address space.
	# Note that OS image is built that .header, .text and .data sections are
	# consecutive and this field only specifies, where the .header section
	# will be loaded.
	kernel_load_address='0x00104000',
	
	# Memory address at which stack will be set up.
	stack_base='0x00104000',

	# The address space is ordered in segment packs. Every process has its stack
	# segment, code segment and data segment, which sizes are configurable through
	# below defines.
	segment_pack_size='0x10',

	# stack segment size - 8kB
	ss_size='0x2000',
	# code segment size - 32kB
	cs_size='0x8000',
	# data segment size - 32kB
	ds_size='0x8000',

	# Defines how many entries can contain the following table.
	gdt_size='0x1FF',
	idt_size='0x100',

	# offset of first hardware interrupt handler in IDT
	irq_offset='0x20',
)

## CONFIGURATION ENDS HERE

import sys
import os

# GENERATORS
# These generate config files. Below you'll find generating functions and even
# lower there's mapping between file extensions and these functions.
def gen_c_header(conf):
	config_h = """
#ifndef CONFIG_H_
#define CONFIG_H_

{content}

#endif"""

	lines = ["#define %s %s" % (name, val) for name, val in conf.items()]
	return config_h.format(content='\n'.join(lines))

def gen_ld_script(conf):
	return '\n'.join("%s = %s ;" % (name, val) for name, val in conf.items())

generators = {
	'.h': gen_c_header,
	'.ld': gen_ld_script
}

def _die(msg):
	print(msg, file=sys.stderr)
	sys.exit(1)

if __name__ == "__main__":
	assert len(sys.argv) > 1, "No args provided!"
	arg = sys.argv[1]
	if arg == "conf":
		assert len(sys.argv) == 3, "Wrong number of args!"
		name = sys.argv[2]
		
		conf = dict((name.upper(), val) for name, val in config.items())
		_, ext = os.path.splitext(name)
		
		try:
			gen = generators[ext]
		except KeyError:
			_die("Generator for extension %s not found!" % ext)
		
		with open(name, 'w') as f:
			f.write(gen(conf))
		sys.exit(0)
	elif arg == "install":
		from paramiko.client import SSHClient
	
		assert len(sys.argv) != 4, "Invalid number of args!"
		local, remote = sys.argv[2:3]
		assert os.path.isfile(local), "%s must be readable file!" % local
		
		client = SSHClient()
		sftp = None
		try:
			client.connect(**ssh_conf)
			client.exec_command(ACQUIRE_COMMAND)
			
			sftp = client.open_sftp()
			sftp.chdir(ROOT_DIR)
			sftp.put(local, '.' + remote)
		finally:
			if sftp is not None:
				sftp.close()
			client.exec_command(RELEASE_COMMAND)
			client.close()
	else:
		_die("Unknown command: %s" % arg)
