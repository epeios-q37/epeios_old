
<!--$Id$-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="html"/>
  <xsl:param name="hierarchy"/>
  <xsl:param name="name"/>
  <xsl:template match="/">
    <xsl:apply-templates select="//document"/>
  </xsl:template>
  <xsl:template match="document">
    <xsl:if test="specific/WEB/@hierarchy=$hierarchy">
      <xsl:if test="specific/WEB/@name=$name">
        <xsl:text disable-output-escaping="yes">&lt;!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd"></xsl:text>
        <html>
          <head>
            <xsl:apply-templates select="specific/WEB" mode="head"/>
            <xsl:apply-templates select="general" mode="head"/>
          </head>
          <body>
            <xsl:apply-templates select="." mode="body"/>
          </body>
        </html>
      </xsl:if>
    </xsl:if>
  </xsl:template>
  <xsl:template match="document/specific/WEB" mode="head" xml:space="preserve">
    <xsl:apply-templates select="redirection" mode="head"/>
    <title>Epeios - <xsl:value-of select="@bookmark"/></title>
    <xsl:call-template name="meta">
      <xsl:with-param name="name">robots</xsl:with-param>
      <xsl:with-param name="value">
        <xsl:value-of select="@robots"/>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>
  <xsl:template match="document/specific/WEB/redirection" mode="head" xml:space="preserve">
    <meta>
      <xsl:attribute name="http-equiv">refresh</xsl:attribute>
      <xsl:attribute name="content">
        <xsl:value-of select="@delay"/>;URL=<xsl:value-of select="@URL"/></xsl:attribute>
    </meta>
  </xsl:template>
  <xsl:template match="document/general" mode="head" xml:space="preserve">
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
        <xsl:value-of select="author/name"/>(<xsl:value-of select="author/email"/>)</xsl:with-param>
    </xsl:call-template>
  </xsl:template>
  <xsl:template name="meta">
    <xsl:param name="name"/>
    <xsl:param name="value"/>
    <meta>
      <xsl:attribute name="name">
        <xsl:value-of select="$name"/>
      </xsl:attribute>
      <xsl:attribute name="content">
        <xsl:value-of select="$value"/>
      </xsl:attribute>
    </meta>
  </xsl:template>
  <xsl:template match="document" mode="body">
    <xsl:text disable-output-escaping="yes">&lt;!-- </xsl:text>
    <xsl:value-of select="general/@abstract"/>
    <xsl:text disable-output-escaping="yes"> --></xsl:text>
    <table border="0" cellpadding="0" width="100%" height="100%">
      <TBODY>
        <tr>
          <td valign="top" width="100%">
            <xsl:apply-templates select="." mode="body_header"/>
            <hr/>
          </td>
        </tr>
        <tr>
          <td width="100%">
            <H1>
              <center>
                <xsl:value-of select="general/@title"/>
              </center>
            </H1>
            <xsl:apply-templates select="content"/>
            <xsl:apply-templates select="Libraries"/>
            <xsl:apply-templates select="specific/WEB/redirection"/>
          </td>
        </tr>
        <tr>
          <td valign="bottom" width="100%">
            <hr/>
            <xsl:apply-templates select="." mode="body_footer"/>
          </td>
        </tr>
      </TBODY>
    </table>
  </xsl:template>
  <xsl:template match="document" mode="body_header">
    <table border="0" width="100%">
      <TBODY>
        <tr>
          <td width="33%">
            <strong>The Epeios project</strong>
            <br/>
            <a href="http://epeios.org/" target="_blank">
              <em>http://epeios.org/</em>
            </a>
          </td>
          <td align="center" width="33%">
            <xsl:apply-templates select="specific/WEB/@parent"/>
          </td>
          <td align="right" width="33%">Hosted on<br/><a href="http://sv.gnu.org" target="_blank">Savannah</a></td>
        </tr>
      </TBODY>
    </table>
  </xsl:template>
  <xsl:template match="document" mode="body_footer">
    <table border="0" width="100%">
      <TBODY>
        <tr>
          <td>
            <font size="2">
              <em xml:space="preserve">Copyright <xsl:text disable-output-escaping="yes">&amp;#169; </xsl:text><xsl:value-of select="general/copyright/date"/><xsl:text> </xsl:text><a href="mailto:{general/copyright/owner/email}">
                  <xsl:value-of select="general/copyright/owner/name"/>
                </a>.<br/> All rights reserved.</em>
            </font>
          </td>
          <td align="center" width="33%">
            <xsl:apply-templates select="specific/WEB/@parent"/>
          </td>
          <td align="right">
            <font size="2">
              <em xml:space="preserve">Last modified :<xsl:text> </xsl:text><xsl:value-of select="general/misc/stamp/@date"/><xsl:text> </xsl:text><xsl:value-of select="general/misc/stamp/@time"/><xsl:text> </xsl:text><xsl:value-of select="general/misc/generation/time"/><br/>by <a href="mailto:{general/author/email}">                  <xsl:value-of select="general/author/name"/></a> (<a href="mailto:{general/author/email}">                  <xsl:value-of select="general/author/email"/></a>).
                </em>
            </font>
          </td>
        </tr>
      </TBODY>
    </table>
  </xsl:template>
  <xsl:template match="specific/WEB/redirection" xml:space="preserve">You will be automatically redirected to <a>
                <xsl:attribute name="href">
                  <xsl:value-of select="@URL"/>
                </xsl:attribute>
                <xsl:value-of select="@URL"/>
              </a> in a few seconds. If not, click <a>
                <xsl:attribute name="href">
                  <xsl:value-of select="@URL"/>
                </xsl:attribute>here</a>.</xsl:template>
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