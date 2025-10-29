#include "ToolPropertiesPanelDock.h"

#include <QPainter>

ToolPropertiesPanelDock::ToolPropertiesPanelDock(QWidget* parent /*= nullptr*/)
	: PanelDock("Tool Properties",false, parent)
{
	SetupUI();
	setFeatures(QDockWidget::DockWidgetVerticalTitleBar | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	setAllowedAreas(Qt::TopDockWidgetArea);
}

void ToolPropertiesPanelDock::OnToolSelected(ETool tool)
{
	int32_t toolIndex = static_cast<int32_t>(tool);
	m_StackedWidget->setCurrentIndex(toolIndex);
}

void ToolPropertiesPanelDock::SetupUI()
{

	QWidget* content = new QWidget(m_Content);
	m_PanelLayout->addWidget(content);
	m_PropertiesLayout = new QHBoxLayout(content);
	m_PropertiesLayout->setContentsMargins(0, 0, 0, 0);
	m_PropertiesLayout->setSpacing(10);
	content->setLayout(m_PropertiesLayout);

	m_StackedWidget = new QStackedWidget(content);
	m_PropertiesLayout->addWidget(m_StackedWidget);
	m_PropertiesLayout->addStretch();
	


	QWidget* selectProperties = new QWidget();
	QPushButton* BrushButton = new QPushButton("Brush");
	m_StackedWidget->addWidget(selectProperties);
	m_StackedWidget->addWidget(BrushButton);

	m_StackedWidget->setCurrentIndex(0);
}



