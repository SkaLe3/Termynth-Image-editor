#include "ClickableLabel.h"



ClickableLabel::ClickableLabel(QWidget* parent /*= nullptr*/)
	: QLabel(parent)
{

}

ClickableLabel::ClickableLabel(const QString& text, QWidget* parent /*= nullptr*/)
	: QLabel(text, parent)
{

}

void ClickableLabel::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		emit clicked();
	}
	QLabel::mousePressEvent(event);
}
