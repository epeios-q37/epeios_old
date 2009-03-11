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
	<xsl:include href="Library.xsl"/>
	<xsl:output method="xml" indent="yes" encoding="ISO-8859-1" omit-xml-declaration="yes" standalone="yes"/>
	<xsl:template match="/emobda">
		<xsl:apply-templates select="Structure"/>
	</xsl:template>
	<xsl:template match="Structure">
		<xsl:element name="menupopup">
			<xsl:choose>
				<xsl:when test="Database">
					<xsl:apply-templates select="Database"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:element name="menuitem">
						<xsl:attribute name="disabled">true</xsl:attribute>
						<xsl:attribute name="label">|Empty|</xsl:attribute>
					</xsl:element>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Database">
		<xsl:apply-templates select="Tables"/>
	</xsl:template>
	<xsl:template match="Tables">
		<xsl:apply-templates select="Table">
			<xsl:with-param name="CurrentTable">
				<xsl:call-template name="GetCurrentTable"/>
			</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	<xsl:template match="Table">
		<xsl:param name="CurrentTable"/>
		<xsl:element name="menuitem">
			<xsl:attribute name="type">radio</xsl:attribute>
			<xsl:attribute name="label">
				<xsl:value-of select="Name"/>
			</xsl:attribute>
			<xsl:attribute name="Row">
				<xsl:value-of select="@Row"/>
			</xsl:attribute>
			<xsl:if test="@Row=$CurrentTable">
				<xsl:attribute name="checked">true</xsl:attribute>
			</xsl:if>
			<xsl:if test="number(Fields/@Amount)=0">
				<xsl:attribute name="disabled">true</xsl:attribute>
			</xsl:if>
		</xsl:element>
	</xsl:template>
</xsl:stylesheet>
