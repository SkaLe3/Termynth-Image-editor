#include "AttributePropertiesWidget.h"

#include <QFrame>

AttributePropertiesWidget::AttributePropertiesWidget(QWidget* parent /*= nullptr*/)
	: QWidget(parent)
{
	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(6);

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

	QFrame* separator4 = new QFrame();
	separator4->setFrameShape(QFrame::VLine);
	separator4->setLineWidth(0);
	separator4->setStyleSheet("color: #333333;");

	QPushButton* transparencyButton = new QPushButton("Transparency", this);
	StyleAttribButton(transparencyButton);
	transparencyButton->setCheckable(true);

	QPushButton* applyToAll = new QPushButton("Add current set to all", this);
	StylePushButton(applyToAll);
	QPushButton* removeFromAll = new QPushButton("Remove current set from all", this);
	StylePushButton(removeFromAll);

	QPushButton* boldButton = new QPushButton("Bold", this);
	QPushButton* dimButton = new QPushButton("Dim", this);
	QPushButton* italicButton = new QPushButton("Italic", this);
	QPushButton* underlineButton = new QPushButton("Underline", this);
	QPushButton* blinkButton = new QPushButton("Blink", this);
	QPushButton* inverseButton = new QPushButton("Inverse", this);
	QPushButton* defaultFgButton = new QPushButton("DefaultFg", this);
	QPushButton* defaultBgButton = new QPushButton("DefaultBg", this);

	mainLayout->addWidget(separator4);
	mainLayout->addWidget(transparencyButton);

	mainLayout->addWidget(separator);
	for (auto& [btn, attribute] :
		{
			QPair{boldButton, EAttribute::Bold},
			QPair{dimButton, EAttribute::Dim},
			QPair{italicButton, EAttribute::Italic},
			QPair{underlineButton, EAttribute::Underline},
			QPair{blinkButton, EAttribute::Blink},
			QPair{inverseButton, EAttribute::Inverse},
			QPair{defaultFgButton, EAttribute::DefaultFg},
			QPair{defaultBgButton, EAttribute::DefaultBg}
		})
	{
		btn->setCheckable(true);
		StyleAttribButton(btn);
		mainLayout->addWidget(btn);
		connect(btn, &QPushButton::toggled, [=](bool checked)
			{
				emit AttributeSelected(attribute, checked);
			});
	}
	mainLayout->addWidget(separator2);
	mainLayout->addWidget(applyToAll);
	mainLayout->addWidget(removeFromAll);
	mainLayout->addWidget(separator3);
	mainLayout->addStretch(1);

	connect(applyToAll, &QPushButton::clicked, [this]() { emit AddToAll(); });
	connect(removeFromAll, &QPushButton::clicked, [this]() { emit RemoveFromAll(); });
	connect(transparencyButton, &QPushButton::toggled, [this](bool checked) {emit TransparencySelected(checked); });

}

void AttributePropertiesWidget::StyleAttribButton(QPushButton* button)
{
	QString style = R"(
    QPushButton {
        background-color: transparent;
        border: 1px solid transparent;
        border-radius: 4px;
        color: white;
		padding: 4px;
    }
    QPushButton:hover {
        background-color: #404040;
		border: 1px solid #636363;
    }
    QPushButton:checked {
        background-color: #333333;
		border: 1px solid#636363;
    }
    QPushButton:pressed {
        background-color: #333333;
		border: 1px solid #636363;
    }
    )";
	button->setStyleSheet(style);
	button->setFixedHeight(32);
}

void AttributePropertiesWidget::StylePushButton(QPushButton* button)
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
