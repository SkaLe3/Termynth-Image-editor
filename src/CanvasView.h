#pragma once 
#include <QGraphicsView>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QPair>

class MainWindow;
class CellItem;

class CanvasView : public QGraphicsView
{
	Q_OBJECT
public:
	explicit CanvasView(MainWindow* parent = nullptr);
	void Clear();

signals:
	void HoveredCellChanged(int32_t x, int32_t y);
	void HoveredCellGone();
	void CanvasDragged(int32_t x, int32_t y, bool bShift);
	void CanvasClicked(int32_t x, int32_t y, bool bShift);

protected:
	void wheelEvent(QWheelEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;

	void Zoom(int32_t delta);
	void UpdateZoom();
	void HandleMouseEvent(QMouseEvent* event, bool bDrag);
	void HandleMouseHoverCell(QMouseEvent* event);

private:

	QPoint m_LastPos;
	double m_ScaleFactor;
	const double m_MinScale;
	const double m_MaxScale;
	bool m_SpaceHeld;
	CellItem* m_HoveredCell;
};