<!--$Id$-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="rss">
		<xsl:element name="table">
			<xsl:attribute name="align">center</xsl:attribute>
			<xsl:attribute name="width">95%</xsl:attribute>
			<xsl:attribute name="border">1</xsl:attribute>
			<xsl:element name="tr">
				<xsl:element name="td">
					<xsl:apply-templates select="channel"/>
				</xsl:element>
			</xsl:element>
		</xsl:element>
	</xsl:template>
	<xsl:template match="channel">
		<xsl:apply-templates select="item"/>
	</xsl:template>
	<xsl:template match="channel/item">
		<xsl:element name="i">
			<xsl:value-of select="pubDate"/>
		</xsl:element>
		<xsl:element name="br"/>
		<xsl:element name="a">
			<xsl:attribute name="href">
				<xsl:value-of select="link"/>
			</xsl:attribute>
			<xsl:element name="b">
				<xsl:value-of select="title"/>
			</xsl:element>
		</xsl:element>
		<xsl:element name="br"/>
		<xsl:element name="div">
			<xsl:attribute name="style">text-indent: 5%; text-align: justify; margin: 0px;</xsl:attribute>
			<xsl:value-of select="description"/>
		</xsl:element>
	</xsl:template>
</xsl:stylesheet>
