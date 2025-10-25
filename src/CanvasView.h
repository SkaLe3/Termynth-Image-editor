#pragma once 
#include <QGraphicsView>
#include <QWheelEvent>
#include <QMouseEvent>


class CanvasView : public QGraphicsView
{
	Q_OBJECT
public:
	explicit CanvasView(QWidget* parent = nullptr)
		: QGraphicsView(parent)
	{
		setRenderHint(QPainter::Antialiasing);
		setDragMode(NoDrag);
		setTransformationAnchor(AnchorUnderMouse);
		setResizeAnchor(AnchorUnderMouse);
		setFrameStyle(QFrame::NoFrame);
	}
protected:
	void wheelEvent(QWheelEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

private:
	QPoint m_LastPos;
};