<!--$Id$-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="CVS_">
		<xsl:text>The CVS repository can be checked out through anonymous (pserver) CVS with the following instruction set. When prompted for a password for</xsl:text>
		<xsl:element name="em">anonymous</xsl:element>
		<xsl:text>, simply press the Enter key.</xsl:text>
		<xsl:element name="p"/>
		<xsl:element name="tt">
			<xsl:text>cvs -d:pserver:anonymous@</xsl:text>
			<xsl:value-of select="@server"/>
			<xsl:text>:</xsl:text>
			<xsl:value-of select="@root"/>
			<xsl:text>login</xsl:text>
		</xsl:element>
		<xsl:element name="br"/>
		<xsl:element name="tt">
			<xsl:text>cvs -z3 -d:pserver:anonymous@</xsl:text>
			<xsl:value-of select="@server"/>
			<xsl:text>:</xsl:text>
			<xsl:value-of select="@root"/>
			<xsl:text>co</xsl:text>
			<xsl:value-of select="@module"/>
		</xsl:element>
		<xsl:element name="p"/>
		<xsl:apply-templates select="link"/>
	</xsl:template>
	<xsl:template match="CVS/link">
		<xsl:text>You can browse the CVS repository by following this</xsl:text>
		<xsl:element name="a">
			<xsl:attribute name="href">
				<xsl:value-of select="@URL"/>
			</xsl:attribute>
			<xsl:attribute name="target">_blank</xsl:attribute>
			<xsl:text>link</xsl:text>
		</xsl:element>
		<xsl:text>(</xsl:text>
		<xsl:element name="a">
			<xsl:attribute name="href">
				<xsl:value-of select="@URL"/>
			</xsl:attribute>
			<xsl:attribute name="target">_blank</xsl:attribute>
			<xsl:value-of select="@URL"/>
		</xsl:element>
		<xsl:text>).</xsl:text>
	</xsl:template>
</xsl:stylesheet>