#include "pch.h"
#include "Textures.h"


Textures::Textures(std::string pathName)
{
	std::fstream hFile(pathName, std::ios::in | std::ios::binary);

	if (!hFile.is_open())
	{
		throw std::invalid_argument("File not found.");
	}

	uint8_t header[18] = { 0 };
	std::vector<uint8_t> imageData;

	static uint8_t deCompressed[12] = { 0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
	static uint8_t isCompressed[12] = { 0x0, 0x0, 0xA, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

	hFile.read(reinterpret_cast<char*>(&header), sizeof(deCompressed));

	if (!memcmp(deCompressed, &header, sizeof(header)))
	{
		BitsPerPixel = header[16];
		width = header[13] * 256 + header[12];
		height = header[15] * 256 + header[14];
		size = ((width * BitsPerPixel + 31) / 32) * 4 * height;

		if ((BitsPerPixel != 24) && (BitsPerPixel != 32))
		{
			hFile.close();
			throw std::invalid_argument("Invalid file format. Required 24 or 32 bit image");

			imageData.resize(size);
			ImageCompressed = false;
			hFile.read(reinterpret_cast<char*>(imageData.data()), size);
		}
		else if (memcmp(isCompressed, &header, sizeof(isCompressed)))
		{
			BitsPerPixel = header[16];
			width = header[13] * 256 + header[12];
			height = header[15] * 256 + header[14];
			size = ((width * BitsPerPixel + 31) / 32) * 4 * height;

			if ((BitsPerPixel != 24) && (BitsPerPixel != 32))
			{
				hFile.close;
				std::invalid_argument("Invalid file format. Required 24 or 32 bit image");
			}

			PixelInfo Pixel = { 0 };
			int currentByte = 0;
			size_t currentPixel = 0;
			ImageCompressed = true;
			uint8_t chunkHeader = { 0 };
			int BytesPerPixel = (BitsPerPixel / 8);
			imageData.resize(width * height * sizeof(PixelInfo));

			do
			{
				hFile.read(reinterpret_cast<char*>(&chunkHeader), sizeof(chunkHeader));

				if (chunkHeader < 128)
				{
					++chunkHeader;
					for (int I = 0; I < chunkHeader; ++I, ++currentPixel)
					{
						hFile.read(reinterpret_cast<char*>(&Pixel), BytesPerPixel);

						imageData[currentByte++] = Pixel.B;
						imageData[currentByte++] = Pixel.G;
						imageData[currentByte++] = Pixel.R;
						if (BitsPerPixel > 24) imageData[currentByte++] = Pixel.A;
					}
				}
				else
				{
					chunkHeader -= 127;
					hFile.read(reinterpret_cast<char*>(&Pixel), BytesPerPixel);

					for (int I = 0; I < chunkHeader; ++I, ++currentPixel)
					{
						imageData[currentByte++] = Pixel.B;
						imageData[currentByte++] = Pixel.G;
						imageData[currentByte++] = Pixel.R;
						if (BitsPerPixel > 24) imageData[currentByte++] = Pixel.A;
					}
				}
			} while (currentPixel < (width * height));
		}
		else
		{
			hFile.close();
			throw std::invalid_argument("Invalid File Format. Required: 24 or 32 Bit TGA File.");
		}

		hFile.close();
		this->pixels = imageData;
	}



}

