
<!--$Id$-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="html"/>
  <xsl:template match="/">
    <content>
      <list>
        <xsl:apply-templates select="site/*"/>
      </list>
    </content>
  </xsl:template>
  <xsl:template match="document">
    <item>
      <link>
        <xsl:attribute name="URL">
          <xsl:value-of select="specific/WEB/@hierarchy"/>/<xsl:value-of select="specific/WEB/@name"/>.html</xsl:attribute>
        <xsl:value-of select="general/@title"/>
      </link> : <xsl:value-of select="general/@abstract"/></item>
  </xsl:template>
  <xsl:template match="dir">
    <item>
      <xsl:value-of select="@id"/>
      <list>
        <xsl:apply-templates select="document"/>
        <xsl:apply-templates select="dir"/>
      </list>
    </item>
  </xsl:template>
  <xsl:template match="template"/>
</xsl:stylesheet>