<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'> 
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
	<em><xsl:value-of select="Description"/></em>
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
	<xsl:text disable-output-escaping="yes">"&gt;</xsl:text>
    Library <EM><xsl:value-of select="Name"/></EM>
   </U></H1>
  </CENTER>
  <UL>
  	<LI><B>Version : <EM><xsl:value-of select="Version"/></EM></B></LI>
	<LI><B>Release : <EM><xsl:value-of select="Release"/></EM></B></LI>
	<LI><B>Coordinators : <EM><xsl:value-of select="Coordinators"/></EM></B></LI>
	</UL>

  <xsl:apply-templates select="Defines"/>
  <xsl:apply-templates select="Shortcuts"/>
  <xsl:apply-templates select="Enums"/>
  <xsl:apply-templates select="Typedefs"/>
  <xsl:apply-templates select="Classes"/>
  <xsl:apply-templates select="Functions"/>
 </xsl:template>
 
 <xsl:template match="Documentation/Library/Defines">
  <H1>
   Defines
  </H1>
  <UL>
   <xsl:apply-templates/>
  </UL>
 </xsl:template>
 
 <xsl:template match="Documentation/Library/Defines/Define">
  <LI>
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
  <UL>
   <xsl:apply-templates/>
  </UL>
 </xsl:template>
 
 <xsl:template match="Documentation/Library/Shortcuts/Shortcut">
  <LI>
   <TT><xsl:value-of select="Name"/> 
   <xsl:apply-templates select="Arguments"/> : 
   <xsl:apply-templates select="Alias"/>
   </TT><BR/>
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
  <UL>
   <xsl:apply-templates select="Enum"/>
  </UL>
 </xsl:template>
 
 <xsl:template match="Enum">
  <li>
   <TT><xsl:value-of select="Name"/></TT> : 
   <EM>
    <xsl:value-of select="Comment"/> 
   </EM>
    <xsl:apply-templates select="Items"/>
  </li>
  <xsl:apply-templates select="Enums"/>
 </xsl:template>
 
 <xsl:template match="/Documentation/Library/Enums/Enum/Items">
  <UL>
   <xsl:apply-templates select="Item"/>
  </UL>
 </xsl:template>

 <xsl:template match="/Documentation/Library/Enums/Enum/Items/Item">
  <li>
   <B><TT>
    <xsl:value-of select="Name"/>
   </TT></B> : 
   <xsl:apply-templates select="Comment"/>
  </li>
 </xsl:template>

 <xsl:template match="/Documentation/Library/Typedefs">
  <H1>
   Typedefs
  </H1>
  <UL>
   <xsl:apply-templates/>
  </UL>
 </xsl:template>
 
 <xsl:template match="/Documentation/Library/Typedefs/Typedef">
  <li>
   <TT><xsl:value-of select="Name"/></TT> : 
   <EM>
    <xsl:apply-templates select="Comment"/>
   </EM>
  </li>
 </xsl:template>
  
 <xsl:template match="/Documentation/Library/Classes/Classe">
   <H2>
    <xsl:value-of select="Name"/>
   </H2>
   <EM>
    <xsl:apply-templates select="Comment"/>
   </EM>
     <xsl:apply-templates select="Methods"/>
</xsl:template>
 
 <xsl:template match="/Documentation/Library/Classes/Classe/Methods">
  <H3>
   Functions
  </H3>
  <UL>
   <xsl:apply-templates/>
  </UL>
 </xsl:template>
 
 <xsl:template match="/Documentation/Library/Classes/Classe/Methods/Method">
  <li>
   <TT>
    <B>
     <xsl:value-of select="Name"/> : 
    </B>
     <xsl:value-of select="Type"/> 
    <xsl:apply-templates select="Parameters"/>
   </TT>	
    <em>
     <xsl:apply-templates select="Comment"/>
    </em>
  </li>
 </xsl:template>
 
 <xsl:template match="/Documentation/Library/Classes/Classe/Methods/Method/Parameters">
  <UL>
   <xsl:apply-templates/>
  </UL>
 </xsl:template>
 
 <xsl:template match="/Documentation/Library/Classes/Classe/Methods/Method/Parameters/Parameter">
  <LI>
     <xsl:value-of select="Type"/>
     <B><xsl:text disable-output-escaping="yes"> </xsl:text>
      <xsl:value-of select="Name"/>
     </B>
    <xsl:apply-templates select="Value"/>
  </LI>
 </xsl:template>
 
 <xsl:template match="/Documentation/Library/Classes/Classe/Methods/Method/Parameters/Parameter/Value">
  = <xsl:value-of select="."/>
 </xsl:template>

 <xsl:template match="/Documentation/Library/Functions">
  <H1>
   Functions
  </H1>
  <UL>
   <xsl:apply-templates select="Function"/>
  </UL>
 </xsl:template>
 
 <xsl:template match="/Documentation/Library/Functions/Function">
  <li>
   <TT>
    <B>
     <xsl:value-of select="Name"/> : 
    </B>
     <xsl:value-of select="Type"/> 
	 <xsl:apply-templates select="Parameters"/>
    <em>
     <xsl:apply-templates select="Comment"/>
    </em>
   </TT>	
  </li>
 </xsl:template>
 
 <xsl:template match="/Documentation/Library/Functions/Function/Parameters">
  <UL>
   <xsl:apply-templates/>
  </UL>
 </xsl:template>
 
 <xsl:template match="/Documentation/Library/Functions/Function/Parameters/Parameter">
  <LI>
   <TT>
    <EM>
     <xsl:value-of select="Type"/>
     <B><xsl:text disable-output-escaping="yes"> </xsl:text>
      <xsl:value-of select="Name"/>
     </B>
    </EM>
    <xsl:apply-templates/>
   </TT>
  </LI>
 </xsl:template>
 
 <xsl:template match="/Documentation/Library/Functions/Function/Parameters/Parameter/Value">
  = <xsl:value-of select="."/>
 </xsl:template>

 <xsl:template match="/Documentation/Library/Classes">
  <H1>
   Classes
  </H1>
   <xsl:apply-templates/>
 </xsl:template>

 <xsl:template match="Name"/> 
 <xsl:template match="Type"/> 
 <xsl:template match="Typedefs"/> 
 <xsl:template match="Defines"/> 
 <xsl:template match="Shortcuts"/> 
 <xsl:template match="Functions"/> 
 <xsl:template match="Classes"/> 
 <xsl:template match="Objects"/> 
 <xsl:template match="Description"/>
 <xsl:template match="Coordinators"/>
 <xsl:template match="Version"/>
 <xsl:template match="Release"/>

</xsl:stylesheet> 
