#pragma once

#include <QLabel>
#include <QMouseEvent>

class ClickableLabel : public QLabel
{
	Q_OBJECT
public:
	explicit ClickableLabel(const QString& text, QWidget* parent = nullptr);
	explicit ClickableLabel(QWidget* parent = nullptr);

signals:
	void clicked();

protected:
	void mousePressEvent(QMouseEvent* event) override;
};