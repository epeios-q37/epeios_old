
<!--$Id$-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:template match="Libraries">
   <center>
     <table>
       <xsl:apply-templates select="Library" mode="index"/>
     </table>
   </center>
   <xsl:apply-templates select="Library"/>
 </xsl:template>
 <xsl:template match="Libraries/Library" mode="index">
    <TR>
      <TD>
        <TT>
          <xsl:text disable-output-escaping="yes">&lt;A HREF="#</xsl:text>
          <xsl:value-of select="@Name"/>
          <xsl:text disable-output-escaping="yes">"></xsl:text>
          <xsl:value-of select="@Name"/>
          <xsl:text disable-output-escaping="yes">&lt;/A> </xsl:text>
        </TT>
      </TD>
      <TD>
        <TT>
          <xsl:value-of select="Version"/>
        </TT>
      </TD>
      <TD>
        <TT xml:space="preserve">(<xsl:value-of select="Release"/>) : </TT>
      </TD>
      <TD>
        <TT>
          <EM>
            <xsl:value-of select="Description"/>
          </EM>
        </TT>
      </TD>
    </TR>
  </xsl:template>
  <xsl:template match="Libraries/Library">
    <HR/>
    <CENTER>
      <H2>
        <U xml:space="preserve">Library <A>
            <xsl:attribute name="NAME">
              <xsl:value-of select="@Name"/>
            </xsl:attribute>
          </A><EM>
            <xsl:value-of select="@Name"/>
          </EM></U>
      </H2>
      <EM>
        <xsl:value-of select="Description"/>
      </EM>
    </CENTER>
    <UL>
      <LI>
        <B xml:space="preserve">Version : <EM>
            <xsl:value-of select="Version"/>
          </EM></B>
      </LI>
      <LI>
        <B xml:space="preserve">Release : <EM>
            <xsl:value-of select="Release"/>
          </EM></B>
      </LI>
      <LI>
        <B xml:space="preserve">Coordinators : <EM>
            <xsl:value-of select="Coordinators"/>
          </EM></B>
      </LI>
    </UL>
    <xsl:call-template name="lib-index">
      <xsl:with-param name="item" select="Defines"/>
    </xsl:call-template>
    <xsl:call-template name="lib-index">
      <xsl:with-param name="item" select="Shortcuts"/>
    </xsl:call-template>
    <xsl:call-template name="lib-index">
      <xsl:with-param name="item" select="Enums"/>
    </xsl:call-template>
    <xsl:call-template name="lib-index">
      <xsl:with-param name="item" select="Typedefs"/>
    </xsl:call-template>
    <xsl:call-template name="lib-index">
      <xsl:with-param name="item" select="Classes"/>
    </xsl:call-template>
    <xsl:call-template name="lib-index">
      <xsl:with-param name="item" select="Functions"/>
    </xsl:call-template>
    <xsl:call-template name="lib-index-target">
      <xsl:with-param name="item" select="Defines"/>
    </xsl:call-template>
    <xsl:apply-templates select="Defines"/>
    <xsl:call-template name="lib-index-target">
      <xsl:with-param name="item" select="Shortcuts"/>
    </xsl:call-template>
    <xsl:apply-templates select="Shortcuts"/>
    <xsl:call-template name="lib-index-target">
      <xsl:with-param name="item" select="Enums"/>
    </xsl:call-template>
    <xsl:apply-templates select="Enums"/>
    <xsl:call-template name="lib-index-target">
      <xsl:with-param name="item" select="Typedefs"/>
    </xsl:call-template>
    <xsl:apply-templates select="Typedefs"/>
    <xsl:call-template name="lib-index-target">
      <xsl:with-param name="item" select="Classes"/>
    </xsl:call-template>
    <xsl:apply-templates select="Classes"/>
    <xsl:call-template name="lib-index-target">
      <xsl:with-param name="item" select="Functions"/>
    </xsl:call-template>
    <xsl:apply-templates select="Functions"/>
  </xsl:template>
  <xsl:template name="lib-index">
    <xsl:param name="item"/>
    <xsl:text disable-output-escaping="yes">&lt;A HREF="#</xsl:text>
    <xsl:value-of select="$item/parent::*/@Name"/>
    <xsl:text disable-output-escaping="yes">.</xsl:text>
    <xsl:value-of select="name($item)"/>
    <xsl:text disable-output-escaping="yes">"></xsl:text>
    <xsl:value-of select="name($item)"/>
    <xsl:text disable-output-escaping="yes">&lt;/A> </xsl:text>
  </xsl:template>
  <xsl:template name="lib-index-target">
    <xsl:param name="item"/>
    <xsl:text disable-output-escaping="yes">&lt;A NAME="</xsl:text>
    <xsl:value-of select="$item/parent::*/@Name"/>
    <xsl:text disable-output-escaping="yes">.</xsl:text>
    <xsl:value-of select="name($item)"/>
    <xsl:text disable-output-escaping="yes">"/></xsl:text>
  </xsl:template>
  <xsl:template match="Libraries/Library/Defines" mode="index">
    <xsl:text disable-output-escaping="yes">&lt;A HREF="#</xsl:text>
    <xsl:value-of select="parent::*/@Name"/>
    <xsl:text disable-output-escaping="yes">.</xsl:text>
    <xsl:value-of select="name(.)"/>
    <xsl:text disable-output-escaping="yes">"></xsl:text>
    <xsl:value-of select="name(.)"/>
    <xsl:text disable-output-escaping="yes">&lt;/A> </xsl:text>
  </xsl:template>
  <xsl:template match="Libraries/Library/Defines">
    <H2>Defines</H2>
    <xsl:apply-templates mode="index"/>
    <UL>
      <xsl:apply-templates/>
    </UL>
  </xsl:template>
  <xsl:template match="Libraries/Library/Defines/Define" mode="index">
    <xsl:call-template name="index">
      <xsl:with-param name="discriminator" select="parent::*/parent::*/@Name"/>
    </xsl:call-template>
  </xsl:template>
  <xsl:template match="Libraries/Library/Defines/Define">
    <LI xml:space="preserve">
      <xsl:call-template name="index-target">
        <xsl:with-param name="discriminator" select="parent::*/parent::*/@Name"/>
      </xsl:call-template>
      <TT>
        <xsl:value-of select="@Name"/>
      </TT>
      <xsl:apply-templates select="Arguments"/> : <EM>
        <xsl:apply-templates select="Comment"/>
      </EM></LI>
  </xsl:template>
  <xsl:template match="Libraries/Library/Defines/Define/Arguments">(<xsl:apply-templates/>)</xsl:template>
  <xsl:template match="Libraries/Library/Defines/Define/Arguments/Argument">
    <B>
      <xsl:value-of select="@Name"/>
    </B>
  </xsl:template>
  <xsl:template match="Libraries/Library/Shortcuts">
    <H2>Shortcuts</H2>
    <xsl:apply-templates mode="index"/>
    <UL>
      <xsl:apply-templates/>
    </UL>
  </xsl:template>
  <xsl:template match="Libraries/Library/Shortcuts/Shortcut" mode="index">
    <xsl:call-template name="index">
      <xsl:with-param name="discriminator" select="parent::*/parent::*/@Name"/>
    </xsl:call-template>
  </xsl:template>
  <xsl:template match="Libraries/Library/Shortcuts/Shortcut">
    <LI>
      <xsl:call-template name="index-target">
        <xsl:with-param name="discriminator" select="parent::*/parent::*/@Name"/>
      </xsl:call-template>
      <TT xml:space="preserve">
        <xsl:value-of select="@Name"/>
        <xsl:apply-templates select="Arguments"/> : <xsl:apply-templates select="Alias"/></TT>
      <br/>
      <EM>
        <xsl:apply-templates select="Comment"/>
      </EM>
    </LI>
  </xsl:template>
  <xsl:template match="Libraries/Library/Shortcuts/Shortcut/Arguments">(<B>
      <xsl:apply-templates/>
    </B>)</xsl:template>
  <xsl:template match="Libraries/Library/Shortcuts/Shortcut/Arguments/Argument">
    <xsl:value-of select="@Name"/>,</xsl:template>
  <xsl:template match="Libraries/Library/Enums">
    <H2>Enums</H2>
    <xsl:apply-templates mode="index"/>
    <UL>
      <xsl:apply-templates select="Enum"/>
    </UL>
  </xsl:template>
  <xsl:template match="Enum" mode="index">
    <xsl:call-template name="index">
      <xsl:with-param name="discriminator" select="parent::*/parent::*/@Name"/>
    </xsl:call-template>
  </xsl:template>
  <xsl:template match="Enum">
    <LI xml:space="preserve">
      <xsl:call-template name="index-target">
        <xsl:with-param name="discriminator" select="parent::*/parent::*/@Name"/>
      </xsl:call-template>
      <TT>
        <xsl:value-of select="@Name"/>
      </TT> : <EM>
        <xsl:value-of select="Comment"/>
      </EM><xsl:apply-templates select="Items"/></LI>
    <xsl:apply-templates select="Enums"/>
  </xsl:template>
  <xsl:template match="Libraries/Library/Enums/Enum/Items">
    <UL>
      <xsl:apply-templates select="Item"/>
    </UL>
  </xsl:template>
  <xsl:template match="Libraries/Library/Enums/Enum/Items/Item">
    <LI xml:space="preserve">
      <B>
        <TT>
          <xsl:value-of select="@Name"/>
        </TT>
      </B> : <xsl:apply-templates select="Comment"/></LI>
  </xsl:template>
  <xsl:template match="Libraries/Library/Typedefs">
    <H2>Typedefs</H2>
    <xsl:apply-templates select="Typedef" mode="index"/>
    <UL>
      <xsl:apply-templates select="Typedef"/>
    </UL>
  </xsl:template>
  <xsl:template match="Libraries/Library/Typedefs/Typedef" mode="index">
    <xsl:call-template name="index">
      <xsl:with-param name="discriminator" select="parent::*/parent::*/@Name"/>
    </xsl:call-template>
  </xsl:template>
  <xsl:template match="Libraries/Library/Typedefs/Typedef">
    <LI xml:space="preserve">
      <xsl:call-template name="index-target">
        <xsl:with-param name="discriminator" select="parent::*/parent::*/@Name"/>
      </xsl:call-template>
      <TT>
        <xsl:value-of select="@Name"/>
      </TT> : <EM>
        <xsl:apply-templates select="Comment"/>
      </EM></LI>
  </xsl:template>
  <xsl:template match="Libraries/Library/Classes">
    <H2>Classes</H2>
    <xsl:apply-templates select="Classe" mode="index"/>
    <xsl:apply-templates select="Classe"/>
  </xsl:template>
  <xsl:template match="Libraries/Library/Classes/Classe" mode="index">
    <xsl:call-template name="index">
      <xsl:with-param name="discriminator" select="parent::*/parent::*/@Name"/>
    </xsl:call-template>
  </xsl:template>
  <xsl:template match="Libraries/Library/Classes/Classe">
    <xsl:call-template name="index-target">
      <xsl:with-param name="discriminator" select="parent::*/parent::*/@Name"/>
    </xsl:call-template>
    <H3>
      <xsl:value-of select="@Name"/>
      <xsl:apply-templates select="Templates"/>
    </H3>
    <xsl:apply-templates select="Base_classes"/>
    <EM>
      <xsl:apply-templates select="Comment"/>
    </EM>
    <xsl:apply-templates select="Objects"/>
    <xsl:apply-templates select="Functions"/>
  </xsl:template>
  <xsl:template match="Libraries/Library/Classes/Classe/Functions[@Type='Methods']">
    <H4>Methods</H4>
    <xsl:apply-templates select="Function" mode="index"/>
    <UL>
      <xsl:apply-templates select="Function"/>
    </UL>
  </xsl:template>
  <xsl:template match="Libraries/Library/Classes/Classe/Functions[@Type='Handlers']">
    <H4>Handlers</H4>
    <xsl:apply-templates select="Function" mode="index"/>
    <UL>
      <xsl:apply-templates select="Function"/>
    </UL>
  </xsl:template>
  <xsl:template match="Libraries/Library/Functions">
    <H2>Functions</H2>
    <xsl:apply-templates select="Function" mode="index"/>
    <UL>
      <xsl:apply-templates select="Function"/>
    </UL>
  </xsl:template>
  <xsl:template match="Libraries/Library/Classes/Classe/Templates" xml:space="preserve">
    <TT>&lt; <xsl:apply-templates select="Template"/> ></TT>
  </xsl:template>
  <xsl:template match="Libraries/Library/Classes/Classe/Base_classes">
    <UL>
      <xsl:apply-templates select="Base_class"/>
    </UL>
  </xsl:template>
  <xsl:template match="Libraries/Library/Classes/Classe/Objects">
    <H4>Objects</H4>
    <UL>
      <xsl:apply-templates select="Object"/>
    </UL>
  </xsl:template>
  <xsl:template match="Libraries/Library/Classes/Classe/Templates/Template">
    <EM>
      <xsl:value-of select="Type"/>
    </EM>
    <xsl:text> </xsl:text>
    <xsl:value-of select="@Name"/>
    <xsl:if test="not(position()=last())">, </xsl:if>
  </xsl:template>
  <xsl:template match="Libraries/Library/Classes/Classe/Base_classes/Base_class">
    <LI>
      <TT>
        <xsl:value-of select="@Name"/>
      </TT>
    </LI>
  </xsl:template>
  <xsl:template match="Libraries/Library/Classes/Classe/Objects/Object">
    <LI>
      <TT>
        <xsl:value-of select="Type"/>
        <xsl:text disable-output-escaping="yes"> </xsl:text>
        <B xml:space="preserve">
          <xsl:value-of select="@Name"/> : </B>
      </TT>
      <EM>
        <xsl:apply-templates select="Comment"/>
      </EM>
    </LI>
  </xsl:template>
  <xsl:template match="Function" mode="index">
    <xsl:call-template name="index">
      <xsl:with-param name="discriminator" select="parent::*/parent::*/@Name"/>
    </xsl:call-template>
  </xsl:template>
  <xsl:template match="Function">
    <LI>
      <xsl:call-template name="index-target">
        <xsl:with-param name="discriminator" select="parent::*/parent::*/@Name"/>
      </xsl:call-template>
      <TT>
        <B xml:space="preserve">
          <xsl:value-of select="@Name"/> : </B>
        <xsl:value-of select="Type"/>
        <xsl:apply-templates select="Parameters"/>
        <EM>
          <xsl:apply-templates select="Comment"/>
        </EM>
      </TT>
    </LI>
  </xsl:template>
  <xsl:template match="Function/Parameters">
    <UL>
      <xsl:apply-templates/>
    </UL>
  </xsl:template>
  <xsl:template match="Function/Parameters/Parameter">
    <LI>
      <TT>
        <EM>
          <xsl:value-of select="Type"/>
          <B>
            <xsl:text disable-output-escaping="yes"> </xsl:text>
            <xsl:value-of select="@Name"/>
          </B>
        </EM>
        <xsl:apply-templates select="Value"/>
      </TT>
    </LI>
  </xsl:template>
  <xsl:template match="Function/Parameters/Parameter/Value" xml:space="preserve"> = <xsl:value-of select="."/></xsl:template>
  <xsl:template name="index">
    <xsl:param name="discriminator"/>
    <xsl:if test="not(preceding-sibling::*/@Name=@Name)">
      <xsl:text disable-output-escaping="yes">&lt;A HREF="#</xsl:text>
      <xsl:value-of select="$discriminator"/>
      <xsl:text disable-output-escaping="yes">.</xsl:text>
      <xsl:value-of select="@Name"/>
      <xsl:text disable-output-escaping="yes">"></xsl:text>
      <xsl:value-of select="@Name"/>
      <xsl:text disable-output-escaping="yes">&lt;/A> </xsl:text>
    </xsl:if>
  </xsl:template>
  <xsl:template name="index-target">
    <xsl:param name="discriminator"/>
    <xsl:if test="not(preceding-sibling::*/@Name=@Name)">
      <xsl:text disable-output-escaping="yes">&lt;A NAME="</xsl:text>
      <xsl:value-of select="$discriminator"/>
      <xsl:text disable-output-escaping="yes">.</xsl:text>
      <xsl:value-of select="@Name"/>
      <xsl:text disable-output-escaping="yes">"/></xsl:text>
    </xsl:if>
  </xsl:template>
</xsl:stylesheet>
