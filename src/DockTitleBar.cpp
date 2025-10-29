#include "DockTitleBar.h"

#include <QMouseEvent>


DockTitleBar::DockTitleBar(QWidget* parent /*= nullptr*/)
	: QWidget(parent)
{

}

void DockTitleBar::mousePressEvent(QMouseEvent* event)
{
	QWidget::mousePressEvent(event);
	emit dragStart(event->pos());
}

void DockTitleBar::mouseMoveEvent(QMouseEvent* event)
{
	QWidget::mousePressEvent(event);
	emit dragMove(event->pos());
}