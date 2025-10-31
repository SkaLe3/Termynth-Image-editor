#include "CharacterMenuPopup.h"

#include <QHideEvent>
#include <QTimer>

#include "CharacterSelectorWidget.h"

CharacterPopup::CharacterPopup(QWidget* parent /*= nullptr*/)
	: QFrame(parent, Qt::Popup | Qt::FramelessWindowHint)
{
	setObjectName("CharacterPopup");

	QFrame* innerFrame = new QFrame(this);
	innerFrame->setObjectName("CharacterPopupInner");

	QVBoxLayout* innerFrameLayout = new QVBoxLayout(this);
	innerFrameLayout->setContentsMargins(0,0, 0, 0);
	innerFrameLayout->addWidget(innerFrame);

	QVBoxLayout* layout = new QVBoxLayout(innerFrame);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	m_Selector = new CharacterSelectorWidget(this);
	layout->addWidget(m_Selector);

	connect(m_Selector, &CharacterSelectorWidget::CharacterSelected,
		this, [this](QChar ch)
		{
			emit CharacterSelected(ch);
			hide();
		});
	StylePopup();
	QTimer::singleShot(0, this, [this]() {
		emit CharacterSelected('A');
		});


}

void CharacterPopup::hideEvent(QHideEvent* event)
{
	emit Closed();
	QWidget::hideEvent(event);
}

void CharacterPopup::StylePopup()
{

	setStyleSheet(R"(
	#CharacterPopup {
		border: 2px solid #333333;
	}
	#CharacterPopupInner {
		border: 1px solid #404040;
	}
	)"
	);
}
