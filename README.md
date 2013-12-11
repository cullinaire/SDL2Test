SDL2Test
========

Testing various systems of SDL 2.0

Starting out with bitmap text routines, followed by menuing systems, then input.
From there I'm not sure where I'll go next but it's sure to be titillating!

Oh that's right, this project requires SDL 2.0. Currently does not use any extensions. Probably uses less than 1GB of memory.

See

	http://nothingtocode.blogspot.com/2013/07/setting-up-sdl2-in-ubuntu-or-linux-mint.html

for help in installing the bleeding edge SDL on your system. Yes, it worked for me.

Also requires CML 1.0.3. www.cmldev.net

Added a cmake config for GNU/Linux building. Create a "bin" subdirectory from the main project path, chdir in there and type:

	cmake ..

Hopefully everything will go swell and a bunch of files will show up in the /bin directory. One of them should be the Makefile. Now you can run make and the executable should pop up for you to enjoy.
