#pragma once

#include <QWidget>
#include <QColor>
#include <QPainter>
#include <QColorDialog>
#include <QMouseEvent>



class ColorSwatch : public QWidget
{
	Q_OBJECT
public:
	explicit ColorSwatch(QWidget* parent = nullptr);

	QColor Color() { return m_Color; }
	void SetColor(const QColor& color);
	
signals:
	void ColorChanged(const QColor& color);

protected:
	void paintEvent(QPaintEvent*) override;
	void mousePressEvent(QMouseEvent* event) override;

private:
	void UpdateColor(const QColor& color);
	void ChooseColor();

private:
	QColor m_Color;
};
