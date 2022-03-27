#pragma once

#include "./zlib.h"
//#include "./crypt/Rijndael.h"
#define CHUNK 16384
#define windowBits 15
#define GZIP_ENCODING 16

__forceinline unsigned char* ZlibCompress(unsigned char *DataPtr, unsigned int lSizeData, unsigned int &lSizeCpr, int level)
{
	unsigned char out[CHUNK];
	z_stream strm;
	unsigned char *CprPtr;

	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;

	//if (deflateInit2(&strm, level, Z_DEFLATED, windowBits | GZIP_ENCODING, 8, Z_DEFAULT_STRATEGY) != Z_OK) // with file header
	if (deflateInit(&strm, level) != Z_OK)
	{
		return NULL;
	}
	strm.next_in = DataPtr;
	strm.avail_in = lSizeData;


	unsigned int lBufferSizeCpr = lSizeData + (lSizeData/0x10) + 0x200;
	CprPtr = (unsigned char*)malloc(lBufferSizeCpr + CHUNK + sizeof(unsigned int));
	*(unsigned int *)CprPtr = lSizeData;
	lSizeCpr = sizeof(unsigned int);

	do {
		int have;
		strm.avail_out = CHUNK;
		strm.next_out = out;
		if (deflate(&strm, Z_FINISH) == Z_STREAM_ERROR)
		{
			deflateEnd(&strm);
			free(CprPtr);
			CprPtr = NULL;
		}
		have = CHUNK - strm.avail_out;

		memcpy(&CprPtr[lSizeCpr], out, have);
		lSizeCpr += have;
	} while (strm.avail_out == 0);

	if (deflateEnd(&strm) != Z_OK)
	{
		free(CprPtr);
		CprPtr = NULL;
	}

	//lSizeCpr = strm.total_out;
	return CprPtr;
}

__forceinline unsigned char* ZlibUncompress(unsigned char *CprPtr, unsigned int lSizeCpr, unsigned int lSizeData, unsigned int &lSizeUncpr)
{
	unsigned char out[CHUNK];
	int ret;
	unsigned have;
	z_stream strm;

	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	//if (inflateInit2(&strm, 16 + MAX_WBITS) != Z_OK)
	if (inflateInit(&strm) != Z_OK)
	{
		return NULL;
	}

	lSizeUncpr = 0;
	int lBufferSizeUncpr = lSizeData + (lSizeData/0x10) + 0x200;
	unsigned char* UncprPtr = (unsigned char*)malloc(lBufferSizeUncpr + CHUNK);

	strm.next_in = CprPtr;
	strm.avail_in = lSizeCpr;
	do {
		strm.avail_out = CHUNK;
		strm.next_out = out;
		ret = inflate(&strm, Z_NO_FLUSH);
		switch (ret) {
		case Z_NEED_DICT:
		case Z_DATA_ERROR:
		case Z_MEM_ERROR:
			inflateEnd(&strm);
			free(UncprPtr);
			UncprPtr = NULL;
		}
		have = CHUNK - strm.avail_out;
		memcpy(&UncprPtr[lSizeUncpr], out, have);
		lSizeUncpr += have;
	} while (strm.avail_out == 0);

	if (inflateEnd(&strm) != Z_OK) {
		free(UncprPtr);
		UncprPtr = NULL;
	}

	//lSizeUncpr=strm.total_out;

	return UncprPtr;
}
