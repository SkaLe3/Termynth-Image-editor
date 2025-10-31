#include "ToolPropertiesPanelDock.h"

#include <QPainter>

#include "BrushPropertiesWidget.h"
#include "AttributePropertiesWidget.h"
#include "PaintPropertiesWidget.h"

ToolPropertiesPanelDock::ToolPropertiesPanelDock(QWidget* parent /*= nullptr*/)
	: PanelDock("ToolProperties",false, parent)
{
	SetupUI();
	setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetVerticalTitleBar | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
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
	m_PropertiesLayout->setSpacing(0);
	content->setLayout(m_PropertiesLayout);


	m_StackedWidget = new QStackedWidget(content);
	m_PropertiesLayout->addWidget(m_StackedWidget);
	m_PropertiesLayout->addStretch();
	

	m_BrushProperties = new BrushPropertiesWidget(content);
	m_AttributeProperties = new AttributePropertiesWidget(content);
	m_PaintProperties = new PaintPropertiesWidget(content);
	m_StackedWidget->addWidget(new QWidget());
	m_StackedWidget->addWidget(m_BrushProperties);
	m_StackedWidget->addWidget(m_PaintProperties);
	m_StackedWidget->addWidget(m_AttributeProperties);

	m_StackedWidget->setCurrentIndex(0);

	connect(m_BrushProperties, &BrushPropertiesWidget::CharacterSelected, [this](QChar ch)
		{
			emit CharacterSelected(ch);
		});

	connect(m_BrushProperties, &BrushPropertiesWidget::ApplyToAll, [this](QChar ch)
		{
			emit ApplyToAll(ch, false);
		});
	connect(m_AttributeProperties, &AttributePropertiesWidget::AttributeSelected, [this](EAttribute attr, bool bSelected)
		{
			emit AttributeSelected(attr, bSelected);
		});
	connect(m_AttributeProperties, &AttributePropertiesWidget::AddToAll, [this]()
		{
			emit ApplyToAll(' ', false);
		});
	connect(m_AttributeProperties, &AttributePropertiesWidget::RemoveFromAll, [this]()
		{
			emit ApplyToAll(' ', true);
		});
	connect(m_PaintProperties, &PaintPropertiesWidget::ColorSelected, [this](const QColor& color)
		{
			emit ColorSelected(color);
		});
	connect(m_PaintProperties, &PaintPropertiesWidget::ApplyToAll, [this](bool bForeground)
		{
			emit ApplyToAll(' ', !bForeground);
		});
	connect(m_AttributeProperties, &AttributePropertiesWidget::TransparencySelected, [this](bool bSelected)
		{
			emit TransparencySelected(bSelected);
		});
}



