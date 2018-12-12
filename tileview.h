#ifndef TILEVIEW_H
#define TILEVIEW_H

#include <QObject>
#include <QEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "Bitmap.h"


class TileView : public QObject {
    Q_OBJECT
public:
    explicit TileView(QObject *parent = nullptr);
    ~TileView() = default;

	void setGraphicsView(QGraphicsView* v);
    QPixmap *px;
    QGraphicsPixmapItem *pxitem;
    Bitmap *bitmap;
protected:
    bool eventFilter(QObject *obj, QEvent *ev);



private:

    static const constexpr qreal scaleSize = 1.5;

    QGraphicsView *m_view = nullptr;

    bool m_canZoom = false;
};

#endif // TILEVIEW_H
