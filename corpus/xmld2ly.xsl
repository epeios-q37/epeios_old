<?xml version="1.0" encoding="ISO-8859-1"?>
<!--$Id$-->
<!DOCTYPE xcmq2ly.xsl [
<!ENTITY nl "&#10;">
<!ENTITY tab "&#9;">
]>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="text" indent="yes" encoding="ISO-8859-1"/>
	<xsl:template name="Mod12">
		<xsl:param name="Operand"/>
		<xsl:choose>
			<xsl:when test="number($Operand)>=12">
				<xsl:call-template name="Mod12">
					<xsl:with-param name="Operand" select="number($Operand)-12"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$Operand"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="2power">
		<xsl:param name="Value">1</xsl:param>
		<xsl:param name="Indice"/>
		<xsl:choose>
			<xsl:when test="number($Indice)=0">
				<xsl:value-of select="$Value"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:call-template name="2power">
					<xsl:with-param name="Value" select="number($Value)*2"/>
					<xsl:with-param name="Indice" select="number($Indice)-1"/>
				</xsl:call-template>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="dot">
		<xsl:param name="Amount">1</xsl:param>
		<xsl:if test="not(number($Amount)=0)">
			<xsl:text>.</xsl:text>
			<xsl:call-template name="dot">
				<xsl:with-param name="Amount" select="number($Amount)-1"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>
	<xsl:template name="OctaveUpOrDown">
		<xsl:param name="Symbol"/>
		<xsl:param name="Amount"/>
		<xsl:if test="not(number($Amount)=0)">
			<xsl:value-of select="$Symbol"/>
			<xsl:call-template name="OctaveUpOrDown">
				<xsl:with-param name="Symbol" select="$Symbol"/>
				<xsl:with-param name="Amount" select="number($Amount)-1"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>
	<xsl:template name="OctaveUp">
		<xsl:param name="Amount"/>
		<xsl:call-template name="OctaveUpOrDown">
			<xsl:with-param name="Symbol">&apos;</xsl:with-param>
			<xsl:with-param name="Amount" select="$Amount"/>
		</xsl:call-template>
	</xsl:template>
	<xsl:template name="OctaveDown">
		<xsl:param name="Amount"/>
		<xsl:call-template name="OctaveUpOrDown">
			<xsl:with-param name="Symbol">,</xsl:with-param>
			<xsl:with-param name="Amount" select="$Amount"/>
		</xsl:call-template>
	</xsl:template>
	<xsl:template match="Melody">
		<xsl:text>\relative c' { </xsl:text>
		<xsl:apply-templates select="Bar"/>
		<xsl:text> }</xsl:text>
	</xsl:template>
	<xsl:template match="Bar">
		<xsl:apply-templates select="Signature"/>
		<xsl:text>&nl;  |</xsl:text>
		<xsl:apply-templates select="Figures"/>
	</xsl:template>
	<xsl:template match="Signature">
		<xsl:if test="@Diff='yes'">
			<xsl:apply-templates select="Time"/>
			<xsl:apply-templates select="Key"/>
		</xsl:if>
	</xsl:template>
	<xsl:template match="Time">
		<xsl:if test="@Diff='yes'">
			<xsl:text>&nl;  \time </xsl:text>
			<xsl:value-of select="@Numerator"/>
			<xsl:text>/</xsl:text>
			<xsl:value-of select="@Denominator"/>
		</xsl:if>
	</xsl:template>
	<xsl:template match="Key">
		<xsl:if test="@Diff='yes'">
			<xsl:text>&nl;  \key </xsl:text>
			<xsl:value-of select="@Name"/>
			<xsl:choose>
				<xsl:when test="@Accidental='Sharp'">
					<xsl:text>is</xsl:text>
				</xsl:when>
				<xsl:when test="@Accidental='Flat'">
					<xsl:text>es</xsl:text>
				</xsl:when>
			</xsl:choose>
			<xsl:text> \major</xsl:text>
		</xsl:if>
	</xsl:template>
	<xsl:template match="Figures">
		<xsl:apply-templates select="*"/>
	</xsl:template>
	<xsl:template match="Rest">
		<xsl:text> r</xsl:text>
		<xsl:apply-templates select="Duration"/>
	</xsl:template>
	<xsl:template match="Note">
		<xsl:apply-templates select="Pitch"/>
		<xsl:apply-templates select="Duration"/>
		<!--xsl:text> </xsl:text-->
	</xsl:template>
	<xsl:template match="Pitch">
		<xsl:text> </xsl:text>
		<xsl:value-of select="@Name"/>
		<xsl:choose>
			<xsl:when test="@Accidental='Sharp'">
				<xsl:text>is</xsl:text>
			</xsl:when>
			<xsl:when test="@Accidental='Flat'">
				<xsl:text>es</xsl:text>
			</xsl:when>
		</xsl:choose>
		<xsl:if test="number(Diff/@Interval)>5">
			<xsl:variable name="Mod12">
				<xsl:call-template name="Mod12">
					<xsl:with-param name="Operand" select="Diff/@Interval"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:variable name="OctaveOffset">
				<xsl:choose>
					<xsl:when test="$Mod12='0'">
						<xsl:text>0</xsl:text>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text>1</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test="Diff/@Sign='+'">
					<xsl:call-template name="OctaveUp">
						<xsl:with-param name="Amount" select="number(Diff/@Octave)+number($OctaveOffset)"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="Diff/@Sign='-'">
					<xsl:call-template name="OctaveDown">
						<xsl:with-param name="Amount" select="number(Diff/@Octave)+number($OctaveOffset)"/>
					</xsl:call-template>
				</xsl:when>
			</xsl:choose>
		</xsl:if>
	</xsl:template>
	<xsl:template match="Duration">
		<xsl:if test="@Diff='yes'">
			<xsl:call-template name="2power">
				<xsl:with-param name="Indice" select="number(@Base)-1"/>
			</xsl:call-template>
			<xsl:call-template name="dot">
				<xsl:with-param name="Amount" select="@Modifier"/>
			</xsl:call-template>
		</xsl:if>
		<xsl:if test="@TiedToNext='yes'">
			<xsl:text> ~</xsl:text>
		</xsl:if>
	</xsl:template>
</xsl:stylesheet>