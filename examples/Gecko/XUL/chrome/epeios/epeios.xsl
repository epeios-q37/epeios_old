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
		<xsl:element name="a">
			<!--xsl:attribute name="onclick">alert( "direct" );</xsl:attribute-->
			<xsl:attribute name="command">cmdJS</xsl:attribute>
			<xsl:text>Link JS</xsl:text>
		</xsl:element>
		<xsl:element name="a">
			<!--xsl:attribute name="onclick">alert( "direct" );</xsl:attribute-->
			<xsl:attribute name="command">cmdXPCOM</xsl:attribute>
			<xsl:text>Link XPCOM</xsl:text>
		</xsl:element>
		<xsl:element xmlns="http://www.mozilla.org/keymaster/gatekeeper/there.is.only.xul" name="vbox>">
			<xsl:element name="button">
				<xsl:attribute name="label">COUUCOU</xsl:attribute>
				<xsl:attribute name="command">cmdXPCOM</xsl:attribute>
			</xsl:element>
			<xsl:element name="description">
				<xsl:attribute name="command">cmdXPCOM</xsl:attribute>
				</xsl:text>CLICK !</xsl:text>
			</xsl:element>
		</xsl:element>
	</xsl:template>
</xsl:stylesheet>
