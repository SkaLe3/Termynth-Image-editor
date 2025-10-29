#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>

#include <cstdint>

class QMainWindow;
class QDockWidget;

class DragStripe : public QWidget
{
	Q_OBJECT
public:
	explicit DragStripe(QWidget* parent = nullptr);

	void SetWidth(uint32_t width);
	void SetVertical();
	void SetHorizontal();
protected:
	void paintEvent(QPaintEvent*) override;


private:
	QDockWidget* GetDockWidget();

private:
	QMainWindow* m_MainWindow;
	bool m_bDragging = false;
	QPoint m_DragStartPos;
	uint32_t m_Width;
	bool m_bIsHorizontal;
};