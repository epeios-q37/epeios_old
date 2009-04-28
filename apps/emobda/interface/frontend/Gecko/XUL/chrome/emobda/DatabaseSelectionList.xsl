<?xml version="1.0" encoding="ISO-8859-1"?>

<!--
	'DatabaseSelectionList.xsl' by Claude SIMON (http://zeusw.org/).

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
	<xsl:template match="/emobda/Available/Databases">
		<xsl:element name="treecols">
			<xsl:element name="treecol">
				<xsl:attribute name="id">Path</xsl:attribute>
				<xsl:attribute name="label">Path</xsl:attribute>
				<xsl:attribute name="flex">1</xsl:attribute>
			</xsl:element>
		</xsl:element>
		<xsl:element name="treechildren">
			<xsl:apply-templates select="Database"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Database">
		<xsl:element name="treeitem">
			<xsl:element name="treerow">
				<xsl:element name="treecell">
					<xsl:attribute name="label">
						<xsl:value-of select="Path"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:element>
		</xsl:element>
	</xsl:template>
</xsl:stylesheet>
