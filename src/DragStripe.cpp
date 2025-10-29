#include "DragStripe.h"

#include <QDockWidget>
#include <QPainter>
#include <QApplication>
#include <QMainWindow>

#include "MainWindow.h"

DragStripe::DragStripe(QWidget* parent /*= nullptr*/)
	: QWidget(parent), m_Width(8), m_bIsHorizontal(true)
{
	setAutoFillBackground(true);
	SetWidth(m_Width);
	setStyleSheet("");
}

void DragStripe::SetWidth(uint32_t width)
{
	m_Width = width;
	if (m_bIsHorizontal)
	{
		setFixedHeight(m_Width);
		setMinimumWidth(0);
		setMaximumWidth(QWIDGETSIZE_MAX);
	}
	else
	{
		setFixedWidth(m_Width);
		setMinimumHeight(0);
		setMaximumHeight(QWIDGETSIZE_MAX);
	}
}

void DragStripe::SetVertical()
{
	m_bIsHorizontal = false;
	SetWidth(m_Width);
}

void DragStripe::SetHorizontal()
{
	m_bIsHorizontal = true;
	SetWidth(m_Width);
}

void DragStripe::paintEvent(QPaintEvent*)
{
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);

	p.setPen(Qt::NoPen); // no border
	p.setBrush(QColor("#FF0000")); // bright red for testing

	p.drawRect(rect()); // fill the full widget
}




QDockWidget* DragStripe::GetDockWidget()
{
	QWidget* w = parentWidget();
	while (w)
	{
		QDockWidget* dock = qobject_cast<QDockWidget*>(w);
		if (dock)
			return dock;
		w = w->parentWidget();
	}
	return nullptr;
}
