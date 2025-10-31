#pragma once

#include <QFrame>

class CharacterSelectorWidget;
class QHideEvent;

class CharacterPopup : public QFrame
{
	Q_OBJECT
public:
	explicit CharacterPopup(QWidget* parent = nullptr);

signals:
	void CharacterSelected(QChar ch);
	void Closed();

protected:
	void hideEvent(QHideEvent* event) override;

private:
	void StylePopup();

private:
	CharacterSelectorWidget* m_Selector;
};