#pragma once

#include <QDockWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QLabel>
#include <QString>
#include <QPixmap>

class PanelDock : public QDockWidget
{
	Q_OBJECT
public:
	explicit PanelDock(const QString& title,bool bVertical = true,  QWidget* parent = nullptr);


protected:
	void HandleFloating(bool floating);
	void StyleHideCloseButton(QPushButton* button);
	void StyleCloseButton(QPushButton* button);

	virtual void StylePanel();

private:
	void CreateCloseIconPixmap();

protected:
	QWidget* m_Content;
	QWidget* m_Titlebar;
	QBoxLayout* m_PanelLayout;

	QPushButton* m_CloseButton;
	QPixmap m_CloseIconPixmap;

	bool m_bVertical;
};