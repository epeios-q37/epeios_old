# $Id$

incp := ..

include $(incp)/Makefile.header

dest := $(dest)/software/mll2html

all: $(dest)/mll2html.html

$(dest)/mll2html.html: $(dest) $(rootxsl) i/mll2html.xml
	sabcmd file://$(rootxsl) i/mll2html.xml $@

i/mll2html.xml: i $(xsl) $(doc)/software/mll2html/manual.xml i/mll2html.crp
	reveal i/mll2html.crp $(templates)/page.xml $@
	
include $(incp)/Makefile.footer
	
