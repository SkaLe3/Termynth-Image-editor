#pragma once


#include <QDockWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedWidget>

#include "PanelDock.h"
#include "Tool.h"

class ToolPropertiesPanelDock : public PanelDock
{
	Q_OBJECT
public:
	explicit ToolPropertiesPanelDock(QWidget* parent = nullptr);

public slots:
	void OnToolSelected(ETool tool);

private:
	void SetupUI();

private:
	QHBoxLayout* m_PropertiesLayout;
	QStackedWidget* m_StackedWidget;
};