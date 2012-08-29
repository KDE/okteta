<?xml version="1.0"?>
<!--

This stylesheet transoforms the output of gccxml into a structure definition
compatible with Okteta.

Usage:
gccxml example.cpp -fxml=example.gcc.xml
xsltproc -stringparam structs complex gccxml-to-osd.xsl example.gcc.xml > example.osd

See:
https://frinring.wordpress.com/2010/01/16/tutorial-create-your-own-okteta-structure-definitions/
http://www.gccxml.org/
http://utils.kde.org/projects/okteta/

In case of doubts you can contact me at ale@clearmind.me

-->

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output indent="yes" />
	<!-- With this parameter you can filter the structs you're interested in -->
	<xsl:param name="structs" />
	
	<xsl:template match="/GCC_XML">
		<data xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="../../structuredefs.xsd">
		
			<!-- TODO: Filter only necessary enumerations -->
			<xsl:for-each select="Enumeration">
				<xsl:call-template name="CreateEnum" select="." />
			</xsl:for-each>
			
			<xsl:for-each select="Struct">
				<xsl:if test="not($structs) or contains(concat(' ', $structs, ' '), concat(' ', @name, ' '))">
					<xsl:apply-templates select="." />
				</xsl:if>
			</xsl:for-each>
			
		</data>
	</xsl:template>

	<xsl:template name="CreateEnum">
		<enumDef name="{@name}" type="Int{@size}">
			<xsl:for-each select="EnumValue">
				<entry name="{@name}" value="{@init}" />
			</xsl:for-each>
		</enumDef>
	</xsl:template>

	<xsl:template match="Struct">
		<xsl:param name="name" select="@name" />
		<struct name="{$name}">
			<xsl:call-template name="split">
				<xsl:with-param name="list" select="@members" />
				<xsl:with-param name="separator" select="' '" />
	
			</xsl:call-template>
		</struct>
	</xsl:template>

	<xsl:template match="Union">
		<xsl:param name="name" select="@name" />
		<union name="{$name}">
			<xsl:call-template name="split">
				<xsl:with-param name="list" select="@members" />
				<xsl:with-param name="separator" select="' '" />
	
			</xsl:call-template>
		</union>

	</xsl:template>

	<xsl:template match="PointerType">
		<xsl:param name="name" select="false()" />
		<primitive name="{$name}" type="UInt{@size}" />
	</xsl:template>
	
	<xsl:template match="ReferenceType">
		<xsl:param name="name" select="false()" />
		<primitive name="{$name}" type="UInt{@size}" />
	</xsl:template>


	<xsl:template match="FundamentalType">
		<xsl:param name="name" select="false()" />
		<xsl:variable name="typename" select="translate(@name, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ', 'abcdefghijklmnoprstuvwxyz')" />
	
		<primitive>
			<xsl:if test="$name">
				<xsl:attribute name="name">
					<xsl:value-of select="$name" />
				</xsl:attribute>
			</xsl:if>
			<xsl:attribute name="type">	
				<xsl:choose>
					<xsl:when test="contains($typename, 'char')">Char</xsl:when>
					<xsl:when test="contains($typename, 'double')">Double</xsl:when>
					<xsl:when test="contains($typename, 'float')">Float</xsl:when>
					<xsl:otherwise>
						<xsl:choose>
							<xsl:when test="contains($typename, 'bool')">Bool</xsl:when>
							<xsl:when test="contains($typename, 'unsigned')">UInt</xsl:when>
							<xsl:otherwise>Int</xsl:otherwise>
						</xsl:choose><xsl:value-of select="@size" />
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>
		</primitive>
	
	</xsl:template>

	<xsl:template match="ArrayType">
		<xsl:param name="name" />
		<xsl:variable name="typename" select="@type" />
		<array name="{$name}" length="{translate(@max, 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ', '')+1}">
			<xsl:apply-templates select="/GCC_XML/*[@id=$typename]" />
		</array>
	</xsl:template>

	<xsl:template match="Typedef">
		<xsl:param name="name" select="@name" />
		<xsl:variable name="typename" select="@type" />
		<xsl:apply-templates select="/GCC_XML/*[@id=$typename]">	
			<xsl:with-param name="name" select="$name" />
		</xsl:apply-templates>
	</xsl:template>

	<xsl:template match="Enumeration">
		<xsl:param name="name" select="@name" />
		<enum name="{$name}" enum="{@name}" type="Int{@size}" />
	</xsl:template>

	<!-- TODO: There could be an offset -->
	<xsl:template match="Field">
		<xsl:variable name="type" select="@type" />
		<xsl:variable name="typeelement" select="/GCC_XML/*[@id=$type]" />
		<xsl:variable name="name" select="@name" />

		<!-- We have to propagate down the field name -->
		<xsl:apply-templates select="$typeelement">
			<xsl:with-param name="name" select="$name" />
		</xsl:apply-templates>

	</xsl:template>

	<xsl:template name="split">
		<xsl:param name="list"      select="''" />
		<xsl:param name="separator" select="','" />

		<xsl:if test="not($list = '' or $separator = '')">
			<xsl:variable name="head" select="substring-before(concat($list, $separator), $separator)" />
			<xsl:variable name="tail" select="substring-after($list, $separator)" />

			<xsl:apply-templates select="/GCC_XML/Field[@id=$head]" />

			<!-- Recurse -->
			<xsl:call-template name="split">
				<xsl:with-param name="list"      select="$tail" />
				<xsl:with-param name="separator" select="$separator" />
			</xsl:call-template>
		</xsl:if>
	</xsl:template>
	
</xsl:stylesheet>
