#include "graphicstileitem.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QImage>
#include "palette.h"


void GraphicsTileItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	if (event->button() == Qt::MouseButton::LeftButton) {
		auto pos = event->scenePos();
		setPixel(pos.toPoint(), Palette::currentColor);
		event->accept();
	} else {
		event->ignore();
	}
}


void GraphicsTileItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	
	auto pos = event->scenePos();
	setPixel(pos.toPoint(), Palette::currentColor);

	
}

void GraphicsTileItem::setPixel(QPoint pos, QRgb color) {

	if (pos.x() < Tileset::tilePixmap->width() && pos.x() >= 0 &&
		pos.y() < Tileset::tilePixmap->height() && pos.y() >= 0) {

		QImage image = Tileset::tilePixmap->toImage();
		Tileset::tileColorId[pos.y()][pos.x()] = Palette::currentColorIndex;

		image.setPixelColor(pos.x(), pos.y(), color);
		Tileset::tilePixmap->convertFromImage(image);
		setPixmap(*Tileset::tilePixmap);
	}
}
