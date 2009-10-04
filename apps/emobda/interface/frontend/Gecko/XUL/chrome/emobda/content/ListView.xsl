<?xml version="1.0" encoding="ISO-8859-1"?>

<!--
	'Recordform.xsl' by Claude SIMON (http://zeusw.org/).

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

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns="http://www.mozilla.org/keymaster/gatekeeper/there.is.only.xul">
	<xsl:include href="Library.xsl"/>
	<xsl:output method="xml" indent="yes" encoding="ISO-8859-1" omit-xml-declaration="yes" standalone="yes"/>
	<xsl:template match="/emobda">
		<xsl:variable name="CurrentTable">
			<xsl:call-template name="GetCurrentTable"/>
		</xsl:variable>
		<xsl:apply-templates select="Structure">
			<xsl:with-param name="Table" select="$CurrentTable"/>
		</xsl:apply-templates>
		<xsl:apply-templates select="Content">
			<xsl:with-param name="Table" select="$CurrentTable"/>
		</xsl:apply-templates>
	</xsl:template>
	<xsl:template match="Structure">
		<xsl:param name="Table"/>
		<xsl:apply-templates select="Fields">
			<xsl:with-param name="Table" select="$Table"/>
		</xsl:apply-templates>
	</xsl:template>
	<xsl:template match="Fields">
		<xsl:param name="Table"/>
		<xsl:element name="treecols">
			<xsl:element name="treecol">
				<!--xsl:attribute name="id">
					<xsl:value-of select="@Row"/>
				</xsl:attribute-->
				<xsl:attribute name="sortDirection">descending</xsl:attribute>
				<xsl:attribute name="label">Id</xsl:attribute>
				<xsl:attribute name="primary">true</xsl:attribute>
				<xsl:attribute name="flex">1</xsl:attribute>
			</xsl:element>
			<xsl:apply-templates select="Field[@TableRow=$Table]"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Field">
		<xsl:element name="treecol">
			<xsl:attribute name="id">
				<xsl:value-of select="@Row"/>
			</xsl:attribute>
			<xsl:attribute name="label">
				<xsl:value-of select="Name"/>
			</xsl:attribute>
			<xsl:attribute name="flex">1</xsl:attribute>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Content">
		<xsl:apply-templates select="Records"/>
	</xsl:template>
	<xsl:template match="Records">
		<xsl:element name="treechildren">
			<xsl:apply-templates select="Record"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Record">
		<xsl:element name="treeitem">
			<xsl:attribute name="Record">
				<xsl:value-of select="@Id"/>
			</xsl:attribute>
			<xsl:attribute name="RecordPosition">
				<xsl:value-of select="position()"/>
			</xsl:attribute>
			<xsl:element name="treerow">
				<xsl:apply-templates select="Data"/>
			</xsl:element>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Data">
		<xsl:element name="treecell">
			<xsl:attribute name="label">
				<xsl:value-of select="../@Id"/>
			</xsl:attribute>
		</xsl:element>
		<xsl:apply-templates select="Datum"/>
	</xsl:template>
	<xsl:template match="Datum">
		<xsl:element name="treecell">
			<xsl:attribute name="label">
				<xsl:value-of select="."/>
			</xsl:attribute>
		</xsl:element>
	</xsl:template>
</xsl:stylesheet>
