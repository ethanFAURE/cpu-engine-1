#include "stdafx.h"

TEXTURE::TEXTURE()
{
	rgba = nullptr;
	Close();
}

TEXTURE::~TEXTURE()
{
	Close();
}

bool TEXTURE::Load(const char* path)
{
	Close();

	FILE* file;
	if ( fopen_s(&file, path, "rb") )
		return false;

	fseek(file, 0, SEEK_END);
	int filesize = ftell(file);
	fseek(file, 0, SEEK_SET);
	byte* data = new byte[filesize];
	fread(data, 1, filesize, file);
	fclose(file);

	BITMAPFILEHEADER bfh;
	memcpy(&bfh, data, sizeof(BITMAPFILEHEADER));

	BITMAPINFOHEADER bih;
	memcpy(&bih, data+sizeof(BITMAPFILEHEADER), sizeof(BITMAPINFOHEADER));

	width = bih.biWidth;
	height = std::abs(bih.biHeight);
	count = width * height;
	size = count * 4;
	rgba = new byte[size];

	byte* src = data + bfh.bfOffBits;
	int bytesPerPixel = (bih.biBitCount == 32) ? 4 : 3;
	int stride = ((((width * bih.biBitCount) + 31) & ~31) >> 3);
	for ( int y=0 ; y<height ; y++ )
	{
		byte* trg;
		if ( bih.biHeight<0 )
			trg = rgba + y * width * 4;
		else
			trg = rgba + (height - 1 - y) * width * 4;

		byte* cur = src;
		for ( int x=0 ; x<width ; x++ )
		{
			if ( bih.biBitCount==32 )
			{
				trg[0] = cur[1];
				trg[1] = cur[2];
				trg[2] = cur[3];
				trg[3] = cur[0];
			}
			else
			{
				if ( cur[0]==0 && cur[1]==0 && cur[2]==0 )
				{
					trg[0] = 0;
					trg[1] = 0;
					trg[2] = 0;
					trg[3] = 0;
				}
				else
				{
					trg[0] = cur[0];
					trg[1] = cur[1];
					trg[2] = cur[2];
					trg[3] = 255;
				}
			}
			cur += bytesPerPixel;
			trg += 4;
		}

		src += stride;
	}
	delete [] data;

	return true;
}

void TEXTURE::Close()
{
	delete [] rgba;
	rgba = nullptr;
	width = 0;
	height = 0;
	count = 0;
	size = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SPRITE::SPRITE()
{
	pTexture = nullptr;
	x = 0;
	y = 0;
	z = 0;
	index = -1;
	sortedIndex = -1;
	dead = false;
}
