<!--$Id$-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
 <xsl:template match="CVS">
The CVS repository can be checked out through anonymous (pserver) CVS with the following instruction set. When prompted for a password for <em>anonymous</em>, simply press the Enter key.<P/>
<tt>cvs -d:pserver:anonymous@<xsl:value-of select="@server"/>:<xsl:value-of select="@root"/> login</tt><br/>
<tt>cvs -z3 -d:pserver:anonymous@<xsl:value-of select="@server"/>:<xsl:value-of select="@root"/> co <xsl:value-of select="@module"/></tt><P/>
 <xsl:apply-templates select="link"/>
 </xsl:template>
  <xsl:template match="CVS/link">
  You can also directly browse the CVS repository by following this 
    <a href="{@URL}"><xsl:attribute name="target">_blank</xsl:attribute>link</a>
   (<a href="{@URL}">
          <xsl:attribute name="target">_blank</xsl:attribute>
        <xsl:value-of select="@URL"/>
      </a>).
  </xsl:template>
</xsl:stylesheet>
