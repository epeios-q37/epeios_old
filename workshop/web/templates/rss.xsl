<!--$Id$-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="rss">
		<xsl:element name="table">
			<xsl:attribute name="align">center</xsl:attribute>
			<xsl:attribute name="width">95%</xsl:attribute>
			<xsl:attribute name="border">1</xsl:attribute>
			<xsl:element name="tr">
				<xsl:element name="th">
					<xsl:attribute name="align">center</xsl:attribute>
					<xsl:text>Latest news (</xsl:text>
					<xsl:element name="a">
						<xsl:attribute name="href">
							<xsl:text>http://www.livejournal.com/users/epeios/</xsl:text>
						</xsl:attribute>
						<xsl:text>Click here to view all entries</xsl:text>
					</xsl:element>
					<xsl:text>)</xsl:text>
				</xsl:element>
			</xsl:element>
			<xsl:apply-templates select="channel"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="channel">
		<xsl:apply-templates select="item"/>
	</xsl:template>
	<xsl:template match="channel/item">
		<xsl:if test="position()&lt;10">
			<xsl:element name="tr">
				<xsl:element name="td">
					<xsl:element name="i">
						<xsl:value-of select="pubDate"/>
					</xsl:element>
					<xsl:text> </xsl:text>
					<xsl:element name="a">
						<xsl:attribute name="href">
							<xsl:value-of select="link"/>
						</xsl:attribute>
						<xsl:element name="b">
							<xsl:value-of select="title"/>
						</xsl:element>
					</xsl:element>
					<xsl:text> (</xsl:text>
					<xsl:element name="a">
						<xsl:attribute name="href">
							<xsl:value-of select="link"/>
						</xsl:attribute>
						<xsl:choose>
							<xsl:when test="position()&lt;2">
								<xsl:text>view/add comments</xsl:text>
							</xsl:when>
							<xsl:otherwise>
								<xsl:text>view entry and comments</xsl:text>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:element>
					<xsl:text>)</xsl:text>
					<xsl:if test="position()&lt;2">
						<xsl:element name="br"/>
						<xsl:element name="div">
							<xsl:attribute name="style">text-indent: 5%; text-align: justify; margin: 0px;</xsl:attribute>
							<xsl:value-of select="description"/>
						</xsl:element>
					</xsl:if>
				</xsl:element>
			</xsl:element>
		</xsl:if>
	</xsl:template>
</xsl:stylesheet>
