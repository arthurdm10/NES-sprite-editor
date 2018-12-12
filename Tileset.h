#pragma once

#include <vector>

#include "graphicstileitem.h"

#include <QPixmap>

using byte = unsigned char;


//current tileset
namespace Tileset {
	constexpr uint tilesetWidth = 16 * 8;	//16 tiles per row * 8 bytes per tile
	constexpr uint tilesetHeight = 256;


	extern std::vector<std::vector<byte>>	tileColorId;
	
	extern std::vector<byte>				tileData;
    
	extern GraphicsTileItem*				tileItem;
	
	extern QPixmap*							tilePixmap;


	static std::vector<byte> getBits(unsigned int d) {
		std::vector<byte> bits;
		for (int i = 0; i < 8; i++) {
			bits.push_back((d >> i) & 0x01);
		}
		return bits;
	}

	template<class IteratorBegin>
		void parseChrData(IteratorBegin dataBegin) {
			/*
			https://n3s.io/index.php?title=How_It_Works
			each tile is 8x8

			1 byte = 1 row
			1 bit  = 1 pixel

			0x00 -> 0x07 = plane 1
			0x08 -> 0x0f = plane 2
			*/

			Bitmap::PixelMatrix pixels(Tileset::tilesetHeight, std::vector<Bitmap::Pixel>(Tileset::tilesetWidth));

			for (uint bmpRow = 0; bmpRow < tilesetHeight; bmpRow += 8) {
				uint col = 0x00;

				for (uint totalTiles = 0x00; totalTiles < 16; ++totalTiles) {
					for (uint i = 0; i < 8; i++) {
						byte v1 = *dataBegin;           //byte from plane 1
						byte v2 = *(dataBegin + 8);     //byte from plane 2

						const auto plane1 = getBits(v1);      //bits of the byte from plane 1
						const auto plane2 = getBits(v2);      //bits of the byte from plane 2
						std::vector<byte> pixelColorId(8);

						std::transform(plane1.begin(),
							plane1.end(),
							plane2.begin(),
							pixelColorId.begin(),
							[](byte b1, byte b2) {
							b1 |= b2 << 1; return b1;
						});


						std::reverse(pixelColorId.begin(), pixelColorId.end());
						uint tileCol = 0x00;

						for (auto pxColor : pixelColorId) {
							const auto colorId = Palette::currentPalette->colors[pxColor];
							const auto y = bmpRow + i;
							const auto x = col + tileCol++;

							pixels[y][x] = Palette::allColors[colorId];
							Tileset::tileColorId[y][x] = pxColor;
						}

						++dataBegin;
					}

					dataBegin += 8;
					col += 8;
				}
			}

			Bitmap tiles(std::move(pixels));
			Tileset::tilePixmap = new QPixmap();

			if (Tileset::tilePixmap->loadFromData(tiles.data().data(),
				tiles.fileSize())) {

				Tileset::tileItem->setPixmap(*Tileset::tilePixmap);
			} else {
				qDebug() << "Failed to load from data";
			}

	}

};

