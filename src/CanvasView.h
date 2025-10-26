#pragma once 
#include <QGraphicsView>
#include <QWheelEvent>
#include <QMouseEvent>

class MainWindow;

class CanvasView : public QGraphicsView
{
	Q_OBJECT
public:
	explicit CanvasView(MainWindow* parent = nullptr);
protected:
	void wheelEvent(QWheelEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;

	void Zoom(int32_t delta);
	void HandleMouseEvent(QMouseEvent* event, bool bDrag);

private:
	MainWindow* m_MainWindow;

	QPoint m_LastPos;
	double m_ScaleFactor;
	const double m_MinScale;
	const double m_MaxScale;
	bool m_SpaceHeld;
};