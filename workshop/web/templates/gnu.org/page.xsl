
<!--$Id$-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="html"/>
  <xsl:template match="/">
    <xsl:text disable-output-escaping="yes">&lt;DOCTYPE html PUBLIC "-//IETF//DTD HTML 2.0//EN"></xsl:text>
    <HTML>
      <HEAD>
        <xsl:apply-templates select="document/specific/WEB" mode="head"/>
      </HEAD>
      <BODY BGCOLOR="#FFFFFF" TEXT="#000000" LINK="#1F00FF" ALINK="#FF0000" VLINK="#9900DD">
        <xsl:apply-templates select="document" mode="body"/>
      </BODY>
    </HTML>
  </xsl:template>
  <xsl:template match="document/specific/WEB" mode="head" xml:space="preserve">
    <TITLE>
      <xsl:value-of select="bookmark"/> - GNU Project - Free Software Foundation (FSF)</TITLE>
    <LINK REV="made" HREF="mailto:webmasters@www.gnu.org"/>
  </xsl:template>
  <xsl:template match="document" mode="body">
    <H3>
      <xsl:value-of select="general/title"/>
    </H3>
    <A HREF="/graphics/agnuhead.html">
      <IMG SRC="/graphics/gnu-head-sm.jpg" ALT=" [image of the Head of a GNU] " WIDTH="129" HEIGHT="122"/>
    </A>
    <xsl:apply-templates select="content"/>
    <xsl:apply-templates select="Libraries"/>
    <xsl:apply-templates select="specific/WEB/redirection"/>
    <HR/>
    <P/>Return to<A HREF="/home.html">GNU's home page</A>.<P/>Please send FSF &amp; GNU inquiries &amp; questions to<A HREF="mailto:gnu@gnu.org">
      <EM>gnu@gnu.org</EM>
    </A>. There are also<A HREF="/home.html#ContactInfo">other ways to contact</A>the FSF.<P/>Please send comments on these web pages to<A HREF="mailto:webmasters@gnu.org">
      <EM>webmasters@gnu.org</EM>
    </A>, send other questions to<A HREF="mailto:gnu@gnu.org">
      <EM>gnu@gnu.org</EM>
    </A>.<P/>Copyright (C) 2001 Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111, USA<P/>Verbatim copying and distribution of this entire article is permitted in any medium, provided this notice is preserved.<P/>Updated:<!-- timestamp start -->$Date$ $Author$<!-- timestamp end --><HR/></xsl:template>
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