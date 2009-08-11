<?xml version="1.0" encoding="ISO-8859-1"?>

<!--
	'Library.xsl' by Claude SIMON (http://zeusw.org/).

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
	<xsl:template name="GetContext">
		<xsl:value-of select="/emobda/Current/@Context"/>
	</xsl:template>
	<xsl:template name="IsContext">
		<xsl:param name="ContextLabel"/>
		<xsl:variable name="Context">
			<xsl:call-template name="GetContext"/>
		</xsl:variable>
		<xsl:value-of select="$Context=$ContextLabel"/>
	</xsl:template>
	<xsl:template name="IsContextStandard">
		<xsl:call-template name="IsContext">
			<xsl:with-param name="ContextLabel">Standard</xsl:with-param>
		</xsl:call-template>
	</xsl:template>
	<xsl:template name="GetCurrentTable">
		<xsl:value-of select="/emobda/Current/@Table"/>
	</xsl:template>
	<xsl:template name="GetCurrentField">
		<xsl:value-of select="/emobda/Current/@Field"/>
	</xsl:template>
	<xsl:template name="GetCurrentRecord">
		<xsl:value-of select="/emobda/Current/@Record"/>
	</xsl:template>
	<xsl:template name="GetFieldWording">
		<xsl:param name="Row"/>
		<xsl:value-of select="/emobda/Structure/Fields/Field[@Row=$Row]/Name"/>
	</xsl:template>
	<xsl:template name="GetDatum">
		<xsl:param name="RecordId"/>
		<xsl:param name="FieldRow"/>
		<xsl:value-of select="/emobda/Content/Records/Record[@Id=$RecordId]/Data/Datum[@Field=$FieldRow]"/>
	</xsl:template>
</xsl:stylesheet>
