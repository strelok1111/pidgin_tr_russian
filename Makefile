GLIB_CFLAGS = -pthread -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/lib/x86_64-linux-gnu/glib-2.0/include/ -I/var/projects/c_and_cpp_expirements/c/pidgin-2.10.7/libpurple -I/var/projects/c_and_cpp_expirements/c/pidgin-2.10.7/pidgin -I/usr/include/gtk-2.0 -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/lib/x86_64-linux-gnu/gtk-2.0/include -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/atk-1.0
GLIB_LIBS = -Wl,--export-dynamic -pthread -lgobject-2.0 -lgmodule-2.0 -ldl -lgthread-2.0 -lglib-2.0

translate_string.so: translate_string.c
	g++ -shared -o translate_string.so translate_string.c $(GLIB_CFLAGS) $(GLIB_LIBS) 

clean:
	find . -name "*.*~" -exec rm {} \;
	find . -name "translate_string.so" -exec rm {} \;
	find . -name "translate_string.la" -exec rm {} \;
	find /home/user/.purple/plugins/ -name "translate_string.so" -exec rm {} \;
