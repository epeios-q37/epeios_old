<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'> 
<xsl:output method="html"/>
 <xsl:template match="/">
  <html>
   <body>
     <center><table><xsl:apply-templates mode="index"/></table></center>
     <xsl:apply-templates/>
   </body>
  </html>
 </xsl:template> 
 
 <xsl:template match="/Documentation/Library" mode="index">
  <TR>
   <TD><TT>
    <xsl:text disable-output-escaping="yes">&lt;A HREF="#</xsl:text>
    <xsl:value-of select="Name"/>
    <xsl:text disable-output-escaping="yes">"&gt;</xsl:text>
    <xsl:value-of select="Name"/>
    <xsl:text disable-output-escaping="yes">&lt;/A&gt; </xsl:text>
   </TT></TD>
   <TD><TT>
	<xsl:value-of select="Version"/>
   </TT></TD>
   <TD><TT>
	(<xsl:value-of select="Release"/>) :
   </TT></TD>
   <TD><TT>
	<EM><xsl:value-of select="Description"/></EM>
   </TT></TD>
  </TR>
 </xsl:template>
 
 <xsl:template match="Ref">
  <B><TT><xsl:value-of select="."/></TT></B>
 </xsl:template>
  
 <xsl:template match="/Documentation/Library">
  <HR/>
  <CENTER>
   <H1><U>
    <xsl:text disable-output-escaping="yes">&lt;A NAME="</xsl:text>
	<xsl:value-of select="Name"/>
	<xsl:text disable-output-escaping="yes">"/&gt;</xsl:text>
    Library <EM><xsl:value-of select="Name"/></EM>
   </U></H1>
  </CENTER>
  <UL>
  	<LI><B>Version : <EM><xsl:value-of select="Version"/></EM></B></LI>
	<LI><B>Release : <EM><xsl:value-of select="Release"/></EM></B></LI>
	<LI><B>Coordinators : <EM><xsl:value-of select="Coordinators"/></EM></B></LI>
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
  <xsl:apply-templates select="Shortcuts"/>
  <xsl:call-template name="lib-index-target">
   <xsl:with-param name="item" select="Enums"/>
  </xsl:call-template>
  <xsl:apply-templates select="Enums"/>
  <xsl:apply-templates select="Typedefs"/>
  <xsl:apply-templates select="Classes"/>
  <xsl:apply-templates select="Functions"/>
 </xsl:template>
 
 <xsl:template name="lib-index">
  <xsl:param name="item"/>
  <xsl:text disable-output-escaping="yes">&lt;A HREF="#</xsl:text>
  <xsl:value-of select="$item/parent::*/Name"/>
  <xsl:text disable-output-escaping="yes">.</xsl:text>
  <xsl:value-of select="name($item)"/>
  <xsl:text disable-output-escaping="yes">"&gt;</xsl:text>
  <xsl:value-of select="name($item)"/>
  <xsl:text disable-output-escaping="yes">&lt;/A&gt; </xsl:text>
 </xsl:template>
 
 <xsl:template name="lib-index-target">
  <xsl:param name="item"/>
    <xsl:text disable-output-escaping="yes">&lt;A NAME="</xsl:text>
  <xsl:value-of select="$item/parent::*/Name"/>
  <xsl:text disable-output-escaping="yes">.</xsl:text>
  <xsl:value-of select="name($item)"/>
	<xsl:text disable-output-escaping="yes">"/&gt;</xsl:text>
 </xsl:template>
 
 <xsl:template match="Documentation/Library/Defines" mode="index">
  <xsl:text disable-output-escaping="yes">&lt;A HREF="#</xsl:text>
  <xsl:value-of select="parent::*/Name"/>
  <xsl:text disable-output-escaping="yes">.</xsl:text>
  <xsl:value-of select="name(.)"/>
  <xsl:text disable-output-escaping="yes">"&gt;</xsl:text>
  <xsl:value-of select="name(.)"/>
  <xsl:text disable-output-escaping="yes">&lt;/A&gt; </xsl:text>
 </xsl:template>
 
 <xsl:template match="Documentation/Library/Defines">
  <H1>
   Defines
  </H1>
   <xsl:apply-templates mode="index"/>
  <UL>
   <xsl:apply-templates/>
  </UL>
 </xsl:template>
 
 <xsl:template match="Documentation/Library/Defines/Define" mode="index" >
  <xsl:call-template name="index">
   <xsl:with-param name="discriminator" select="parent::*/parent::*/Name"/>      </xsl:call-template>
 </xsl:template>
 
 <xsl:template match="Documentation/Library/Defines/Define">
  <LI>
   <xsl:call-template name="index-target">
    <xsl:with-param name="discriminator" select="parent::*/parent::*/Name"/>
   </xsl:call-template>
   <TT><xsl:value-of select="Name"/></TT> 
   <xsl:apply-templates select="Arguments"/> : 
   <EM><xsl:apply-templates select="Comment"/></EM> 
  </LI>
 </xsl:template>
 
 <xsl:template match="Documentation/Library/Defines/Define/Arguments">
 ( <xsl:apply-templates/> )
 </xsl:template>
 
 <xsl:template match="Documentation/Library/Defines/Define/Arguments/Argument">
 <B><xsl:value-of select="Name"/></B>
 </xsl:template>
 
 <xsl:template match="Documentation/Library/Shortcuts">
  <H1>
   Shortcuts
  </H1>
   <xsl:apply-templates mode="index"/>
  <UL>
   <xsl:apply-templates/>
  </UL>
 </xsl:template>
 
 <xsl:template match="Documentation/Library/Shortcuts/Shortcut" mode="index" >
  <xsl:call-template name="index">
   <xsl:with-param name="discriminator" select="parent::*/parent::*/Name"/>        </xsl:call-template>
 </xsl:template>
 
 <xsl:template match="Documentation/Library/Shortcuts/Shortcut">
  <LI>
   <xsl:call-template name="index-target">
    <xsl:with-param name="discriminator" select="parent::*/parent::*/Name"/>
   </xsl:call-template>
   <TT><xsl:value-of select="Name"/> 
   <xsl:apply-templates select="Arguments"/> : 
   <xsl:apply-templates select="Alias"/>
   </TT><br/>
   <EM><xsl:apply-templates select="Comment"/></EM> 
  </LI>
 </xsl:template>
 
 <xsl:template match="Documentation/Library/Shortcuts/Shortcut/Arguments">
 ( <B><xsl:apply-templates/></B> )
 </xsl:template>
 
 <xsl:template match="Documentation/Library/Shortcuts/Shortcut/Arguments/Argument">
 <xsl:value-of select="Name"/>, 
 </xsl:template>
 
 <xsl:template match="/Documentation/Library/Enums">
  <H1>
   Enums
  </H1>
   <xsl:apply-templates mode="index"/>
  <UL>
   <xsl:apply-templates select="Enum"/>
  </UL>
 </xsl:template>
 
 <xsl:template match="Enum" mode="index" >
  <xsl:call-template name="index">
   <xsl:with-param name="discriminator" select="parent::*/parent::*/Name"/>        </xsl:call-template>
 </xsl:template>
 
 <xsl:template match="Enum">
  <LI>
   <xsl:call-template name="index-target">
    <xsl:with-param name="discriminator" select="parent::*/parent::*/Name"/>
   </xsl:call-template>
   <TT><xsl:value-of select="Name"/></TT> : 
   <EM>
    <xsl:value-of select="Comment"/> 
   </EM>
    <xsl:apply-templates select="Items"/>
  </LI>
  <xsl:apply-templates select="Enums"/>
 </xsl:template>
 
 <xsl:template match="/Documentation/Library/Enums/Enum/Items">
  <UL>
   <xsl:apply-templates select="Item"/>
  </UL>
 </xsl:template>

 <xsl:template match="/Documentation/Library/Enums/Enum/Items/Item">
  <LI>
   <B><TT>
    <xsl:value-of select="Name"/>
   </TT></B> : 
   <xsl:apply-templates select="Comment"/>
  </LI>
 </xsl:template>

 <xsl:template match="/Documentation/Library/Typedefs">
  <H1>
   Typedefs
  </H1>
   <xsl:apply-templates select="Typedef" mode="index"/>
  <UL>
   <xsl:apply-templates select="Typedef"/>
  </UL>
 </xsl:template>
 
 <xsl:template match="/Documentation/Library/Typedefs/Typedef" mode="index">
  <xsl:call-template name="index">
   <xsl:with-param name="discriminator" select="parent::*/parent::*/Name"/>
  </xsl:call-template>
 </xsl:template>
 
 <xsl:template match="/Documentation/Library/Typedefs/Typedef">
  <LI>
  <xsl:call-template name="index-target">
   <xsl:with-param name="discriminator" select="parent::*/parent::*/Name"/>
  </xsl:call-template>
   <TT><xsl:value-of select="Name"/></TT> : 
   <EM>
    <xsl:apply-templates select="Comment"/>
   </EM>
  </LI>
 </xsl:template>
 
  <xsl:template match="/Documentation/Library/Classes">
  <H1>
   Classes
  </H1>
   <xsl:apply-templates select="Classe" mode="index"/>
   <xsl:apply-templates select="Classe"/>
 </xsl:template>
 
 <xsl:template match="/Documentation/Library/Classes/Classe" mode="index">
  <xsl:call-template name="index">
   <xsl:with-param name="discriminator" select="parent::*/parent::*/Name"/>
  </xsl:call-template>
 </xsl:template>

 
 <xsl:template match="/Documentation/Library/Classes/Classe">
  <xsl:call-template name="index-target">
   <xsl:with-param name="discriminator" select="parent::*/parent::*/Name"/>
  </xsl:call-template>
   <H2>
    <xsl:value-of select="Name"/>
   </H2>
     <xsl:apply-templates select="Base_classes"/>
   <EM>
    <xsl:apply-templates select="Comment"/>
   </EM>
     <xsl:apply-templates select="Objects"/>
     <xsl:apply-templates select="Functions"/>
