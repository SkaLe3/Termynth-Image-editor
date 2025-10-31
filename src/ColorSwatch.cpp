#include "ColorSwatch.h"

#include <QColorDialog>

ColorSwatch::ColorSwatch(QWidget* parent /*= nullptr*/)
	: QWidget(parent)
	, m_Color(Qt::red)
{
	setFixedSize(32, 32);
	setCursor(Qt::PointingHandCursor);
}



void ColorSwatch::SetColor(const QColor& color)
{
	UpdateColor(color);
}

void ColorSwatch::paintEvent(QPaintEvent*)
{
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);

	QColor outerGray("#333333");
	p.setPen(Qt::NoPen);
	p.setBrush(outerGray);
	p.drawRect(rect().adjusted(1, 1, -2, -2));

	p.setBrush(Qt::white);
	p.drawRect(rect().adjusted(2, 2, -3, -3));

	p.setBrush(m_Color);
	p.drawRect(rect().adjusted(3, 3, -4, -4));


}

void ColorSwatch::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		ChooseColor();
	}
}

void ColorSwatch::UpdateColor(const QColor& color)
{
	m_Color = color;
	emit ColorChanged(m_Color);
	update();
}

void ColorSwatch::ChooseColor()
{
	QColorDialog dialog(this);
	dialog.setOption(QColorDialog::DontUseNativeDialog, true);
	dialog.setWindowTitle("Select Color");
	//dialog.setStyleSheet("QDialog { background-color: #8c8c8c; }");
	dialog.setCurrentColor(m_Color);

	if (dialog.exec() == QDialog::Accepted)
	{
		QColor selectedColor = dialog.selectedColor();
		if (selectedColor.isValid())
		{
			UpdateColor(selectedColor);
		}
	}
}

