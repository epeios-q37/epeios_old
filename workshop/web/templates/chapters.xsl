<!--$Id$-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="chapter" mode="index">
		<xsl:element name="li">
			<xsl:element name="a">
				<xsl:attribute name="href">
					<xsl:text>#</xsl:text>
					<xsl:value-of select="generate-id()"/>
				</xsl:attribute>
				<xsl:value-of select="@title"/>
			</xsl:element>
			<xsl:element name="ol">
				<xsl:apply-templates select="chapter" mode="index"/>
			</xsl:element>
		</xsl:element>
	</xsl:template>
	<xsl:template match="chapter">
		<xsl:element name="h2">
			<xsl:attribute name="style">text-decoration: underline;</xsl:attribute>
			<xsl:element name="a">
				<xsl:attribute name="name">
					<xsl:value-of select="generate-id()"/>
				</xsl:attribute>
			</xsl:element>
			<xsl:value-of select="@title"/>
		</xsl:element>
		<xsl:apply-templates/>
	</xsl:template>
	<xsl:template match="chapter/chapter">
		<xsl:element name="h3">
			<xsl:element name="a">
				<xsl:attribute name="name">
					<xsl:value-of select="generate-id()"/>
				</xsl:attribute>
			</xsl:element>
			<xsl:value-of select="@title"/>
		</xsl:element>
		<xsl:apply-templates/>
	</xsl:template>
	<xsl:template match="chapter/chapter/chapter">
		<xsl:element name="h4">
			<xsl:element name="blockquote">
				<xsl:element name="a">
					<xsl:attribute name="name">
						<xsl:value-of select="generate-id()"/>
					</xsl:attribute>
				</xsl:element>
				<xsl:value-of select="@title"/>
			</xsl:element>
		</xsl:element>
		<xsl:apply-templates/>
	</xsl:template>
</xsl:stylesheet>