# $Id$

incp := ..

include $(incp)/Makefile.header

all: $(dest)/index.html

$(dest)/index.html: $(dest) $(rootxsl) i/index.xml
	sabcmd $(rootxsl) i/index.xml $@

i/index.xml: i $(xsl) $(doc)/software/mll2html/manual.xml i/index.crp
	reveal i/index.crp $(templates)/page.xml $@

include $(incp)/Makefile.footer
	
