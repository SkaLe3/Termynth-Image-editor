#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QPixmap>

class CharacterPopup;
class ClickableLabel;
class QPushButton;

class BrushPropertiesWidget : public QWidget
{
	Q_OBJECT
public:
	explicit BrushPropertiesWidget(QWidget* parent = nullptr);
		
signals:
	void CharacterSelected(QChar ch);
	void ApplyToAll(QChar ch);
	void SpaceToAll(QChar ch);

private:
	void CreateDropdownIconPixmap();
	void StyleDropdownButton(QPushButton* button);
	void StylePushButton(QPushButton* button);
private:
	CharacterPopup* m_CharacterPopup;
	ClickableLabel* m_ChosenChar;
	QPixmap m_DropdownIconPixmap;
};