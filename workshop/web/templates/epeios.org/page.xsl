<!--$Id$-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="xhtml" encoding="iso-8859-1" indent="yes" doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd" doctype-public="-//W3C//DTD XHTML 1.0 Strict//EN"/>
	<xsl:param name="hierarchy"/>
	<xsl:param name="name"/>
	<xsl:template match="/">
		<xsl:apply-templates select="//document"/>
	</xsl:template>
	<xsl:template match="document">
		<xsl:if test="specific/WEB/@hierarchy=$hierarchy">
			<xsl:if test="specific/WEB/@name=$name">
				<xsl:element name="html">
					<xsl:attribute name="xmlns">http://www.w3.org/1999/xhtml</xsl:attribute>
					<xsl:element name="head">
						<xsl:element name="meta">
							<xsl:attribute name="http-equiv">Content-type</xsl:attribute> 
							<xsl:attribute name="content">text/html; charset=iso-8859-1</xsl:attribute>
						</xsl:element>
						<xsl:apply-templates select="specific/WEB" mode="head"/>
						<xsl:apply-templates select="general" mode="head"/>
					</xsl:element>
					<xsl:element name="body">
						<xsl:apply-templates select="." mode="body"/>
					</xsl:element>
				</xsl:element>
			</xsl:if>
		</xsl:if>
	</xsl:template>
	<xsl:template match="document/specific/WEB" mode="head">
		<xsl:apply-templates select="redirection" mode="head"/>
		<xsl:element name="title">
			<xsl:text>Epeios - </xsl:text>
			<xsl:value-of select="@bookmark"/>
		</xsl:element>
		<xsl:call-template name="meta">
			<xsl:with-param name="name">robots</xsl:with-param>
			<xsl:with-param name="value">
				<xsl:value-of select="@robots"/>
			</xsl:with-param>
		</xsl:call-template>
	</xsl:template>
	<xsl:template match="document/specific/WEB/redirection" mode="head">
		<xsl:element name="meta">
			<xsl:attribute name="http-equiv">refresh</xsl:attribute>
			<xsl:attribute name="content">
				<xsl:value-of select="@delay"/>
				<xsl:text>;URL=</xsl:text>
				<xsl:value-of select="@URL"/></xsl:attribute>
		</xsl:element>
	</xsl:template>
	<xsl:template match="document/general" mode="head">
		<xsl:call-template name="meta">
			<xsl:with-param name="name">description</xsl:with-param>
			<xsl:with-param name="value">
				<xsl:value-of select="@abstract"/>
			</xsl:with-param>
		</xsl:call-template>
		<xsl:call-template name="meta">
			<xsl:with-param name="name">keywords</xsl:with-param>
			<xsl:with-param name="value">
				<xsl:value-of select="@keywords"/>
			</xsl:with-param>
		</xsl:call-template>
		<xsl:call-template name="meta">
			<xsl:with-param name="name">author</xsl:with-param>
			<xsl:with-param name="value">
				<xsl:value-of select="author/name"/>
				<xsl:text>(</xsl:text>
				<xsl:value-of select="author/email"/>
				<xsl:text>)</xsl:text>
			</xsl:with-param>
		</xsl:call-template>
	</xsl:template>
	<xsl:template name="meta">
		<xsl:param name="name"/>
		<xsl:param name="value"/>
		<xsl:element name="meta">
			<xsl:attribute name="name">
				<xsl:value-of select="$name"/>
			</xsl:attribute>
			<xsl:attribute name="content">
				<xsl:value-of select="$value"/>
			</xsl:attribute>
		</xsl:element>
	</xsl:template>
	<xsl:template match="document" mode="body">
		<xsl:comment>
			<xsl:value-of select="general/@abstract"/>
		</xsl:comment>
		<xsl:element name="table">
			<xsl:attribute name="border">0</xsl:attribute>
			<xsl:attribute name="cellpadding">0</xsl:attribute>
			<xsl:attribute name="width">100%</xsl:attribute>
			<xsl:element name="tbody">
				<xsl:element name="tr">
					<xsl:element name="td">
						<xsl:attribute name="valign">top</xsl:attribute>
						<xsl:apply-templates select="." mode="body_header"/>
						<xsl:element name="hr"/>
					</xsl:element>
				</xsl:element>
				<xsl:element name="tr">
					<xsl:element name="td">
						<xsl:element name="div">
							<xsl:attribute name="align">center</xsl:attribute>
							<xsl:element name="h1">
								<xsl:value-of select="general/@title"/>
							</xsl:element>
						</xsl:element>
						<xsl:apply-templates select="content"/>
						<xsl:apply-templates select="Libraries"/>
						<xsl:apply-templates select="specific/WEB/redirection"/>
					</xsl:element>
				</xsl:element>
				<xsl:element name="tr">
					<xsl:element name="td">
						<xsl:attribute name="valign">bottom</xsl:attribute>
						<xsl:element name="hr"/>
						<xsl:apply-templates select="." mode="body_footer"/>
					</xsl:element>
				</xsl:element>
			</xsl:element>
		</xsl:element>
	</xsl:template>
	<xsl:template match="document" mode="body_header">
		<xsl:element name="table">
			<xsl:attribute name="border">0</xsl:attribute>
			<xsl:attribute name="width">100%</xsl:attribute>
			<xsl:element name="tbody">
				<xsl:element name="tr">
					<xsl:element name="td">
						<xsl:element name="strong">
							<xsl:text>The Epeios project</xsl:text>
						</xsl:element>
						<xsl:element name="br"/>
						<xsl:element name="a">
							<xsl:attribute name="href">http://epeios.org/</xsl:attribute>
							<xsl:element name="em">
								<xsl:text>http://epeios.org/</xsl:text>
							</xsl:element>
						</xsl:element>
					</xsl:element>
					<xsl:element name="td">
						<xsl:attribute name="align">center</xsl:attribute>
						<xsl:apply-templates select="specific/WEB/@parent"/>
					</xsl:element>
					<xsl:element name="td">
						<xsl:attribute name="align">right</xsl:attribute>
						<xsl:text>Hosted on</xsl:text>
						<xsl:element name="br"/>
						<xsl:element name="a">
							<xsl:attribute name="href">http://sv.gnu.org</xsl:attribute>
							<xsl:text>Savannah</xsl:text>
						</xsl:element>
					</xsl:element>
				</xsl:element>
			</xsl:element>
		</xsl:element>
	</xsl:template>
	<xsl:template match="document" mode="body_footer">
		<xsl:element name="table">
			<xsl:attribute name="border">0</xsl:attribute>
			<xsl:attribute name="width">100%</xsl:attribute>
			<xsl:element name="tbody">
				<xsl:element name="tr">
					<xsl:element name="td">
						<xsl:element name="font">
							<xsl:attribute name="size">2</xsl:attribute>
							<xsl:element name="em">
								<xsl:text>Copyright </xsl:text>
								<xsl:text>&#169; </xsl:text>
								<xsl:value-of select="general/copyright/date"/>
								<xsl:text> </xsl:text>
								<xsl:element name="a">
									<xsl:attribute name="href">
										<xsl:text>mailto:</xsl:text>
										<xsl:value-of select="general/copyright/owner/email"/>
									</xsl:attribute>
									<xsl:value-of select="general/copyright/owner/name"/>
								</xsl:element>
								<xsl:text>.</xsl:text>
								<xsl:element name="br"/>
								<xsl:text> All rights reserved.</xsl:text>
							</xsl:element>
						</xsl:element>
					</xsl:element>
					<xsl:element name="td">
						<xsl:attribute name="align">center</xsl:attribute>
						<xsl:apply-templates select="specific/WEB/@parent"/>
					</xsl:element>
					<xsl:element name="td">
						<xsl:attribute name="align">right</xsl:attribute>
						<xsl:element name="font">
							<xsl:attribute name="size">2</xsl:attribute>
							<xsl:element name="em">
								<xsl:text>Last modified : </xsl:text>
								<xsl:value-of select="general/misc/stamp/@date"/>
								<xsl:text> </xsl:text>
								<xsl:value-of select="general/misc/stamp/@time"/>
								<xsl:text> </xsl:text>
								<xsl:value-of select="general/misc/generation/time"/>
								<xsl:element name="br"/>
								<xsl:text>by </xsl:text>
								<xsl:element name="a">
									<xsl:attribute name="href">
										<xsl:text>mailto:</xsl:text>
										<xsl:value-of select="general/author/email"/>
									</xsl:attribute>
									<xsl:value-of select="general/author/name"/>
								</xsl:element>
								<xsl:text> (</xsl:text>
								<xsl:element name="a">
									<xsl:attribute name="href">
										<xsl:text>mailto:</xsl:text>
										<xsl:value-of select="general/author/email"/>
									</xsl:attribute>
									<xsl:value-of select="general/author/email"/>
								</xsl:element>
								<xsl:text>).</xsl:text>
							</xsl:element>
						</xsl:element>
					</xsl:element>
				</xsl:element>
			</xsl:element>
		</xsl:element>
	</xsl:template>
	<xsl:template match="specific/WEB/redirection">
		<xsl:text>You will be automatically redirected to </xsl:text>
		<xsl:element name="a">
			<xsl:attribute name="href">
				<xsl:value-of select="@URL"/>
			</xsl:attribute>
			<xsl:value-of select="@URL"/>
		</xsl:element>
		<xsl:text>in a few seconds. If not, click </xsl:text>
		<xsl:element name="a">
			<xsl:attribute name="href">
				<xsl:value-of select="@URL"/>
			</xsl:attribute>
			<xsl:text>here</xsl:text>
		</xsl:element>
		<xsl:text>.</xsl:text>
	</xsl:template>
	<xsl:template match="warning" mode="warning">
		<xsl:apply-templates/>
	</xsl:template>
	<xsl:template match="warning"/>
	<xsl:include href="../chapters.xsl"/>
	<xsl:include href="../content.xsl"/>
	<xsl:include href="../list.xsl"/>
	<xsl:include href="../misc.xsl"/>
	<xsl:include href="../libraries.xsl"/>
	<xsl:include href="../cvs.xsl"/>
</xsl:stylesheet>