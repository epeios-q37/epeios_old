
<!--$Id$-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:template match="section">
    <xsl:apply-templates/>
    <P/>
  </xsl:template>
  <xsl:template match="para">
    <xsl:if test="@type='remark'">
      <xsl:text disable-output-escaping="yes">&lt;em></xsl:text>
    </xsl:if>
    <xsl:apply-templates/>
    <xsl:if test="@type='remark'">
      <xsl:text disable-output-escaping="yes">&lt;/em></xsl:text>
    </xsl:if>
    <br/>
  </xsl:template>
 <xsl:template match="link">
    <a href="{@URL}">
      <xsl:if test="@type='external'">
        <xsl:attribute name="target">_blank</xsl:attribute>
      </xsl:if>
      <xsl:apply-templates/>
    </a>
    <xsl:if test="@aspect='long'" xml:space="preserve"> (<a href="{@URL}">
        <xsl:if test="@type='external'">
          <xsl:attribute name="target">_blank</xsl:attribute>
        </xsl:if>
        <xsl:value-of select="@URL"/>
      </a>)</xsl:if>
  </xsl:template>
  <xsl:template match="raw">
    <TT>
      <strong>
        <xsl:value-of select="."/>
      </strong>
    </TT>
  </xsl:template>
  <xsl:template match="source">
    <pre>
      <xsl:value-of select="."/>
    </pre>
  </xsl:template>
  <xsl:template match="specific/WEB/parent">
    <a>
      <xsl:attribute name="href">
        <xsl:value-of select="."/>
      </xsl:attribute>Parent</a>
  </xsl:template>
  <xsl:template match="contact">
    <a>
      <xsl:attribute name="href">mailto:<xsl:value-of select="@adress"/></xsl:attribute>
      <xsl:value-of select="."/>
    </a>
    <xsl:if test="@type='long'" xml:space="preserve"> (<a>
        <xsl:attribute name="href">mailto:<xsl:value-of select="@adress"/></xsl:attribute>
        <xsl:value-of select="@adress"/>
      </a>)</xsl:if>
  </xsl:template>
  <xsl:template match="ignore"/>
</xsl:stylesheet>
