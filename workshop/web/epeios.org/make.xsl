
<!--$Id$-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="text"/>
	<xsl:template match="/">
<xsl:text>include Makefile.header

all:
	</xsl:text>
		<xsl:apply-templates select="//document/specific/WEB"/>
	</xsl:template>
 <xsl:template match="document/specific/WEB">
 	<xsl:text>$(mkdir) -p $(dest)/</xsl:text>
 	<xsl:value-of select="@hierarchy"/>
<xsl:text>
	sabcmd file://$(templates)/epeios.org/page.xsl <![CDATA[<]]>i/site.xml <![CDATA[>]]>$(dest)/</xsl:text>
	<xsl:value-of select="@hierarchy"/>
	<xsl:text>/</xsl:text>
	<xsl:value-of select="@name"/>
	<xsl:text>.html '$$hierarchy=</xsl:text>
	<xsl:value-of select="@hierarchy"/>
	<xsl:text>' '$$name=</xsl:text>
	<xsl:value-of select="@name"/>
	<xsl:text>'
	</xsl:text>
	</xsl:template>
</xsl:stylesheet>
