/*
 * VorbisFile.cpp
 *
 *  Created on: 5 de gen. 2018
 *      Author: biel
 */

#define OV_EXCLUDE_STATIC_CALLBACKS
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#include "../idlib/sys/sys_defines.h"
#include "../idlib/sys/sys_types.h"
#include "../framework/FileSystem.h"

#include "sound/snd_local.h"

#include "VorbisFile.h"

namespace BFG
{

VorbisFile::VorbisFile() {
	// TODO Auto-generated constructor stub

}

VorbisFile::~VorbisFile() {
	// TODO Auto-generated destructor stub
}

/*
===================================================================================

  Thread safe decoder memory allocator.

  Each OggVorbis decoder consumes about 150kB of memory.

===================================================================================
*/

idDynamicBlockAlloc<byte, 1<<20, 128>		decoderMemoryAllocator;

const int MIN_OGGVORBIS_MEMORY				= 768 * 1024;

extern "C" {
	void *_decoder_malloc( size_t size );
	void *_decoder_calloc( size_t num, size_t size );
	void *_decoder_realloc( void *memblock, size_t size );
	void _decoder_free( void *memblock );
}

void *_decoder_malloc( size_t size ) {
	void *ptr = decoderMemoryAllocator.Alloc( size );
	assert( size == 0 || ptr != NULL );
	return ptr;
}

void *_decoder_calloc( size_t num, size_t size ) {
	void *ptr = decoderMemoryAllocator.Alloc( num * size );
	assert( ( num * size ) == 0 || ptr != NULL );
	memset( ptr, 0, num * size );
	return ptr;
}

void *_decoder_realloc( void *memblock, size_t size ) {
	void *ptr = decoderMemoryAllocator.Resize( (byte *)memblock, size );
	assert( size == 0 || ptr != NULL );
	return ptr;
}

void _decoder_free( void *memblock ) {
	decoderMemoryAllocator.Free( (byte *)memblock );
}


/*
===================================================================================

  OggVorbis file loading/decoding.

===================================================================================
*/

/*
====================
FS_ReadOGG
====================
*/
size_t FS_ReadOGG( void *dest, size_t size1, size_t size2, void *fh ) {
	idFile *f = reinterpret_cast<idFile *>(fh);
	return f->Read( dest, size1 * size2 );
}

/*
====================
FS_SeekOGG
====================
*/
int FS_SeekOGG( void *fh, ogg_int64_t to, int type ) {
	fsOrigin_t retype = FS_SEEK_SET;

	if ( type == SEEK_CUR ) {
		retype = FS_SEEK_CUR;
	} else if ( type == SEEK_END ) {
		retype = FS_SEEK_END;
	} else if ( type == SEEK_SET ) {
		retype = FS_SEEK_SET;
	} else {
		common->FatalError( "fs_seekOGG: seek without type\n" );
	}
	idFile *f = reinterpret_cast<idFile *>(fh);
	return f->Seek( to, retype );
}

/*
====================
FS_CloseOGG
====================
*/
int FS_CloseOGG( void *fh ) {
	return 0;
}

/*
====================
FS_TellOGG
====================
*/
long FS_TellOGG( void *fh ) {
	idFile *f = reinterpret_cast<idFile *>(fh);
	return f->Tell();
}

/*
====================
ov_openFile
====================
*/
int ov_openFile( idFile *f, OggVorbis_File *vf ) {
	ov_callbacks callbacks;

	memset( vf, 0, sizeof( OggVorbis_File ) );

	callbacks.read_func = FS_ReadOGG;
	callbacks.seek_func = FS_SeekOGG;
	callbacks.close_func = FS_CloseOGG;
	callbacks.tell_func = FS_TellOGG;
	return ov_open_callbacks((void *)f, vf, NULL, -1, callbacks);
}

/*
====================
idWaveFile::OpenOGG
====================
*/
int idWaveFile::OpenOGG( const char* strFileName, waveformatex_t *pwfx ) {
	OggVorbis_File *ov;

	memset( pwfx, 0, sizeof( waveformatex_t ) );

	mhmmio = fileSystem->OpenFileRead( strFileName );
	if ( !mhmmio ) {
		return -1;
	}

	Sys_EnterCriticalSection( CRITICAL_SECTION_ONE );

	ov = new OggVorbis_File;

	if( ov_openFile( mhmmio, ov ) < 0 ) {
		delete ov;
		Sys_LeaveCriticalSection( CRITICAL_SECTION_ONE );
		fileSystem->CloseFile( mhmmio );
		mhmmio = NULL;
		return -1;
	}

	mfileTime = mhmmio->Timestamp();

	vorbis_info *vi = ov_info( ov, -1 );

	mpwfx.Format.nSamplesPerSec = vi->rate;
	mpwfx.Format.nChannels = vi->channels;
	mpwfx.Format.wBitsPerSample = sizeof(short) * 8;
	mdwSize = ov_pcm_total( ov, -1 ) * vi->channels;	// pcm samples * num channels
	mbIsReadingFromMemory = false;

	if ( idSoundSystemLocal::s_realTimeDecoding.GetBool() ) {

		ov_clear( ov );
		fileSystem->CloseFile( mhmmi




o );
		mhmmio = NULL;
		delete ov;

		mpwfx.Format.wFormatTag = WAVE_FORMAT_TAG_OGG;
		mhmmio = fileSystem->OpenFileRead( strFileName );
		mMemSize = mhmmio->Length();

	} else {

		ogg = ov;

		mpwfx.Format.wFormatTag = WAVE_FORMAT_TAG_PCM;
		mMemSize = mdwSize * sizeof( short );
	}

	memcpy( pwfx, &mpwfx, sizeof( waveformatex_t ) );

	Sys_LeaveCriticalSection( CRITICAL_SECTION_ONE );

	isOgg = true;

	return 0;
}

/*
====================
idWaveFile::ReadOGG
====================
*/
int idWaveFile::ReadOGG( byte* pBuffer, int dwSizeToRead, int *pdwSizeRead ) {
	int total = dwSizeToRead;
	char *bufferPtr = (char *)pBuffer;
	OggVorbis_File *ov = (OggVorbis_File *) ogg;

	do {
		int ret = ov_read( ov, bufferPtr, total >= 4096 ? 4096 : total, Swap_IsBigEndian(), 2, 1, NULL );
		if ( ret == 0 ) {
			break;
		}
		if ( ret < 0 ) {
			return -1;
		}
		bufferPtr += ret;
		total -= ret;
	} while( total > 0 );

	dwSizeToRead = (byte *)bufferPtr - pBuffer;

	if ( pdwSizeRead != NULL ) {
		*pdwSizeRead = dwSizeToRead;
	}

	return dwSizeToRead;
}

/*
====================
idWaveFile::CloseOGG
====================
*/
int idWaveFile::CloseOGG( void ) {
	OggVorbis_File *ov = (OggVorbis_File *) ogg;
	if ( ov != NULL ) {
		Sys_EnterCriticalSection( CRITICAL_SECTION_ONE );
		ov_clear( ov );
		delete ov;
		Sys_LeaveCriticalSection( CRITICAL_SECTION_ONE );
		fileSystem->CloseFile( mhmmio );
		mhmmio = NULL;
		ogg = NULL;
		return 0;
	}
	return -1;
}

} // namespace BFG
