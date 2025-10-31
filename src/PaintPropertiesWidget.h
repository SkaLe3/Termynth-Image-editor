#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QPixmap>
#include <QPushButton>
#include <QColor>

class PaintPropertiesWidget : public QWidget
{
	Q_OBJECT
public:
	explicit PaintPropertiesWidget(QWidget* parent = nullptr);

signals:
	void ColorSelected(const QColor& color);
	void ApplyToAll(bool bForeground);

private:
	void StyleColorButton(QPushButton* button, const QColor& color);
	void StylePushButton(QPushButton* button);
};