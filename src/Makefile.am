
libexec_PROGRAMS = whiteboard-sib-access-plain-nota
libexecdir = $(prefix)/lib/whiteboard/libexec

# Compiler flags
whiteboard_sib_access_plain_nota_CFLAGS  = -Wall -I$(top_srcdir)/include
whiteboard_sib_access_plain_nota_CFLAGS += @LIBWHITEBOARD_CFLAGS@ @LIBSSAP_PNG_CFLAGS@ @NOTA_CFLAGS@
whiteboard_sib_access_plain_nota_CFLAGS += @GNOME_CFLAGS@ 
whiteboard_sib_access_plain_nota_LDFLAGS = @LIBWHITEBOARD_LIBS@ @LIBSSAP_PNG_LIBS@ @GNOME_LIBS@ @NOTA_LIBS@

AM_CFLAGS = $(whiteboard_sib_access_plain_nota_CFLAGS) -g

# Put these in alphabetical order so that they are easy to find.
whiteboard_sib_access_plain_nota_SOURCES = \
	main.c \
	serverthread.c \
	sib_access.c \
	sib_controller.c \
	sib_server.c \
	sib_service.c 
