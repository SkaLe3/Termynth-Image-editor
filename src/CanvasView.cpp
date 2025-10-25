#include "CanvasView.h"

#include <QScrollBar>

void CanvasView::wheelEvent(QWheelEvent* event)
{
	const double scaleFactor = 1.15;
	if (event->angleDelta().y() > 0)
		scale(scaleFactor, scaleFactor);
	else
		scale(1.0 / scaleFactor, 1.0 / scaleFactor);
}

void CanvasView::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton && event->modifiers() & Qt::KeyboardModifier::ControlModifier) 
	{
		m_LastPos = event->pos();
		setCursor(Qt::ClosedHandCursor);
	}
	else if (event->button() == Qt::MiddleButton || (event->modifiers() & Qt::KeyboardModifier::ShiftModifier)) 
	{
		m_LastPos = event->pos();
		setCursor(Qt::ClosedHandCursor);
	}
	else 
	{
		QGraphicsView::mousePressEvent(event);
	}
}

void CanvasView::mouseMoveEvent(QMouseEvent* event)
{
	if (!m_LastPos.isNull() && (event->buttons() & Qt::LeftButton)) 
	{
		auto delta = event->pos() - m_LastPos;
		m_LastPos = event->pos();
		horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
		verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
	}
	else 
	{
		QGraphicsView::mouseMoveEvent(event);
	}
}

void CanvasView::mouseReleaseEvent(QMouseEvent* event)
{
	m_LastPos = QPoint();
	setCursor(Qt::ArrowCursor);
	QGraphicsView::mouseReleaseEvent(event);
}
