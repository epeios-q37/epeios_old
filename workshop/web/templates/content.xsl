<!--$Id$-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="content">
		<xsl:apply-templates select="warning" mode="warning"/>
			<xsl:element name="ol">
				<xsl:apply-templates select="chapter" mode="index"/>
			</xsl:element>
		<xsl:apply-templates/>
	</xsl:template>
</xsl:stylesheet>
