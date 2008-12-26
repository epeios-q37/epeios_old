<?xml version="1.0" encoding="ISO-8859-1"?>
<!-- $Id$ -->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="html" indent="yes" encoding="ISO-8859-1"/>
	<xsl:param name="Value"/>
	<xsl:template match="Root">
		<xsl:text>XSL transformation succeed !</xsl:text>
		<xsl:element name="hr"/>
		<xsl:text>Value of the above box : </xsl:text>
		<xsl:element name="font">
			<xsl:attribute name="style">color: red; font-weight: bold; font-style: italic;</xsl:attribute>
			<xsl:value-of select="$Value"/>
		</xsl:element>
	</xsl:template>
</xsl:stylesheet>
