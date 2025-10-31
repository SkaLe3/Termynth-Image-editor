#include "CharacterSelectorWidget.h"

#include <QListView>
#include <QStyleFactory>

CharacterSelectorWidget::CharacterSelectorWidget(QWidget* parent /*= nullptr*/)
	: QWidget(parent)
{
	m_MainLayout = new QVBoxLayout(this);
	m_MainLayout->setContentsMargins(6, 6, 6, 6);
	m_MainLayout->setSpacing(4);

	m_BlockSelector = new QComboBox();
	m_MainLayout->addWidget(new QLabel("Unicode Block:"));
	m_MainLayout->addWidget(m_BlockSelector);

	m_Blocks = {
		{ "Basic Latin", {0x0020, 0x007F} },
		{ "Latin-1 Supplement", {0x00A0, 0x00FF} },
		{ "Cyrillic", {0x0400, 0x04FF} },
		{ "Greek and Coptic", {0x0370, 0x03FF} },
		{ "Box Drawing", {0x2500, 0x257F} },
		{ "Block Elements", {0x2580, 0x259F} },
		{ "Geometric Shapes", {0x25A0, 0x25FF} },
		{ "Misc. Symbols", {0x2600, 0x26FF} },
		{ "Emoticons", {0x1F600, 0x1F64F} },
	};

	for (auto it = m_Blocks.constBegin(); it != m_Blocks.constEnd(); ++it)
	{
		const QString& key = it.key();
		m_BlockSelector->addItem(key);
	}
	

	connect(m_BlockSelector, &QComboBox::currentTextChanged,
		this, &CharacterSelectorWidget::PopulateCharacters);

	// ---- Scrollable character grid ----
	m_ScrollArea = new QScrollArea();
	m_ScrollArea->setWidgetResizable(true);
	m_MainLayout->addWidget(m_ScrollArea, 1);

	m_GridWidget = new QWidget();
	m_GridWidget->setObjectName("GridWidget");
	m_GridLayout = new QGridLayout(m_GridWidget);
	m_GridLayout->setSpacing(2);
	m_GridLayout->setContentsMargins(4, 4, 4, 4);
	m_ScrollArea->setWidget(m_GridWidget);

	PopulateCharacters("Basic Latin");
	StyleSelector();

}

void CharacterSelectorWidget::PopulateCharacters(const QString& blockName)
{
	// Clear previous characters
	QLayoutItem* item;
	while ((item = m_GridLayout->takeAt(0)) != nullptr) 
	{
		delete item->widget();
		delete item;
	}

	const auto range = m_Blocks[blockName];
	const int start = range.first;
	const int end = range.second;

	const int cols = 12;
	int row = 0, col = 0;

	for (int code = start; code <= end; ++code) {
		QChar ch(code);
		QPushButton* btn = new QPushButton(QString(ch));
		btn->setFixedSize(28, 28);
		StyleGridButton(btn);
		m_GridLayout->addWidget(btn, row, col);

		connect(btn, &QPushButton::clicked, this, [this, ch]() {
			emit CharacterSelected(ch);
			});

		if (++col >= cols) 
		{
			col = 0;
			++row;
		}
	}

	m_GridLayout->setRowStretch(row + 1, 1);
}

void CharacterSelectorWidget::StyleSelector()
{
	m_BlockSelector->setStyleSheet(R"(
		QComboBox {
			border: 1px solid #636363;
			border-radius: 2px;
			background: #404040;
			color: white;
			padding: 4px;
			font-size: 11px;
		}
		QComboBox::drop-down {
			border: none;
			background: transparent;
			width: 20px;
		}
		QComboBox::down-arrow {
			image: url(:/icons/dropdown.png);
			width: 12px;
			height: 12px;
		}
		QComboBox QAbstractItemView {
			background: #333333;
			border: 1px solid #636363;
			selection-background-color: #333;
			selection-color: white;
		}

	)");


	m_ScrollArea->setStyleSheet(R"(
		QScrollArea {
			border: 1px solid #636363;
			border-radius: 2px;
		}
	)");

	m_GridWidget->setStyleSheet(R"(
		#GridWidget {
			background: #404040;
		}
	)");
}

void CharacterSelectorWidget::StyleGridButton(QPushButton* button)
{
	button->setStyleSheet(R"(
		QPushButton {
			font-size: 14px;
			border: none;
			background: #222222;
			color: white;
			border-radius: 2px;
		}
		QPushButton:hover {
			background: #333333;
		}
		QPushButton:pressed {
			background: #111111;
		}
	)");
}