</xsl:template>
 
 <xsl:template match="/Documentation/Library/Classes/Classe/Functions[@Type='Methods']">
  <H3>
   Methods
  </H3>
   <xsl:apply-templates select="Function" mode="index"/>
  <UL>
   <xsl:apply-templates select="Function"/>
  </UL>
 </xsl:template>
 
 <xsl:template match="/Documentation/Library/Classes/Classe/Functions[@Type='Handlers']">
  <H3>
   Handlers
  </H3>
   <xsl:apply-templates select="Function" mode="index"/>
  <UL>
   <xsl:apply-templates select="Function"/>
  </UL>
 </xsl:template>
 

 <xsl:template match="/Documentation/Library/Functions">
  <H1>
   Functions
  </H1>
   <xsl:apply-templates select="Function" mode="index"/>
  <UL>
   <xsl:apply-templates select="Function"/>
  </UL>
 </xsl:template>
 
  <xsl:template match="/Documentation/Library/Classes/Classe/Base_classes">
  <UL>
   <xsl:apply-templates select="Name"/>
  </UL>
 </xsl:template>
 
  <xsl:template match="/Documentation/Library/Classes/Classe/Objects">
  <H3>
   Objects
  </H3>
  <UL>
   <xsl:apply-templates select="Object"/>
  </UL>
 </xsl:template>
 
 <xsl:template match="/Documentation/Library/Classes/Classe/Base_classes/Name">
  <LI>
   <TT>
     <xsl:value-of select="."/>
   </TT>	
  </LI>
 </xsl:template>
 
 <xsl:template match="/Documentation/Library/Classes/Classe/Objects/Object">
  <LI>
   <TT>
     <xsl:value-of select="Type"/>
	 <xsl:text disable-output-escaping="yes"> </xsl:text>
    <B>
     <xsl:value-of select="Name"/> : 
    </B>
   </TT>	
    <EM>
     <xsl:apply-templates select="Comment"/>
    </EM>
  </LI>
 </xsl:template>
 
 <xsl:template match="Function" mode="index">
  <xsl:call-template name="index">
   <xsl:with-param name="discriminator" select="parent::*/parent::*/Name"/>
  </xsl:call-template>
 </xsl:template>


 <xsl:template match="Function">
  <LI>
  <xsl:call-template name="index-target">
   <xsl:with-param name="discriminator" select="parent::*/parent::*/Name"/>
  </xsl:call-template>
   <TT>
    <B>
     <xsl:value-of select="Name"/> : 
    </B>
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
     <B><xsl:text disable-output-escaping="yes"> </xsl:text>
      <xsl:value-of select="Name"/>
     </B>
    </EM>
   </TT>
  </LI>
 </xsl:template>
 
 <xsl:template match="Function/Parameters/Parameter/Value">
  = <xsl:value-of select="."/>
 </xsl:template>

 <xsl:template name="index">
  <xsl:param name="discriminator"/>
  <xsl:text disable-output-escaping="yes">&lt;A HREF="#</xsl:text>
  <xsl:value-of select="$discriminator"/>
  <xsl:text disable-output-escaping="yes">.</xsl:text>
  <xsl:value-of select="Name"/>
  <xsl:text disable-output-escaping="yes">"&gt;</xsl:text>
  <xsl:value-of select="Name"/>
  <xsl:text disable-output-escaping="yes">&lt;/A&gt; </xsl:text>
 </xsl:template>
 
 
 <xsl:template name="index-target">
  <xsl:param name="discriminator"/>
    <xsl:text disable-output-escaping="yes">&lt;A NAME="</xsl:text>
  <xsl:value-of select="$discriminator"/>
  <xsl:text disable-output-escaping="yes">.</xsl:text>
	<xsl:value-of select="Name"/>
	<xsl:text disable-output-escaping="yes">"/&gt;</xsl:text>
 </xsl:template>


</xsl:stylesheet> 
