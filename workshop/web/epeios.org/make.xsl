
<!--$Id$-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="html"/>
  <xsl:param name="os"/>
  <xsl:template match="/" xml:space="preserve">include Makefile.header

all:
<xsl:apply-templates select="//document/specific/WEB"/></xsl:template>
  <xsl:template match="document/specific/WEB" xml:space="preserve">	$(mkdir) -p $(dest)/<xsl:value-of select="@hierarchy"/>
	sabcmd file://$(templates)/epeios.org/page.xsl i/site.xml $(dest)/<xsl:value-of select="@hierarchy"/>/<xsl:value-of select="@name"/>.html <xsl:choose>
      <xsl:when test="$os='windows'">"</xsl:when>
      <xsl:when test="$os='windows'">'</xsl:when>
    </xsl:choose>$$hierarchy=<xsl:value-of select="@hierarchy"/><xsl:choose>
      <xsl:when test="$os='windows'">"</xsl:when>
  <xsl:when test="$os='windows'">'</xsl:when>
 </xsl:choose><xsl:text> </xsl:text><xsl:choose>
  <xsl:when test="$os='windows'">"</xsl:when>
  <xsl:when test="$os='windows'">'</xsl:when>
</xsl:choose>$$name=<xsl:value-of select="@name"/><xsl:choose>
  <xsl:when test="$os='windows'">"</xsl:when>
  <xsl:when test="$os='windows'">'</xsl:when>
</xsl:choose><xsl:text>
</xsl:text></xsl:template>
</xsl:stylesheet>