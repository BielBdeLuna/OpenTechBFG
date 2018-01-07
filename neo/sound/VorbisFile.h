/*
 * VorbisFile.h
 *
 *  Created on: 5 de gen. 2018
 *      Author: biel
 */

#ifndef NEO_SOUND_VORBISFILE_H_
#define NEO_SOUND_VORBISFILE_H_

#ifdef ID_DEDICATED
// stub-only mode: AL_API and ALC_API shouldn't refer to any dll-stuff
// because the implemenations are in openal_stub.cpp
// this is ensured by defining AL_LIBTYPE_STATIC before including the AL headers
#define AL_LIBTYPE_STATIC
#endif

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

namespace BFG
{

class VorbisFile {
public:
	VorbisFile();
	virtual ~VorbisFile();
};

}; // namespace BFG

#endif /* NEO_SOUND_VORBISFILE_H_ */
