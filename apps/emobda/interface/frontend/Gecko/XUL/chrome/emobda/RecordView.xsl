<?xml version="1.0" encoding="ISO-8859-1"?>

<!--
	'RecordView.xsl' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'emobda' software.

	'emobda' is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	'emobda' is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with 'emobda'.  If not, see <http://www.gnu.org/licenses/>.
-->

<!-- $Id$ -->

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns="http://www.w3.org/1999/xhtml">
	<xsl:include href="Library.xsl"/>
	<xsl:output method="xml" indent="yes" encoding="ISO-8859-1" omit-xml-declaration="yes" standalone="yes"/>
	<xsl:template match="/emobda">
		<xsl:apply-templates select="Content"/>
	</xsl:template>
	<xsl:template match="Content">
		<xsl:element name="html">
			<xsl:element name="h1">
				<xsl:apply-templates select="Content"/>
			</xsl:element>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Content">
		<xsl:apply-templates select="Records"/>
	</xsl:template>
	<xsl:template match="Records">
		<xsl:element name="table">
			<xsl:apply-templates select="Record"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Record">
		<xsl:apply-templates select="Data"/>
	</xsl:template>
	<xsl:template match="Data">
		<xsl:apply-templates select="Datum"/>
	</xsl:template>
	<xsl:template match="Datum">
		<xsl:element name="tr">
			<xsl:element name="td">
				<xsl:call-template name="GetFieldWording">
					<xsl:with-param name="Row" select="@Field"/>
				</xsl:call-template>
			</xsl:element>
			<xsl:text> : </xsl:text>
			<xsl:element name="td">
				<xsl:value-of select="."/>
			</xsl:element>
		</xsl:element>
	</xsl:template>
</xsl:stylesheet>
