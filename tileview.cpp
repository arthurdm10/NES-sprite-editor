#include "tileview.h"
#include <QDebug>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QtMath>
#include <QImage>

#include "palette.h"


TileView::TileView(QObject *parent) : QObject(parent) {}


void TileView::setGraphicsView(QGraphicsView * v) { m_view = v; }

bool TileView::eventFilter(QObject *obj, QEvent *ev){
	if (obj == m_view->viewport() && ev->type() == QEvent::Wheel) {
		if (m_canZoom) {
			auto wheelEvent = static_cast<QWheelEvent*>(ev);
			auto delta = wheelEvent->delta();
			
			double scale = qPow(1.0015, wheelEvent->angleDelta().y());

			m_view->scale(scale, scale);
			return true;
		}
	} else if(ev->type() == QEvent::KeyPress){
        auto keyEvent = static_cast<QKeyEvent*>(ev);
        if(keyEvent->key() == Qt::Key::Key_Control){
            m_canZoom = true;
			return true;
		}
    }else if(ev->type() == QEvent::KeyRelease){
        auto keyEvent = static_cast<QKeyEvent*>(ev);
        if(keyEvent->key() == Qt::Key::Key_Control){
            m_canZoom = false;
			return true;
		}
    }

    return false;
}
