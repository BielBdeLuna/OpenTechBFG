/*
 * BMfont.h
 *
 *  Created on: 3 de febr. 2018
 *      Author: Biel Bestué de Luna
 *
 *      this file is GPLv3
 */

#ifndef NEO_TOOLS_COMPILERS_FONTCODEC_BMFONT_H_
#define NEO_TOOLS_COMPILERS_FONTCODEC_BMFONT_H_

#include "../idlib/Str.h"
#include "../idlib/containers/List.h"

#include "../tools/compilers/fontcodec/fontcodec.h"

namespace BFG {
/*
 * These are classes represent fonts generated by BMFont
 */

typedef struct
{
    int 	id;        	// char utf32
    int 	x;         	// x position in image (in pixels)
    int 	y;         	// y position in image (in pixels)
    int 	width;    	// width in image (in pixels)
    int 	height;    	// height in image (in pixels)
    int 	xoffset;   	// offset from pen position to start of glyph (in pixels)
    int 	yoffset;  	// offset from top of the line to start of glyph (in pixels)
    int 	xadvance;	// how much to move pen after drawing glyph (in pixels)
    int 	page;      	// image number
    int 	chnl;      	// channel mask in the image (0xF == all channels) (important when chars are packed)
} BMglyphStructure_t;

// information about how the font was generated
typedef struct
{
	idStr 	faceName;
	int		size;		// font size in points
	bool 	bold;
  	bool 	italic;
  	idStr	charset;
	bool	unicode;    // true if unicode (charset is empty then)
	int 	heightStretchPercent;
	bool 	fontSmoothing;
 	int 	antiAliasLevel;
 	int 	paddingT;
  	int 	paddingR;
  	int 	paddingB;
   	int 	paddingL;
  	int 	spacingHoriz;
   	int 	spacingVert;
  	int 	outlineThickness;
} BMprocessfontStructure_t;

// information about the generated font
typedef struct
{
	int 	lineHeight;	// line height
	int		fontBase;	// dist from top to baseline
	int		scaleW;  	// width of the picture
	int 	scaleH;  	// height of the picture
	int 	numPages;	// numeber of pictures
	bool 	packed;     // true if glyphs are packed into multiple channels
	int		alphaChnl;	// contents of alpha channel: 0 - glyph, 1 - outline, 2 - encoded glyph & outline, 3 - zero, 4 - one
	int		redChnl;  	// same
	int		greenChnl; 	// same
	int 	blueChnl;  	// same
} BMgeneratedfontStructure_t;

class BMglyph {
public:
	BMglyph();

	void Load(idStr glyphLine );

	const BMglyphStructure_t& getGlyphStructue() const {
		return glyphStructue;
	}

	void setGlyphStructue(const BMglyphStructure_t& glyphStructue) {
		this->glyphStructue = glyphStructue;
	}

private:
	BMglyphStructure_t glyphStructue;
};

class BMpage {
public:
	BMpage();

	bool Load( idStr line );

private:
	int id;
    idStr file;

};

class BMfont {
public:
	BMfont();
	virtual ~BMfont();

	bool LoadInfo( idStr line );
	bool LoadCommon( idStr line );
	void Load( idStr fileName );

	const BMgeneratedfontStructure_t& getGeneratedFontStructure() const {
		return generatedFontStructure;
	}

	void setGeneratedFontStructure(
			const BMgeneratedfontStructure_t& generatedFontStructure) {
		this->generatedFontStructure = generatedFontStructure;
	}

	const BMprocessfontStructure_t& getProcessStrucutre() const {
		return processStrucutre;
	}

	void setProcessStrucutre(const BMprocessfontStructure_t& processStrucutre) {
		this->processStrucutre = processStrucutre;
	}

private:
	idList<BMglyph> 			glyphs;
	idList<BMpage> 				pages;
	BMprocessfontStructure_t	processStrucutre;
	BMgeneratedfontStructure_t 	generatedFontStructure;
};

} /* namespace BFG */

#endif /* NEO_TOOLS_COMPILERS_FONTCODEC_BMFONT_H_ */
