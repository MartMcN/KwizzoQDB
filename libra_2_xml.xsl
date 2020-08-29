<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0" xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0" xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0" exclude-result-prefixes="office table text">

 <xsl:template match="/">
  <root>
   <xsl:apply-templates select="/*/office:body" />
  </root>
 </xsl:template>

 <xsl:template match="office:body">
  <xsl:apply-templates />
 </xsl:template>

 <xsl:template match="office:spreadsheet">
  <xsl:apply-templates />
 </xsl:template>

 <xsl:template match="office:spreadsheet/table:table">

   <xsl:for-each select="table:table-row[position() &gt; 1]">

   <quizz>
    <question><xsl:value-of select="table:table-cell[2]/text:p" /></question>
    <answer><xsl:value-of select="table:table-cell[3]/text:p" /></answer>
    <catagory><xsl:value-of select="table:table-cell[1]/text:p" /></catagory>
   </quizz>

   </xsl:for-each>

 </xsl:template>
</xsl:stylesheet>
