<!--$Id$-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="download">
		<xsl:text>Files to download can be found following this </xsl:text>
		<xsl:element name="a">
			<xsl:attribute name="href">
				<xsl:value-of select="@URL"/>
			</xsl:attribute>
			<xsl:text>link</xsl:text>
		</xsl:element>
		<xsl:text> (</xsl:text>
		<xsl:element name="a">
			<xsl:attribute name="href">
				<xsl:value-of select="@URL"/>
			</xsl:attribute>
			<xsl:value-of select="@URL"/>
		</xsl:element>
		<xsl:text>).</xsl:text>
	</xsl:template>
</xsl:stylesheet>