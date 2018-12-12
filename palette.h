#ifndef PALETTE_H
#define PALETTE_H

#include <vector>
#include <array>
#include <string>
#include <fstream>
#include <algorithm>
#include <functional>


#include "Bitmap.h"
#include "Tileset.h"

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneEvent>
#include <QGraphicsPixmapItem>
#include <QPushButton>
#include <QDebug>
#include <QEvent>
#include <QGraphicsScene>
#include <QLabel>

using byte = unsigned char;
using uint = unsigned int;

struct Palette {

    std::array<byte, 0x04> colors;

	
	Palette(std::initializer_list<byte> colorsIndex) {
		std::copy_n(colorsIndex.begin(), 4, colors.begin());
	}

	Palette() { colors.fill(0x00); }


    static std::vector<Palette> palettes;


    static Palette *currentPalette;
    static std::vector<Bitmap::Pixel> allColors;

    
    static uint currentColor;		//current color RGB value
    
	static byte currentColorIndex;

    static void updateTilesPalette(){
		if (Tileset::tilePixmap == nullptr) {
			return;
		}

        QImage image = Tileset::tilePixmap->toImage();
        for(int x = 0; x < image.width(); ++x){
            for (int y = 0; y < image.height(); ++y) {
                auto colorIndex = Tileset::tileColorId[y][x];
                auto colorId = Palette::currentPalette->colors[colorIndex];
                image.setPixelColor(x, y, Palette::allColors[colorId].val());

            }
        }

        Tileset::tilePixmap->convertFromImage(image);
        Tileset::tileItem->setPixmap(*Tileset::tilePixmap);
    }

    static void loadPallete(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        allColors.resize(256);

        if(file.is_open()){
            byte colorId = 0x00;
            Bitmap::Pixel pixel;


            while(!file.eof()){
                file.read(reinterpret_cast<char*>(&pixel), 0x03);

                std::swap(pixel.b, pixel.r);

                allColors[colorId++] = pixel;
            }
        }
    }
};



class GraphicsColorItem : public QGraphicsRectItem {
public:

    GraphicsColorItem() = default;
    ~GraphicsColorItem() = default;


    static QPushButton* selectedButton;
   
    static QLabel* colorRgbLabel;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event){
        if(event->button() == Qt::MouseButton::LeftButton){
            auto colorIndex = Palette::currentColorIndex;
            Palette::currentPalette->colors[colorIndex] = *(static_cast<byte*>(this->data(0xcc).data()));
            selectedButton->setProperty("colorIndex", colorIndex);

            auto colorId = Palette::currentPalette->colors[colorIndex];
            auto color = Palette::allColors[colorId];
            auto style = "background-color: rgb(" + std::to_string(color.r) + ", "
                                                  + std::to_string(color.g) + ", "
                                                  + std::to_string(color.b) + ");";
            
			
			selectedButton->setStyleSheet(QString::fromStdString(style));
			selectedButton->setToolTip(QString::number(colorId, 16));

			Palette::currentColor = color.val();
            Palette::updateTilesPalette();
            colorRgbLabel->setText(QString::fromStdString(color.toString()).toUpper());
			
			event->accept();
        }
    }
};



#endif // PALETTE_H


