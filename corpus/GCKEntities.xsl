<?xml version="1.0" encoding="utf-8"?>
<!-- $Id$ -->
<!--
	Génération des 'locale/(...)/(...).dtd' à partir d'un unique fichier.
-->
<!DOCTYPE GCKEntries.xsl [
<!ENTITY nl "&#10;">
<!ENTITY tab "&#9;">
]>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:param name="Language"/>
	<xsl:output method="text" encoding="utf-8"/>
	<xsl:template match="//Entities">
		<xsl:apply-templates select="Entity[@Name]" mode="solo"/>
		<xsl:apply-templates select="EntitySet"/>
		<xsl:apply-templates select="Raw[@language=$Language]"/>
	</xsl:template>
	<xsl:template match="EntitySet">
		<xsl:apply-templates select="Entity" mode="set"/>
		<xsl:text>&nl;</xsl:text>
	</xsl:template>
	<xsl:template match="Entity" mode="solo">
		<xsl:apply-templates select="Translation[@language=$Language]" mode="solo"/>
		<xsl:text>&nl;</xsl:text>
	</xsl:template>
	<xsl:template match="Translation" mode="solo">
		<xsl:text>&lt;!ENTITY </xsl:text>
		<xsl:value-of select="../@Name"/>
		<xsl:text> "</xsl:text>
		<xsl:value-of select="."/>
		<xsl:text>"&gt;&nl;</xsl:text>
	</xsl:template>
	<xsl:template match="Raw">
		<xsl:value-of select="."/>
	</xsl:template>
	<xsl:template match="Entity" mode="set">
		<xsl:apply-templates select="Translation[@language=$Language]" mode="set"/>
	</xsl:template>
	<xsl:template match="Translation" mode="set">
		<xsl:text>&lt;!ENTITY </xsl:text>
		<xsl:value-of select="../../@Affix"/>
		<xsl:text>.</xsl:text>
		<xsl:value-of select="../@Suffix"/>
		<xsl:text> "</xsl:text>
		<xsl:value-of select="."/>
		<xsl:text>"&gt;&nl;</xsl:text>
	</xsl:template>
</xsl:stylesheet>
