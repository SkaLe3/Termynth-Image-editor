#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QChar>
#include <QSignalMapper>
#include <QMap>
#include <QString>


class CharacterSelectorWidget : public QWidget
{
	Q_OBJECT
public:
	explicit CharacterSelectorWidget(QWidget* parent = nullptr);
	
signals:
	void CharacterSelected(QChar ch);

private slots:
	void PopulateCharacters(const QString& blockName);

private:
	void StyleSelector();
	void StyleGridButton(QPushButton* button);

private:
	QBoxLayout* m_MainLayout;
	QComboBox* m_BlockSelector;
	QScrollArea* m_ScrollArea;
	QWidget* m_GridWidget;
	QGridLayout* m_GridLayout;

	QMap<QString, QPair<int32_t, int32_t>> m_Blocks;
};