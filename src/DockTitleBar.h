#pragma once

#include <QWidget>
#include <QPoint>

class DockTitleBar : public QWidget
{
	Q_OBJECT
public:
	explicit DockTitleBar(QWidget* parent = nullptr);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

signals:
	void dragStart(const QPoint&);
	void dragMove(const QPoint&);
};