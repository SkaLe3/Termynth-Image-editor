#pragma once


#include <QDockWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedWidget>

#include "PanelDock.h"
#include "Tool.h"
#include "Attribute.h"

class BrushPropertiesWidget;
class AttributePropertiesWidget;
class PaintPropertiesWidget;

class ToolPropertiesPanelDock : public PanelDock
{
	Q_OBJECT
public:
	explicit ToolPropertiesPanelDock(QWidget* parent = nullptr);

signals:
	void CharacterSelected(QChar ch);
	void AttributeSelected(EAttribute attr, bool bSelected);
	void TransparencySelected(bool bSelected);
	void ColorSelected(const QColor& color);
	void ApplyToAll(QChar ch, bool bInvert);

public slots:
	void OnToolSelected(ETool tool);

private:
	void SetupUI();

private:
	QHBoxLayout* m_PropertiesLayout;
	QStackedWidget* m_StackedWidget;
	BrushPropertiesWidget* m_BrushProperties;
	AttributePropertiesWidget* m_AttributeProperties;
	PaintPropertiesWidget* m_PaintProperties;
};