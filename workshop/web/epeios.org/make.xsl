
<!--$Id$-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="html"/>
  <xsl:template match="/" xml:space="preserve">include Makefile.header

all:
<xsl:apply-templates select="//document/specific/WEB"/></xsl:template>
  <xsl:template match="document/specific/WEB" xml:space="preserve">	$(mkdir) -p $(dest)/<xsl:value-of select="@hierarchy"/>
	sabcmd file://$(templates)/epeios.org/page.xsl i/site.xml $(dest)/<xsl:value-of select="@hierarchy"/>/<xsl:value-of select="@name"/>.html "$$hierarchy=<xsl:value-of select="@hierarchy"/> $$name=<xsl:value-of select="@name"/>"<xsl:text>
</xsl:text></xsl:template>
</xsl:stylesheet>
