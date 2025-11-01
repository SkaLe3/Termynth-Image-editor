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

public:
	void AddWindowMenuAction(QAction* action);

signals:
	void MinimizeRequested();
	void MaximizeRequested();
	void CloseRequested();

	void NewFile();
	void OpenFile();
	void SaveFile();
	void SaveFileAs();
	void Close();

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent*) override;

private:
	void SetupMenuBar(QMenuBar* menuBar);
	void StyleTitleBar();
	void StyleMenu(QMenu* menu);

	QPixmap CreateMinimizeIconPixmap();
	QPixmap CreateMaximizeIconPixmap();
	QPixmap CreateCloseIconPixmap();
	void CreateIconPixmap(QPixmap& iconPixmap, const QColor& color);

private:
	QMenuBar* m_MenuBar;
	QMenu* m_WindowMenu;
	QPoint m_DragPosition;
	bool m_IsDragging = false;
};

