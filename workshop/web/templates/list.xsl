<!--$Id$-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="list">
		<xsl:value-of select="title"/>
		<xsl:element name="ul">
			<xsl:apply-templates select="item"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="item">
		<xsl:element name="li">
			<xsl:apply-templates/>
		</xsl:element>
	</xsl:template>
	<xsl:include href="misc.xsl"/>
</xsl:stylesheet>