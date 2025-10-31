#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QPixmap>
#include <QPushButton>

#include "Attribute.h"

class AttributePropertiesWidget : public QWidget
{
	Q_OBJECT
public:
	explicit AttributePropertiesWidget(QWidget* parent = nullptr);

signals:
	void AttributeSelected(EAttribute attr, bool bSelected);
	void TransparencySelected(bool bSelected);
	void AddToAll();
	void RemoveFromAll();

private:
	void StyleAttribButton(QPushButton* button);
	void StylePushButton(QPushButton* button);

};