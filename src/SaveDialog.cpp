#include "SaveDialog.h"

#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include <QComboBox>

SaveDialog::SaveDialog(QWidget* parent /*= nullptr*/)
	: QDialog(parent)
{
	setWindowTitle("Save Texture As...");
	setModal(true);
	resize(260, 100);

	QLabel* formatLabel = new QLabel("Format:");

	m_FormatCombo = new QComboBox(this);
	m_FormatCombo->addItem("DefaultText (3 bytes)");
	m_FormatCombo->addItem("AttributesText (4 bytes)");
	m_FormatCombo->addItem("ColoredText (9 bytes)");
	m_FormatCombo->addItem("AttributesColoredText (10 bytes)");

	m_SaveButton = new QPushButton("Save", this);
	m_CancelButton = new QPushButton("Cancel", this);
	connect(m_SaveButton, &QPushButton::clicked, this, &SaveDialog::OnSaveClicked);
	connect(m_CancelButton, &QPushButton::clicked, this, &SaveDialog::reject);
	m_SaveButton->setObjectName("SaveButton");
	m_CancelButton->setObjectName("CancelButton");
	m_SaveButton->setFixedSize(70, 28);
	m_CancelButton->setFixedSize(70, 28);

	QHBoxLayout* formatLayout = new QHBoxLayout;
	formatLayout->addWidget(formatLabel);
	formatLayout->addWidget(m_FormatCombo);

	QHBoxLayout* buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(m_CancelButton);
	buttonsLayout->addWidget(m_SaveButton);


	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(formatLayout);
	mainLayout->addStretch();
	mainLayout->addLayout(buttonsLayout);
	
	m_FormatCombo->setCurrentIndex(3);

	StyleDialog();
}

QString SaveDialog::FilePath() const
{
	return m_Filename;
}

EImageFormat SaveDialog::SelectedFormat() const
{
	return static_cast<EImageFormat>(m_FormatCombo->currentIndex());
}

void SaveDialog::OnSaveClicked()
{
	QString filename = QFileDialog::getSaveFileName(this, "Save Texture",
		"", "Texture Files (*.thtx)");
	if (!filename.isEmpty())
	{
		m_Filename = filename;
		SaveDialog::accept();
	}
}

void SaveDialog::StyleDialog()
{
	setStyleSheet(R"(
	QComboBox {
		background-color: #404040;
		color: #ffffff;
		border: 1px solid #636363;
		border-radius: 4px;
		padding: 4px;
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
		selection-background-color: transparent;
		outline: none;
		show-decoration-selected: 0;
		alternate-background-color: transparent;
	}
	QComboBox QAbstractItemView::item {
		min-height: 28px;
	}
	QComboBox QAbstractItemView::item:hover {
		background: #6002b8;
		border: 1px solid #7604d6
	}

	QPushButton {
		color: #ffffff;
		border-radius: 12px;
		padding: 6px 12px;
		font-size: 14px;
		font-weight: bold;
		border: 2px;
	}
	QPushButton#SaveButton {
		background-color: #7604d6;
		border: 2px solid #7604d6;
	}
	QPushButton#SaveButton:hover {
        background-color: #6602c6;
		border: 2px solid #6602c6;
    }
	QPushButton#SaveButton:pressed {
        background-color: #6002b8;
		border: 2px solid #6002b8;
    }
	QPushButton#CancelButton	{
		background-color: transparent;
		border: 2px solid #dddddd;
	}
	QPushButton#CancelButton:hover {
		background-color: #dddddd;
		border: 2px solid #dddddd;
		color: #4b4b4b;
	}
	QPushButton#CancelButton:pressed {
		background-color: #ffffff;
		border: 2px solid #ffffff;
	}
	)");
}
