<!--$Id$-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="xml" omit-xml-declaration="yes"/>
	<xsl:template match="/">
		<xsl:element name="content">
			<xsl:element name="list">
				<xsl:apply-templates select="site/*"/>
			</xsl:element>
		</xsl:element>
	</xsl:template>
	<xsl:template match="document">
		<xsl:element name="item">
			<xsl:element name="link">
				<xsl:attribute name="URL">
					<xsl:value-of select="specific/WEB/@hierarchy"/>
					<xsl:text>/</xsl:text>
					<xsl:value-of select="specific/WEB/@name"/>
					<xsl:text>.html</xsl:text>
				</xsl:attribute>
				<xsl:value-of select="general/@title"/>
			</xsl:element>
			<xsl:text> - </xsl:text>
			<xsl:element name="raw">
				<xsl:value-of select="specific/WEB/@bookmark"/>
			</xsl:element>
			<xsl:text> : </xsl:text>
			<xsl:value-of select="general/@abstract"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="dir">
		<xsl:element name="item">
			<xsl:value-of select="@id"/>
			<xsl:element name="list">
				<xsl:apply-templates/>
			</xsl:element>
		</xsl:element>
	</xsl:template>
	<xsl:template match="template"/>
</xsl:stylesheet>