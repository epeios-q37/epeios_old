<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'> 
 <xsl:template match="/">
  <html>
   <body>
    <UL>
     <xsl:apply-templates mode="index"/>
	</UL>
    <xsl:apply-templates/>
   </body>
  </html>
 </xsl:template> 
 
 <xsl:template match="/Documentation/Library" mode="index">
  <LI>
    <xsl:text disable-output-escaping="yes">&lt;A HREF="#</xsl:text>
	<xsl:value-of select="Name"/>
	<xsl:text disable-output-escaping="yes">"&gt;</xsl:text>
	<xsl:value-of select="Name"/>
    <xsl:text disable-output-escaping="yes">&lt;/A&gt;</xsl:text>
  </LI>
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
  <xsl:apply-templates/>
 </xsl:template>

 <xsl:template match="/Documentation/Library/Enums">
  <H1>
   Enums
  </H1>
  <UL>
   <xsl:apply-templates/>
  </UL>
 </xsl:template>
 
 <xsl:template match="/Documentation/Library/Enums/Enum">
  <li>
   <xsl:value-of select="Name"/> : 
   <EM>
    <xsl:value-of select="Comment"/> 
   </EM>
  </li>
  <xsl:apply-templates/>
 </xsl:template>
 
 <xsl:template match="/Documentation/Library/Enums/Enum/Items">
  <UL>
   <xsl:apply-templates/>
  </UL>
 </xsl:template>

 <xsl:template match="/Documentation/Library/Enums/Enum/Items/Item">
  <li>
   <B>
    <xsl:value-of select="Name"/>
   </B> : 
   <xsl:value-of select="Comment"/> 
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
   <xsl:value-of select="Name"/> : 
   <EM>
    <xsl:value-of select="Comment"/> 
   </EM>
  </li>
  <xsl:apply-templates/>
 </xsl:template>
 
 <xsl:template match="/Documentation/Library/Functions">
  <H1>
   Functions
  </H1>
  <UL>
   <xsl:apply-templates/>
  </UL>
 </xsl:template>
 
 <xsl:template match="/Documentation/Library/Functions/Function">
  <li>
   <TT>
    <B>
     <xsl:value-of select="Name"/> : 
    </B>
     <xsl:value-of select="Type"/> 
    <xsl:apply-templates/>
    <em>
     <xsl:value-of select="Comment"/> 
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
 
 <xsl:template match="/Documentation/Library/Classes/Classe">
   <H2>
    <xsl:value-of select="Name"/>
   </H2>
   <EM>
    <xsl:value-of select="Comment"/> 
   </EM>
   <xsl:apply-templates/>
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
    <xsl:apply-templates/>
    <em>
     <xsl:value-of select="Comment"/> 
    </em>
   </TT>	
  </li>
 </xsl:template>
 
 <xsl:template match="/Documentation/Library/Classes/Classe/Methods/Method/Parameters">
  <UL>
   <xsl:apply-templates/>
  </UL>
 </xsl:template>
 
 <xsl:template match="/Documentation/Library/Classes/Classe/Methods/Method/Parameters/Parameter">
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
 
 <xsl:template match="/Documentation/Library/Classes/Classe/Methods/Method/Parameters/Parameter/Value">
  = <xsl:value-of select="."/>
 </xsl:template>



 
 <xsl:template match="Name"/> 
 <xsl:template match="Comment"/> 
 <xsl:template match="Type"/> 
 <xsl:template match="Typedefs"/> 
 <xsl:template match="Defines"/> 
 <xsl:template match="Shortcuts"/> 
 <xsl:template match="Functions"/> 
 <xsl:template match="Classes"/> 
 <xsl:template match="Objects"/> 


</xsl:stylesheet> 
