# The following variables may be changed on the make command line:
#
# the directory to the strongSwan sources
SWANDIR?=~/strongswan
# location of config.h
CONFIGH?=$(SWANDIR)/config.h
# default install prefix
PREFIX?=/usr/local
# location of the installed strongSwan libraries
SWANLIBS?=$(PREFIX)/lib/ipsec
# location of the strongSwan plugins
SWANPLUGINS?=$(PREFIX)/lib/ipsec/plugins
# target location of the plugin config snippet
PLUGINCONF?=/etc/strongswan.d/charon/

CFLAGS=-O2 -Wall -fpic \
		-include $(CONFIGH) \
		-I$(SWANDIR)/src/libstrongswan \
		-I$(SWANDIR)/src/libcharon

LDFLAGS=-L$(SWANLIBS) \
	-lstrongswan \
	-lcharon

redirect.so: redirect.o
	gcc -shared -o $@ $< $(LDFLAGS)

%.o: %.c
	gcc -c $< -o $@ $(CFLAGS)

install: redirect.so redirect.conf
	cp redirect.so $(SWANPLUGINS)/libstrongswan-redirect.so
	cp redirect.conf $(PLUGINCONF)

clean:
	rm -f *.so *.o

.PHONY: clean install
