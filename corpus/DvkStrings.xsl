<?xml version="1.0" encoding="ISO-8859-1"?>
<!-- $Id$ -->
<!--
	Génération des 'res/values-(...)/strings.xml' à partir d'un unique fichier.
-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:param name="Language"/>
	<xsl:output method="xml" indent="yes" encoding="ISO-8859-1"/>
	<xsl:template match="//Strings">
		<xsl:element name="resources">
			<xsl:attribute name="test">
				<xsl:value-of select="$Language"/>
			</xsl:attribute>
			<xsl:apply-templates select="String"/>
			<xsl:apply-templates select="String-Array"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="String">
			<xsl:apply-templates select="Translation[@language=$Language]" mode="String"/>
	</xsl:template>
	<xsl:template match="Translation" mode="String">
		<xsl:element name="string">
			<xsl:attribute name="name">
				<xsl:value-of select="../@name"/>
			</xsl:attribute>
			<xsl:value-of select="."/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="String-Array">
		<xsl:element name="string-array">
			<xsl:attribute name="name">
				<xsl:value-of select="@name"/>
			</xsl:attribute>
			<xsl:apply-templates select="Item"/>
		</xsl:element>	</xsl:template>
	<xsl:template match="Item">
		<xsl:apply-templates select="Translation[@language=$Language]" mode="String-Array"/>
	</xsl:template>
	<xsl:template match="Translation" mode="String-Array">
		<xsl:element name="item">
			<xsl:value-of select="."/>
		</xsl:element>
	</xsl:template>
</xsl:stylesheet>
