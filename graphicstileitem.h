#ifndef GRAPHICSTILEITEM_H
#define GRAPHICSTILEITEM_H

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QGraphicsRectItem>
#include <QPen>
#include <QGraphicsScene>
using byte = unsigned char;


class GraphicsTileItem : public QGraphicsPixmapItem {
public:
    GraphicsTileItem() = default;


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
  //  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);


private:
    void setPixel(QPoint pos, QRgb color);

};

#endif // GRAPHICSTILEITEM_H
