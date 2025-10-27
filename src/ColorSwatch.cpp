#include "ColorSwatch.h"

#include <QColorDialog>

ColorSwatch::ColorSwatch(QWidget* parent /*= nullptr*/)
	: QWidget(parent)
	, m_Color(Qt::white)
{
	setFixedSize(32, 32);
	setCursor(Qt::PointingHandCursor);
}



void ColorSwatch::paintEvent(QPaintEvent*)
{
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);

	p.setPen(Qt::gray);
	p.setBrush(m_Color);
	p.drawRect(rect().adjusted(0, 0, -1, -1));
}

void ColorSwatch::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		ChooseColor();
	}
}

void ColorSwatch::ChooseColor()
{
	QColorDialog dialog(this);
	dialog.setOption(QColorDialog::DontUseNativeDialog, true);
	dialog.setWindowTitle("Select Color");
	dialog.setStyleSheet("QDialog { background-color: #dcdcdc; }");
	dialog.setCurrentColor(m_Color);

	if (dialog.exec() == QDialog::Accepted)
	{
		QColor selectedColor = dialog.selectedColor();
		if (selectedColor.isValid())
		{
			m_Color = selectedColor;
			emit ColorChanged(m_Color);
			update();
		}
	}
}

