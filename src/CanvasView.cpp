#include "CanvasView.h"

#include <QScrollBar>

#include "MainWindow.h"
#include "CellItem.h"

CanvasView::CanvasView(MainWindow* parent /*= nullptr*/)
	: QGraphicsView(parent)
	, m_ScaleFactor(0.8)
	, m_MinScale(0.2)
	, m_MaxScale(5.0)
	, m_SpaceHeld(false)
	, m_HoveredCell(nullptr)
{
	setRenderHint(QPainter::Antialiasing);
	setDragMode(NoDrag);
	setTransformationAnchor(AnchorUnderMouse);
	setResizeAnchor(AnchorUnderMouse);
	setFrameStyle(QFrame::NoFrame);
	UpdateZoom();
}


void CanvasView::wheelEvent(QWheelEvent* event)
{
	Zoom(event->angleDelta().y());
	event->accept();
}

void CanvasView::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton && m_SpaceHeld)
	{
		m_LastPos = event->pos();
		setCursor(Qt::ClosedHandCursor);
		event->accept();
		return;
	}
	HandleMouseEvent(event, false);

	QGraphicsView::mousePressEvent(event);

}

void CanvasView::mouseMoveEvent(QMouseEvent* event)
{
	if (m_SpaceHeld && !m_LastPos.isNull() && (event->buttons() & Qt::LeftButton))
	{
		QPoint delta = event->pos() - m_LastPos;
		horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
		verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
		m_LastPos = event->pos();
		event->accept();
		return;
	}
	HandleMouseHoverCell(event);
	if (event->buttons() & Qt::LeftButton)
	{
		HandleMouseEvent(event, true);
	}
	QGraphicsView::mouseMoveEvent(event);
}

void CanvasView::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_LastPos = QPoint();
		setCursor(Qt::ArrowCursor);
	}
	QGraphicsView::mouseReleaseEvent(event);
}

void CanvasView::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Space)
	{
		m_SpaceHeld = true;
		setCursor(Qt::OpenHandCursor);
		event->accept();
		return;
	}
	else if (event->key() == Qt::Key_Equal && event->modifiers() & Qt::KeyboardModifier::ControlModifier)
	{
		Zoom(1);
		event->accept();
		return;
	}
	else if (event->key() == Qt::Key_Minus && event->modifiers() & Qt::KeyboardModifier::ControlModifier)
	{
		Zoom(-1);
		event->accept();
		return;
	}


	QGraphicsView::keyPressEvent(event);
}

void CanvasView::keyReleaseEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Space)
	{
		m_SpaceHeld = false;
		setCursor(Qt::ArrowCursor);
		event->accept();
		return;
	}

	QGraphicsView::keyReleaseEvent(event);
}

void CanvasView::Zoom(int32_t delta)
{
	const double zoomFactor = 1.15;
	if (delta > 0)
		m_ScaleFactor *= zoomFactor;
	else
		m_ScaleFactor /= zoomFactor;

	m_ScaleFactor = std::clamp(m_ScaleFactor, m_MinScale, m_MaxScale);

	UpdateZoom();
}

void CanvasView::UpdateZoom()
{
	QTransform transform;
	transform.scale(m_ScaleFactor, m_ScaleFactor);
	setTransform(transform);
}

void CanvasView::HandleMouseEvent(QMouseEvent* event, bool bDrag)
{
	QPointF scenePos = mapToScene(event->pos());
	QList<QGraphicsItem*> itemsAtPos = scene()->items(scenePos, Qt::IntersectsItemShape, Qt::DescendingOrder, transform());
	for (QGraphicsItem* item : itemsAtPos)
	{
		if (CellItem* cell = dynamic_cast<CellItem*>(item))
		{
			bool bShift = event->modifiers() & Qt::ShiftModifier;
			if (bDrag)
			{
				emit CanvasDragged(cell->GetGridX(), cell->GetGridY(), bShift);
			}
			else
			{
				emit CanvasClicked(cell->GetGridX(), cell->GetGridY(), bShift);
			}
			break;
		}
	}
}

void CanvasView::HandleMouseHoverCell(QMouseEvent* event)
{
	QPointF scenePos = mapToScene(event->pos());
	QGraphicsItem* item = scene()->itemAt(scenePos, transform());
	if (CellItem* cell = dynamic_cast<CellItem*>(item))
	{
		if (cell != m_HoveredCell)
		{
			m_HoveredCell = cell;
			emit HoveredCellChanged(cell->GetGridX(), cell->GetGridY());
		}
	}
	else
	{
		m_HoveredCell = nullptr;
		emit HoveredCellGone();
	}
}

