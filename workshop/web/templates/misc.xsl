<!--$Id$-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="section">
		<xsl:apply-templates/>
		<xsl:element name="p"/>
	</xsl:template>
	<xsl:template match="para">
		<xsl:choose>
			<xsl:when test="@type='remark'">
				<xsl:element name="em">
					<xsl:apply-templates/>
				</xsl:element>
			</xsl:when>
			<xsl:otherwise>
				<xsl:apply-templates/>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:element name="br"/>
	</xsl:template>
	<xsl:template match="link">
		<xsl:element name="a">
			<xsl:attribute name="href">
				<xsl:value-of select="@URL"/>
			</xsl:attribute>
			<xsl:if test="@type='external'">
				<xsl:attribute name="target">_blank</xsl:attribute>
			</xsl:if>
			<xsl:apply-templates/>
		</xsl:element>
		<xsl:if test="@aspect='long'">
			<xsl:text>(</xsl:text>
			<xsl:element name="a">
				<xsl:attribute name="href">
					<xsl:value-of select="@URL"/>
				</xsl:attribute>
				<xsl:if test="@type='external'">
					<xsl:attribute name="target">_blank</xsl:attribute>
				</xsl:if>
				<xsl:value-of select="@URL"/>
			</xsl:element>
			<xsl:text>)</xsl:text>
		</xsl:if>
	</xsl:template>
	<xsl:template match="raw">
		<xsl:element name="tt">
			<xsl:element name="strong">
				<xsl:apply-templates/>
			</xsl:element>
		</xsl:element>
	</xsl:template>
	<xsl:template match="source">
		<xsl:element name="pre">
			<xsl:value-of select="."/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="specific/WEB/@parent">
		<xsl:element name="a">
			<xsl:attribute name="href">
				<xsl:value-of select="."/>
			</xsl:attribute>
			<xsl:text>Parent</xsl:text>
		</xsl:element>
	</xsl:template>
	<xsl:template match="contact">
		<xsl:element name="a">
			<xsl:attribute name="href">
				<xsl:text>mailto:</xsl:text>
				<xsl:value-of select="@adress"/>
			</xsl:attribute>
			<xsl:value-of select="."/>
		</xsl:element>
		<xsl:if test="@type='long'">
			<xsl:text> (</xsl:text>
			<xsl:element name="a">
				<xsl:attribute name="href">
					<xsl:text>mailto:</xsl:text>
					<xsl:value-of select="@adress"/>
				</xsl:attribute>
				<xsl:value-of select="@adress"/>
			</xsl:element>
			<xsl:text>)</xsl:text>
		</xsl:if>
	</xsl:template>
	<xsl:template match="ignore"/>
</xsl:stylesheet>