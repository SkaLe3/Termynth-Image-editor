#include "BrushPropertiesWidget.h"

#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QTimer>

#include "CharacterMenuPopup.h"
#include "ClickableLabel.h"
#include "InstantToggleButton.h"

BrushPropertiesWidget::BrushPropertiesWidget(QWidget* parent /*= nullptr*/)
	: QWidget(parent), m_CharacterPopup(nullptr)
{
	CreateDropdownIconPixmap();

	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);

	InstantToggleButton* chooseCharDropdown = new InstantToggleButton(this);
	chooseCharDropdown->setCheckable(true);
	StyleDropdownButton(chooseCharDropdown);

	m_ChosenChar = new ClickableLabel("-");
	m_ChosenChar->setFixedSize(32, 32);
	m_ChosenChar->setAlignment(Qt::AlignCenter);
	m_ChosenChar->setStyleSheet("color: white; font-size: 24px; font-family: 'Consolas'; font-weight: bold;");


	QFrame* separator = new QFrame();
	separator->setFrameShape(QFrame::VLine);
	separator->setLineWidth(0);
	separator->setStyleSheet("color: #333333;");

	QFrame* separator2 = new QFrame();
	separator2->setFrameShape(QFrame::VLine);
	separator2->setLineWidth(0);
	separator2->setStyleSheet("color: #333333;");

	QFrame* separator3 = new QFrame();
	separator3->setFrameShape(QFrame::VLine);
	separator3->setLineWidth(0);
	separator3->setStyleSheet("color: #333333;");

	QPushButton* applyToAllButton = new QPushButton("Apply to all", this);
	StylePushButton(applyToAllButton);

	QPushButton* clearAllButton = new QPushButton("Space to all", this);
	StylePushButton(clearAllButton);

	mainLayout->addSpacing(10);
	mainLayout->addWidget(separator);
	mainLayout->addSpacing(6);
	mainLayout->addWidget(m_ChosenChar);
	mainLayout->addWidget(chooseCharDropdown);
	mainLayout->addSpacing(6);
	mainLayout->addWidget(separator2);
	mainLayout->addSpacing(6);
	mainLayout->addWidget(applyToAllButton);
	mainLayout->addSpacing(6);
	mainLayout->addWidget(clearAllButton);
	mainLayout->addSpacing(6);
	mainLayout->addWidget(separator3);
	
	mainLayout->addStretch(1);

	m_CharacterPopup = new CharacterPopup(this);
	m_CharacterPopup->resize(420, 400);
	chooseCharDropdown->SetToggleObject(m_CharacterPopup);

	// Update selected character
	connect(m_CharacterPopup, &CharacterPopup::CharacterSelected, [this](QChar ch)
		{
			m_ChosenChar->setText(QString(ch));
			emit CharacterSelected(ch);
		});

	// Toggle popup on button click
	connect(chooseCharDropdown, &QPushButton::toggled, [chooseCharDropdown, this](bool checked)
		{
			if (checked)
			{
				QPoint globalPos = chooseCharDropdown->mapToGlobal(QPoint(0, chooseCharDropdown->height()));
				m_CharacterPopup->move(globalPos);
				m_CharacterPopup->show();
			}
			else
			{
				m_CharacterPopup->hide();
			}
		});

	// When popup closes, uncheck the button
	connect(m_CharacterPopup, &CharacterPopup::Closed, [this, chooseCharDropdown]()
		{
			// Don't uncheck if clicked on chosen char lable or dropdown button, it will handle closing by itself
			bool bIsOverLabel = m_ChosenChar->rect().contains(m_ChosenChar->mapFromGlobal(QCursor::pos()));
			bool bIsOverButton = chooseCharDropdown->rect().contains(chooseCharDropdown->mapFromGlobal(QCursor::pos()));
			if (!bIsOverButton && !bIsOverLabel)
			{
				chooseCharDropdown->CloseToggleObject();
			}
		});

	// When lable clicked - toggle button
	connect(m_ChosenChar, &ClickableLabel::clicked, [this, chooseCharDropdown]()
		{
			chooseCharDropdown->HandleToggle();
		});

	connect(applyToAllButton, &QPushButton::clicked, [this]()
		{
			emit ApplyToAll(m_ChosenChar->text().at(0));
		});

	connect(clearAllButton, &QPushButton::clicked, [this]()
		{
			emit ApplyToAll(QChar(' '));
		});
}

void BrushPropertiesWidget::CreateDropdownIconPixmap()
{
	QPixmap pixmap(":/icons/dropdown.png");
	m_DropdownIconPixmap = QPixmap(pixmap.size());
	m_DropdownIconPixmap.fill(Qt::transparent);

	QPainter painter(&m_DropdownIconPixmap);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.drawPixmap(0, 0, pixmap);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.fillRect(m_DropdownIconPixmap.rect(), QColor("#ababab"));
	painter.end();
}

void BrushPropertiesWidget::StyleDropdownButton(QPushButton* button)
{
	button->setFixedSize(12, 32);
	button->setIcon(m_DropdownIconPixmap);
	button->setIconSize(button->size());

	button->setStyleSheet(R"(
	QPushButton {
        background-color: transparent;
		border: 0px solid transparent;
        color: white;
		outline: none;
    }
    QPushButton:hover {
        background-color: transparent;
		border: 0px solid transparent;
    }
	QPushButton:checked {
        background-color: #333333;
		border: 1px solid #636363;
		border-radius: 1px;
    }
	QPushButton:pressed {
        background-color: #333333;
		border: 1px solid #636363;
		border-radius: 0px;
    }
	)");

}

void BrushPropertiesWidget::StylePushButton(QPushButton* button)
{
	button->setFixedHeight(32);
	button->setStyleSheet(R"(
	QPushButton {
        background-color: transparent;
		border: 0px solid transparent;
        color: white;
		outline: none;
		padding: 4px;
		border-radius: 2px;
    }
    QPushButton:hover {
        background-color: #404040;
		border: 1px solid #636363;
    }
	QPushButton:checked {
        background-color: #333333;
		border: 1px solid #636363;
    }
	QPushButton:pressed {
        background-color: #333333;
		border: 1px solid #636363;
		
    }
	)");
}
