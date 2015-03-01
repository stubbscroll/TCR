two small examples for using lua with C.

warning! http://luabinaries.sourceforge.net/ seems to be the most sensible
place to get mingw64 libraries, but there are problems. lua errors will
result in a crash instead of proper catching because of gcc optimizations.
instead, download the source from the official lua page, and in src/Makefile,
change the line

CFLAGS= -O2 -Wall -Wextra -DLUA_COMPAT_5_2 $(SYSCFLAGS) $(MYCFLAGS)

to

CFLAGS= -O2 -fno-omit-frame-pointer -Wall -Wextra -DLUA_COMPAT_5_2 $(SYSCFLAGS) $(MYCFLAGS)

and then run make. then, rename liblua.a to liblua53.a (or change make.bat in
this directory to use -llua instead of -llua53) and copy the lib file and the
necessary header files to the correct gcc directories. the fix is the same for
other versions of lua: add the -fno-omit-frame-pointer switch to CFLAGS.

sources:
http://stackoverflow.com/questions/19903762/lua-5-2-compiled-with-mingw32-crashes-my-host-program-when-a-lua-error-occurs
http://lua-users.org/lists/lua-l/2011-10/msg00549.html
