
<!--$Id$-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:template match="chapter" mode="index">
    <LI>
      <a href="#{generate-id()}">
        <xsl:value-of select="@title"/>
      </a>
      <OL>
        <xsl:apply-templates select="chapter" mode="index"/>
      </OL>
    </LI>
  </xsl:template>
  <xsl:template match="chapter">
    <H2>
      <U>
        <a name="{generate-id()}"/>
        <xsl:value-of select="@title"/>
      </U>
    </H2>
    <xsl:apply-templates/>
  </xsl:template>
  <xsl:template match="chapter/chapter">
    <H3>
      <a name="{generate-id()}"/>
      <xsl:value-of select="@title"/>
    </H3>
    <xsl:apply-templates/>
  </xsl:template>
  <xsl:template match="chapter/chapter/chapter">
    <H4>
      <blockquote>
        <a name="{generate-id()}"/>
        <xsl:value-of select="@title"/>
      </blockquote>
    </H4>
    <xsl:apply-templates/>
  </xsl:template>
</xsl:stylesheet>