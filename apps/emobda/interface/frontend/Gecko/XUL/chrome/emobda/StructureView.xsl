<?xml version="1.0" encoding="ISO-8859-1"?>

<!--
	'StructureView.xsl' by Claude SIMON (http://zeusw.org/).

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
	<xsl:output method="xml" indent="yes" encoding="ISO-8859-1" omit-xml-declaration="yes" standalone="yes"/>
	<xsl:template match="Database">
		<xsl:element name="treeitem">
			<xsl:attribute name="container">true</xsl:attribute>
			<xsl:attribute name="Type">Database</xsl:attribute>
			<xsl:element name="treerow">
				<xsl:element name="treecell">
					<xsl:attribute name="label">
						<xsl:value-of select="Name"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:element>
			<xsl:element name="treechildren">
				<xsl:apply-templates select="Tables"/>
			</xsl:element>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Tables">
		<xsl:apply-templates select="Table"/>
	</xsl:template>
	<xsl:template match="Table">
		<xsl:element name="treeitem">
			<xsl:attribute name="container">true</xsl:attribute>
			<xsl:attribute name="Type">Table</xsl:attribute>
			<xsl:element name="treerow">
				<xsl:element name="treecell">
					<xsl:attribute name="label">
						<xsl:value-of select="@Name"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:element>
			<xsl:element name="treechildren">
				<xsl:apply-templates select="Fields"/>
			</xsl:element>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Fields">
		<xsl:apply-templates select="Field"/>
	</xsl:template>
	<xsl:template match="Field">
		<xsl:element name="treeitem">
			<xsl:attribute name="label">TOTO</xsl:attribute>
			<xsl:attribute name="Type">Field</xsl:attribute>
			<xsl:element name="treerow">
				<xsl:element name="treecell">
					<xsl:attribute name="label">
						<xsl:value-of select="@Name"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:element>
		</xsl:element>
	</xsl:template>
</xsl:stylesheet>
