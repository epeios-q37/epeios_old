
<!--$Id$-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:template match="list">
    <xsl:value-of select="title"/>
    <UL>
      <xsl:apply-templates select="item"/>
    </UL>
  </xsl:template>
  <xsl:template match="item">
    <LI>
      <xsl:apply-templates/>
    </LI>
  </xsl:template>
  <xsl:include href="../misc.xsl"/>
</xsl:stylesheet>