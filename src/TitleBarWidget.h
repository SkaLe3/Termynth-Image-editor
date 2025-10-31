#pragma once

#include <QWidget>
#include <QMenuBar>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QPixmap>


class TitleBarWidget : public QWidget
{
	Q_OBJECT
public:
	explicit TitleBarWidget(QWidget* parent = nullptr);

signals:
	void MinimizeRequested();
	void MaximizeRequested();
	void CloseRequested();

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent*) override;

private:
	void StyleTitleBar();

	QPixmap CreateMinimizeIconPixmap();
	QPixmap CreateMaximizeIconPixmap();
	QPixmap CreateCloseIconPixmap();
	void CreateIconPixmap(QPixmap& iconPixmap, const QColor& color);
	
public:
	QMenuBar* MenuBar;

private:
	QPoint m_DragPosition;
	bool m_IsDragging = false;
};

