#pragma once

#include <QDockWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QLabel>

#include "PanelDock.h"
#include "Tool.h"

class ColorSwatch;
class DockTitleBar;

class ToolPanelDock : public PanelDock
{
	Q_OBJECT

public:
	explicit ToolPanelDock(QWidget* parent = nullptr);

	QColor GetActiveColor() const;
	void ActivateTool(ETool tool);

signals:
	void ToolSelected(ETool tool);
	void ColorChanged(const QColor& color);
	

private:
	void SetupUI();
	void StyleToolButton(QPushButton* button);

private:
	QVBoxLayout* m_ToolLayout;
	QButtonGroup* m_ToolGroup;
	ColorSwatch* m_ColorSwatch;
};