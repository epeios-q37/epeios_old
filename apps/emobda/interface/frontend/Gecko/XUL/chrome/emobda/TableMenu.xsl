<?xml version="1.0" encoding="ISO-8859-1"?>

<!--
	'TableMenu.xsl' by Claude SIMON (http://zeusw.org/).

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
	<xsl:template match="Structure">
		<xsl:apply-templates select="Database"/>
	</xsl:template>
	<xsl:template match="Database">
		<xsl:apply-templates select="Tables"/>
	</xsl:template>
	<xsl:template match="Tables">
		<xsl:element name="menupopup">
			<xsl:apply-templates select="Table"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Table">
		<xsl:element name="menuitem">
			<xsl:attribute name="type">radio</xsl:attribute>
			<xsl:attribute name="label">
				<xsl:value-of select="Name"/>
			</xsl:attribute>
		</xsl:element>
	</xsl:template>
</xsl:stylesheet>
