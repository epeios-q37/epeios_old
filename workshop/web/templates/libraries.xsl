<!--$Id$-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="Libraries">
		<xsl:element name="center">
			<xsl:element name="table">
				<xsl:apply-templates select="Library" mode="index"/>
			</xsl:element>
		</xsl:element>
		<xsl:apply-templates select="Library"/>
	</xsl:template>
	<xsl:template match="Libraries/Library" mode="index">
		<xsl:element name="tr">
			<xsl:element name="td">
				<xsl:element name="tt">
					<xsl:element name="A">
						<xsl:attribute name="href">
							<xsl:text>#</xsl:text>
							<xsl:value-of select="@Name"/>
						</xsl:attribute>
						<xsl:value-of select="@Name"/>
					</xsl:element>
				</xsl:element>
			</xsl:element>
			<xsl:element name="td">
				<xsl:element name="tt">
					<xsl:value-of select="Version"/>
				</xsl:element>
			</xsl:element>
			<xsl:element name="td">
				<xsl:element name="tt">
					<xsl:text>(</xsl:text>
					<xsl:value-of select="Release"/>
					<xsl:text>) : </xsl:text>
				</xsl:element>
			</xsl:element>
			<xsl:element name="td">
				<xsl:element name="tt">
					<xsl:element name="em">
						<xsl:value-of select="Description"/>
					</xsl:element>
				</xsl:element>
			</xsl:element>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Libraries/Library">
		<xsl:element name="hr"/>
			<xsl:element name="center">
				<xsl:element name="h2">
					<xsl:element name="u">
						<xsl:text>Library </xsl:text>
						<xsl:element name="a">
							<xsl:attribute name="name">
								<xsl:value-of select="@Name"/>
							</xsl:attribute>
						</xsl:element>
						<xsl:element name="em">
							<xsl:value-of select="@Name"/>
						</xsl:element>
					</xsl:element>
				</xsl:element>
				<xsl:element name="em">
					<xsl:value-of select="Description"/>
				</xsl:element>
			</xsl:element>
			<xsl:element name="ul">
				<xsl:element name="li">
					<xsl:element name="b">
						<xsl:text>Version : </xsl:text>
						<xsl:element name="em">
							<xsl:value-of select="Version"/>
						</xsl:element>
					</xsl:element>
				</xsl:element>
				<xsl:element name="li">
					<xsl:element name="b">
						<xsl:text>Release : </xsl:text>
						<xsl:element name="em">
							<xsl:value-of select="Release"/>
						</xsl:element>
					</xsl:element>
				</xsl:element>
				<xsl:element name="li">
					<xsl:element name="b">
					<xsl:text>Coordinators : </xsl:text>
					<xsl:element name="em">
						<xsl:value-of select="Coordinators"/>
					</xsl:element>
				</xsl:element>
			</xsl:element>
		</xsl:element>
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
		<xsl:element name="a">
			<xsl:attribute name="href">
				<xsl:text>#</xsl:text>
				<xsl:value-of select="$item/parent::*/@Name"/>
				<xsl:text>.</xsl:text>
				<xsl:value-of select="name($item)"/>
			</xsl:attribute>
			<xsl:value-of select="name($item)"/>
		</xsl:element>
	</xsl:template>
	<xsl:template name="lib-index-target">
		<xsl:param name="item"/>
		<xsl:element name="a">
			<xsl:attribute name="name">
				<xsl:value-of select="$item/parent::*/@Name"/>
				<xsl:text>.</xsl:text>
				<xsl:value-of select="name($item)"/>
			</xsl:attribute>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Libraries/Library/Defines" mode="index">
		<xsl:element name="a">
			<xsl:attribute name="href">
				<xsl:text>#</xsl:text>
				<xsl:value-of select="parent::*/@Name"/>
				<xsl:text>.</xsl:text>
				<xsl:value-of select="name(.)"/>
			</xsl:attribute>
			<xsl:value-of select="name(.)"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Libraries/Library/Defines">
		<xsl:element name="h2">
			<xsl:text>Defines</xsl:text>
		</xsl:element>
		<xsl:apply-templates mode="index"/>
		<xsl:element name="ul">
			<xsl:apply-templates/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Libraries/Library/Defines/Define" mode="index">
		<xsl:call-template name="index">
			<xsl:with-param name="discriminator" select="parent::*/parent::*/@Name"/>
		</xsl:call-template>
	</xsl:template>
	<xsl:template match="Libraries/Library/Defines/Define">
		<xsl:element name="li">
			<xsl:call-template name="index-target">
				<xsl:with-param name="discriminator" select="parent::*/parent::*/@Name"/>
			</xsl:call-template>
			<xsl:element name="tt">
				<xsl:value-of select="@Name"/>
			</xsl:element>
			<xsl:apply-templates select="Arguments"/>
			<xsl:text>: </xsl:text>
			<xsl:element name="em">
				<xsl:apply-templates select="Comment"/>
			</xsl:element>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Libraries/Library/Defines/Define/Arguments">
		<xsl:text>(</xsl:text>
		<xsl:apply-templates/>
		<xsl:text>)</xsl:text>
	</xsl:template>
	<xsl:template match="Libraries/Library/Defines/Define/Arguments/Argument">
		<xsl:element name="b">
		  <xsl:value-of select="@Name"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Libraries/Library/Shortcuts">
		<xsl:element name="h2">
			<xsl:text>Shortcuts</xsl:text>
		</xsl:element>
		<xsl:apply-templates mode="index"/>
		<xsl:element name="ul">
			<xsl:apply-templates/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Libraries/Library/Shortcuts/Shortcut" mode="index">
		<xsl:call-template name="index">
			<xsl:with-param name="discriminator" select="parent::*/parent::*/@Name"/>
		</xsl:call-template>
	</xsl:template>
	<xsl:template match="Libraries/Library/Shortcuts/Shortcut">
		<xsl:element name="li">
			<xsl:call-template name="index-target">
				<xsl:with-param name="discriminator" select="parent::*/parent::*/@Name"/>
			</xsl:call-template>
			<xsl:element name="tt">
				<xsl:value-of select="@Name"/>
				<xsl:apply-templates select="Arguments"/>
				<xsl:text>: </xsl:text>
				<xsl:apply-templates select="Alias"/>
			</xsl:element>
			<xsl:element name="br"/>
			<xsl:element name="em">
				<xsl:apply-templates select="Comment"/>
			</xsl:element>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Libraries/Library/Shortcuts/Shortcut/Arguments">
		<xsl:text>(</xsl:text>
		<xsl:element name="b">
			<xsl:apply-templates/>
		</xsl:element>
		<xsl:text>)</xsl:text>
	</xsl:template>
	<xsl:template match="Libraries/Library/Shortcuts/Shortcut/Arguments/Argument">
		<xsl:value-of select="@Name"/>
		<xsl:text>,</xsl:text>
	</xsl:template>
	<xsl:template match="Libraries/Library/Enums">
		<xsl:element name="h2">
			<xsl:text>Enums</xsl:text>
		</xsl:element>
		<xsl:apply-templates mode="index"/>
		<xsl:element name="ul">
			<xsl:apply-templates select="Enum"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Enum" mode="index">
		<xsl:call-template name="index">
			<xsl:with-param name="discriminator" select="parent::*/parent::*/@Name"/>
		</xsl:call-template>
	</xsl:template>
	<xsl:template match="Enum">
		<xsl:element name="li">
			<xsl:call-template name="index-target">
				<xsl:with-param name="discriminator" select="parent::*/parent::*/@Name"/>
			</xsl:call-template>
		 	<xsl:element name="tt">
				<xsl:value-of select="@Name"/>
			</xsl:element>
			<xsl:text>: </xsl:text>
			<xsl:element name="em">
				<xsl:value-of select="Comment"/>
			</xsl:element>
			<xsl:apply-templates select="Items"/>
		</xsl:element>
		<xsl:apply-templates select="Enums"/>
	</xsl:template>
	<xsl:template match="Libraries/Library/Enums/Enum/Items">
		<xsl:element name="ul">
			<xsl:apply-templates select="Item"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Libraries/Library/Enums/Enum/Items/Item">
		<xsl:element name="li">
			<xsl:element name="b">
				<xsl:element name="tt">
					<xsl:value-of select="@Name"/>
				</xsl:element>
			</xsl:element>
			<xsl:text>: </xsl:text>
			<xsl:apply-templates select="Comment"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Libraries/Library/Typedefs">
		<xsl:element name="h2">
			<xsl:text>Typedefs</xsl:text>
		</xsl:element>
		<xsl:apply-templates select="Typedef" mode="index"/>
		<xsl:element name="ul">
			<xsl:apply-templates select="Typedef"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Libraries/Library/Typedefs/Typedef" mode="index">
		<xsl:call-template name="index">
			<xsl:with-param name="discriminator" select="parent::*/parent::*/@Name"/>
		</xsl:call-template>
	</xsl:template>
	<xsl:template match="Libraries/Library/Typedefs/Typedef">
		<xsl:element name="li">
			<xsl:call-template name="index-target">
				<xsl:with-param name="discriminator" select="parent::*/parent::*/@Name"/>
			</xsl:call-template>
			<xsl:element name="tt">
				<xsl:value-of select="@Name"/>
			</xsl:element>
			<xsl:text>: </xsl:text>
			<xsl:element name="em">
				<xsl:apply-templates select="Comment"/>
			</xsl:element>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Libraries/Library/Classes">
		<xsl:element name="h2">
			<xsl:text>Classes</xsl:text>
		</xsl:element>
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
		<xsl:element name="h3">
			<xsl:value-of select="@Name"/>
			<xsl:apply-templates select="Templates"/>
		</xsl:element>
		<xsl:apply-templates select="Base_classes"/>
		<xsl:element name="em">
			<xsl:apply-templates select="Comment"/>
		</xsl:element>
		<xsl:apply-templates select="Objects"/>
		<xsl:apply-templates select="Functions"/>
	</xsl:template>
	<xsl:template match="Libraries/Library/Classes/Classe/Functions[@Type='Methods']">
		<xsl:element name="h4">
			<xsl:text>Methods</xsl:text>
		</xsl:element>
		<xsl:apply-templates select="Function" mode="index"/>
		<xsl:element name="ul">
			<xsl:apply-templates select="Function"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Libraries/Library/Classes/Classe/Functions[@Type='Handlers']">
		<xsl:element name="h4">
			<xsl:text>Handlers</xsl:text>
		</xsl:element>
		<xsl:apply-templates select="Function" mode="index"/>
		<xsl:element name="ul">
			<xsl:apply-templates select="Function"/>
		</xsl:element>
	  </xsl:template>
	  <xsl:template match="Libraries/Library/Functions">
		<xsl:element name="h2">
			<xsl:text>Functions</xsl:text>
		</xsl:element>
		<xsl:apply-templates select="Function" mode="index"/>
		<xsl:element name="ul">
			<xsl:apply-templates select="Function"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Libraries/Library/Classes/Classe/Templates">
		<xsl:element name="tt">&lt; <xsl:apply-templates select="Template"/> &gt;</xsl:element>
	</xsl:template>
	<xsl:template match="Libraries/Library/Classes/Classe/Base_classes">
		<xsl:element name="ul">
			<xsl:apply-templates select="Base_class"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Libraries/Library/Classes/Classe/Objects">
		<xsl:element name="h4">
			<xsl:text>Objects</xsl:text>
		</xsl:element>
		<xsl:element name="ul">
			<xsl:apply-templates select="Object"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Libraries/Library/Classes/Classe/Templates/Template">
		<xsl:element name="em">
			<xsl:value-of select="Type"/>
		</xsl:element>
		<xsl:text> </xsl:text>
		<xsl:value-of select="@Name"/>
		<xsl:if test="not(position()=last())">
			<xsl:text>, </xsl:text>
		</xsl:if>
	</xsl:template>
	<xsl:template match="Libraries/Library/Classes/Classe/Base_classes/Base_class">
		<xsl:element name="li">
			<xsl:element name="tt">
				<xsl:value-of select="@Name"/>
			</xsl:element>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Libraries/Library/Classes/Classe/Objects/Object">
		<xsl:element name="li">
			<xsl:element name="tt">
				<xsl:value-of select="Type"/>
				<xsl:text> </xsl:text>
				<xsl:element name="b">
					<xsl:value-of select="@Name"/>
					<xsl:text>: </xsl:text>
				</xsl:element>
			</xsl:element>
			<xsl:element name="em">
				<xsl:apply-templates select="Comment"/>
			</xsl:element>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Function" mode="index">
		<xsl:call-template name="index">
			<xsl:with-param name="discriminator" select="parent::*/parent::*/@Name"/>
		</xsl:call-template>
	</xsl:template>
	<xsl:template match="Function">
		<xsl:element name="li">
			<xsl:call-template name="index-target">
				<xsl:with-param name="discriminator" select="parent::*/parent::*/@Name"/>
			</xsl:call-template>
			<xsl:element name="tt">
				<xsl:element name="b">
					<xsl:value-of select="@Name"/>
					<xsl:text> : </xsl:text>
				</xsl:element>
				<xsl:value-of select="Type"/>
				<xsl:apply-templates select="Parameters"/>
				<xsl:element name="em">
					<xsl:apply-templates select="Comment"/>
				</xsl:element>
			</xsl:element>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Function/Parameters">
		<xsl:element name="ul">
			<xsl:apply-templates/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Function/Parameters/Parameter">
		<xsl:element name="li">
			<xsl:element name="tt">
				<xsl:element name="em">
					<xsl:value-of select="Type"/>
					<xsl:element name="b">
						<xsl:text> </xsl:text>
						<xsl:value-of select="@Name"/>
					</xsl:element>
				</xsl:element>
				<xsl:apply-templates select="Value"/>
			</xsl:element>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Function/Parameters/Parameter/Value">
		<xsl:text> = </xsl:text>
		<xsl:value-of select="."/>
	</xsl:template>
	<xsl:template name="index">
		<xsl:param name="discriminator"/>
		<xsl:if test="not(preceding-sibling::*/@Name=@Name)">
			<xsl:element name="a">
				<xsl:attribute name="href">
					<xsl:text>#</xsl:text>
					<xsl:value-of select="$discriminator"/>
					<xsl:text>.</xsl:text>
					<xsl:value-of select="@Name"/>
				</xsl:attribute>
				<xsl:value-of select="@Name"/>
			</xsl:element>
		</xsl:if>
	</xsl:template>
	<xsl:template name="index-target">
		<xsl:param name="discriminator"/>
		<xsl:if test="not(preceding-sibling::*/@Name=@Name)">
			<xsl:element name="a">
				<xsl:attribute name="name">
					<xsl:value-of select="$discriminator"/>
					<xsl:text>.</xsl:text>
					<xsl:value-of select="@Name"/>
				</xsl:attribute>
			</xsl:element>
		</xsl:if>
	</xsl:template>
</xsl:stylesheet>
