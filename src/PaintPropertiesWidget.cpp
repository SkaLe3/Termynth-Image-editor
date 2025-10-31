#include "PaintPropertiesWidget.h"

#include <QFrame>
#include <QGridLayout>

PaintPropertiesWidget::PaintPropertiesWidget(QWidget* parent /*= nullptr*/)
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

	QPushButton* applyFgToAll = new QPushButton("Apply FG to all", this);
	StylePushButton(applyFgToAll);
	QPushButton* applyBgToAll = new QPushButton("Apply BG to all", this);
	StylePushButton(applyBgToAll);

	QWidget* colorsWidget = new QWidget(this);
	QGridLayout* colorsLayout = new QGridLayout(colorsWidget);
	colorsLayout->setContentsMargins(0, 0, 0, 0);
	colorsLayout->setSpacing(4);
	colorsWidget->setLayout(colorsLayout);
	
	mainLayout->addWidget(separator);
	mainLayout->addWidget(colorsWidget);
	mainLayout->addWidget(separator2);
	mainLayout->addWidget(applyFgToAll);
	mainLayout->addWidget(applyBgToAll);
	mainLayout->addWidget(separator3);
	mainLayout->addStretch(1);


	for (auto& [color, pos] :
		{
			QPair{QColor("#000000"),QPair{0, 0}},
			QPair{QColor("#800000"),QPair{1, 0}},
			QPair{QColor("#008000"),QPair{0, 1}},
			QPair{QColor("#808000"),QPair{1, 1}},
			QPair{QColor("#000080"),QPair{0, 2}},
			QPair{QColor("#800080"),QPair{1, 2}},
			QPair{QColor("#008080"),QPair{0, 3}},
			QPair{QColor("#C0C0C0"),QPair{1, 3}},
			QPair{QColor("#808080"),QPair{0, 4}},
			QPair{QColor("#FF0000"),QPair{1, 4}},
			QPair{QColor("#00FF00"),QPair{0, 5}},
			QPair{QColor("#FFFF00"),QPair{1, 5}},
			QPair{QColor("#0000FF"),QPair{0, 6}},
			QPair{QColor("#FF00FF"),QPair{1, 6}},
			QPair{QColor("#00FFFF"),QPair{0, 7}},
			QPair{QColor("#FFFFFF"),QPair{1, 7}},
		})
	{
		QPushButton* colorButton = new QPushButton(this);
		StyleColorButton(colorButton, color);
		colorsLayout->addWidget(colorButton, pos.first, pos.second);
		connect(colorButton, &QPushButton::clicked, [this, color]()
			{
				emit ColorSelected(color);
			});
	}

	connect(applyFgToAll, &QPushButton::clicked, [this]() {emit ApplyToAll(true); });
	connect(applyBgToAll, &QPushButton::clicked, [this]() {emit ApplyToAll(false); });

}

void PaintPropertiesWidget::StyleColorButton(QPushButton* button, const QColor& color)
{
	button->setFixedSize(14, 14);
	QString colorString = color.name();
	QString style = QString(R"(
        QPushButton {
            background-color: %1;
            border: 1px solid #636363;
        }
    )").arg(colorString);

	button->setStyleSheet(style);

}

void PaintPropertiesWidget::StylePushButton(QPushButton* button)
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
