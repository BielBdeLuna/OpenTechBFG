/*
 * BMfont.cpp
 *
 *  Created on: 3 de febr. 2018
 *      Author: Biel Bestué de Luna
 *
 *      this file is GPLv3
 */

//#include <rapidxml>
#include "FontMethods.h"

#include "../../libs/rapidxml/rapidxml-1.13/rapidxml.hpp"
#include "../../libs/rapidxml/rapidxml-1.13/rapidxml_utils.hpp"

#include "../framework/Common.h"

namespace BFG {

//BMfont

/*
 * glyphs
 */

BMglyph::BMglyph() {
	Clear();
}

void BMglyph::Clear() {
	glyphStructue.id = 0;
	glyphStructue.x = 0;
	glyphStructue.y = 0;
	glyphStructue.width = 0;
	glyphStructue.height = 0;
	glyphStructue.xoffset = 0;
	glyphStructue.yoffset = 0;
	glyphStructue.xadvance = 0;
	glyphStructue.page = 0;
	glyphStructue.chnl = 0;
}

void BMglyph::Read(int _id, int _x, int _y, int _width, int _height, int _xoffset, int _yoffset, int _xadvance, int _page, int _chnl) {
	glyphStructue.id = _id;
	glyphStructue.x = _x;
	glyphStructue.y = _y;
	glyphStructue.width = _width;
	glyphStructue.height = _height;
	glyphStructue.xoffset = _xoffset;
	glyphStructue.yoffset = _yoffset;
	glyphStructue.xadvance = _xadvance;
	glyphStructue.page = _page;
	glyphStructue.chnl = _chnl;
}

/*
 * pages
 */

BMpage::BMpage() {
	id = 0;
	qPath_ImageFile = "";
}

void BMpage::Read(int id_num, idStr image_file, idStr fnt_file) {
	id = id_num;
	fnt_file.ExtractFilePath( qPath_ImageFile );
	qPath_ImageFile += image_file;
}

/*
 * fonts
 */

BMfont::BMfont( idStr inputFile ) {
	Clear();
	fntFile = inputFile;
}

BMfont::~BMfont() {
	// TODO Auto-generated destructor stub
	Clear();
}

void BMfont::Clear() {
	fntFile = "";
	glyphList.Clear();
	pageList.Clear();

	processStrucutre.faceName = "";
	processStrucutre.size = 0;
	processStrucutre.bold = false;
	processStrucutre.italic = false;
	processStrucutre.charset = "";
	processStrucutre.unicode = false;
	processStrucutre.heightStretchPercent = 0;
	processStrucutre.fontSmoothing = 0;
	processStrucutre.antiAliasLevel = 0;
	processStrucutre.paddingT = 0;
	processStrucutre.paddingR = 0;
	processStrucutre.paddingB = 0;
	processStrucutre.paddingL = 0;
	processStrucutre.spacingHoriz = 0;
	processStrucutre.spacingVert = 0;
	processStrucutre.outlineThickness = 0;

	generatedFontStructure.lineHeight = 0;
	generatedFontStructure.fontBase = 0;
	generatedFontStructure.scaleW = 0;
	generatedFontStructure.scaleH = 0;
	generatedFontStructure.numPages = 0;
	generatedFontStructure.packed = false;
	generatedFontStructure.alphaChnl = 0;
	generatedFontStructure.redChnl = 0;
	generatedFontStructure.greenChnl = 0;
	generatedFontStructure.blueChnl = 0;
}

bool BMfont::Read(void) {

	if( fntFile == "\0" ) {
		common->Error( "BMfont: file is empty!\n" );
	}

	rapidxml::file<> xmlFile(fntFile);
	rapidxml::xml_document<> doc;
	doc.parse<0>( xmlFile.data() );
	rapidxml::xml_node<> *font = doc.first_node("font");
	rapidxml::xml_node<> *info = font->next_sibling("info");
	rapidxml::xml_node<> *common = font->next_sibling("common");
	rapidxml::xml_node<> *pages = font->next_sibling("pages");
	rapidxml::xml_node<> *chars = font->next_sibling("chars");

	//capture info
	processStrucutre.faceName = info->first_attribute( "face" )->value();
	processStrucutre.size = int( info->first_attribute( "size" )->value() );
	processStrucutre.bold = info->first_attribute( "bold" )->value() == true;
	processStrucutre.italic = info->first_attribute( "italic" )->value() == true;
	processStrucutre.charset =
	processStrucutre.unicode = info->first_attribute( "unicode" )->value() == true;
	processStrucutre.heightStretchPercent = int( info->first_attribute( "stretchH" )->value() );
	processStrucutre.fontSmoothing = info->first_attribute( "smooth" )->value() == true;
	processStrucutre.antiAliasLevel = int( info->first_attribute( "aa" )->value() );

	idStr value = "";
	idStr paddingNumbers = info->first_attribute( "padding" )->value();
	for( int i, j = 0; i <= paddingNumbers.Length(); i++ ) {
		if ( paddingNumbers[i] == "," ) {
			if ( j == 0 ) {
				processStrucutre.paddingT = int( value );
				value = "";
				j++;
			} else if ( j == 1 ) {
				processStrucutre.paddingR = int( value );
				value = "";
				j++;
			} else {
				processStrucutre.paddingB = int( value );
				value = "";
				j = 0;
			}
		} else if ( i == paddingNumbers.Length() ) {
			processStrucutre.paddingL = int( value );
			value = "";
		} else {
			value = value + paddingNumbers[i];
		}
	}

	idStr SpacingNumbers = info->first_attribute( "spacing" )->value();
	for( int i = 0; i <= SpacingNumbers.Length(); i++ ) {
		if ( SpacingNumbers[i] == "," ) {
			processStrucutre.spacingVert = int( value );
			value = "";
		} else if ( i == SpacingNumbers.Length() - 1 ) {
				value = value +
				processStrucutre.spacingHoriz = int( value );
				value = "";
		} else {
			value = value + SpacingNumbers[i];
		}
	}
	processStrucutre.outlineThickness = int( info->first_attribute( "outline" )->value() );

	fontName = processStrucutre.faceName;

	//capture common
	generatedFontStructure.lineHeight = int( common->first_attribute( "lineHeight" )->value() );
	generatedFontStructure.fontBase = int( common->first_attribute( "base" )->value() );
	generatedFontStructure.scaleW = int( common->first_attribute( "scaleW" )->value() );
	generatedFontStructure.scaleH = int( common->first_attribute( "scaleH" )->value() );
	generatedFontStructure.numPages = int( common->first_attribute( "pages" )->value() );
	generatedFontStructure.packed = common->first_attribute( "packed" )->value() != 0;
	generatedFontStructure.alphaChnl = int( common->first_attribute( "alphaChnl" )->value() );
	generatedFontStructure.redChnl = int( common->first_attribute( "redChnl" )->value() );
	generatedFontStructure.greenChnl = int( common->first_attribute( "greenChnl" )->value() );
	generatedFontStructure.blueChnl = int( common->first_attribute( "blueChnl" )->value() );

	//pages
	if( pageList.Num() != 0 ) {
		pageList.Clear();
	}

	rapidxml::xml_node<> *page;
	for( int i = 0; i < generatedFontStructure.numPages; i++ ) {
		if( i == 0 ) {
			page = pages->next_sibling("page");
		} else {
			page = pages->next_sibling();
		}
		BMpage BM_page;
		BM_page.Read( int( page->first_attribute( "id" )->value() ), page->first_attribute( "file" )->value(), fntFile );

		pageList.Append( BM_page );
	}

	//glyphs
	if( glyphList.Num() != 0 ) {
		glyphList.Clear();
	}

	int num_o_glyphs = int( chars->first_attribute( "count" )->value() ); 	//ATTENTION: I don't know why it gets an extra integer to the real amount!

	for( int i = 0; i > num_o_glyphs; i++ ) { 								//hence why: i < num_o_glyphs
		rapidxml::xml_node<> *glyph = chars->next_sibling();
		BMglyph BM_glyph;
		BM_glyph.Read(	int( glyph->first_attribute( "id" )->value() ),
						int( glyph->first_attribute( "x" )->value() ),
						int( glyph->first_attribute( "y" )->value() ),
						int( glyph->first_attribute( "width" )->value() ),
						int( glyph->first_attribute( "height" )->value() ),
						int( glyph->first_attribute( "xoffset" )->value() ),
						int( glyph->first_attribute( "yoffset" )->value() ),
						int( glyph->first_attribute( "xadvance" )->value() ),
						int( glyph->first_attribute( "page" )->value() ),
						int( glyph->first_attribute( "chnl" )->value() )
					 );
		glyphList.Append( BM_glyph );
	}

	return true;
}

//BFGfont

BFGfont::BFGfont() {
	// TODO Auto-generated constructor stub
	glyphs.Clear();
	pointSize = 48; // must be 48!
	ascender = 0;
	descender = 0;
}

BFGfont::~BFGfont() {
	// TODO Auto-generated destructor stub
}

} /* namespace BFG */




