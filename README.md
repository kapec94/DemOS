DemOS
=====

This is an __university project__. Pretty big one, actually.

![](https://github.com/kapec94/DemOS/raw/master/demos.png "Hello!")

There aren't many projects out there which functionality can be fully described with one screenshot, are there? Well, that's the whole frontend currently available with only one simple command to call. But still DemOS can do a thing or two.

Functionality
-------------

 - Hardware and software interrupts handling
 - COM port driver
 - PS/2 driver and keyboard support
 - Segment-based runtime environment (with no means of loading nor executing any programs so far)
 - Simple conio-like console input/output API
 - VGA text mode driver with support for cursor and text color attributes

What's next?
----------------

So far that's everything I was able to implement. Next part will be implementing IDE (or SATA, if I'd feel adventurous) driver and support for VFAT filesystem. With this, I'll be able to implement executable file loader. Then I'll provide means of communication with user-space programs (through interrupts, most probably) and then... We'll see.

For now, though, I need to suspend my work on this project. I've got some other work that needs to be done, sadly. But having so serious basis, I'll sure come back to this project one day.