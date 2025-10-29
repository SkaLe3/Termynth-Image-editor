#include "PanelDock.h"

#include <QTimer>
#include <QPainter>

PanelDock::PanelDock(const QString& title, bool bVertical, QWidget* parent /*= nullptr*/)
	: QDockWidget(title, parent), m_bVertical(bVertical)
{
	QTimer::singleShot(0, this, [this]() {
		HandleFloating(isFloating());
		});

	// Handle floating state changes
	connect(this, &QDockWidget::topLevelChanged, [this](bool floating)
		{
			HandleFloating(floating);
		});

	CreateCloseIconPixmap();


	m_Content = new QWidget(this);
	m_Content->setObjectName(title + "Content");
	if (m_bVertical)
	{
		m_PanelLayout = new QVBoxLayout(m_Content);
	}
	else
	{
		m_PanelLayout = new QHBoxLayout(m_Content);
	}
	m_PanelLayout->setContentsMargins(4, 4, 4, 4);
	m_PanelLayout->setSpacing(0);

	m_Content->setLayout(m_PanelLayout);
	setWidget(m_Content);

	// Titlebar
	m_Titlebar = new QWidget(this);
	m_Titlebar->setObjectName(title + "Titlebar");
	QBoxLayout* titlebarLayout;
	if (m_bVertical)
	{
		titlebarLayout = new QHBoxLayout(m_Titlebar);
		m_Titlebar->setFixedHeight(12);
	}
	else
	{
		titlebarLayout = new QVBoxLayout(m_Titlebar);
		m_Titlebar->setFixedWidth(12);
	}
	titlebarLayout->setContentsMargins(0, 0, 0, 0);

	m_CloseButton = new QPushButton(m_Titlebar);
	m_CloseButton->setFixedSize(12, 12);

	titlebarLayout->addWidget(m_CloseButton, 0, Qt::AlignRight);
	connect(m_CloseButton, &QPushButton::clicked, this, &QDockWidget::close);
	setTitleBarWidget(m_Titlebar);
	titlebarLayout->insertStretch(0, 1);

	StyleHideCloseButton(m_CloseButton);

	StylePanel();
}

void PanelDock::HandleFloating(bool floating)
{
	QSize hint = widget()->sizeHint();
	if (floating)
	{
		setMinimumSize(hint);
		setMaximumSize(hint);
		resize(hint);
		setWindowFlags(/*Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | */ Qt::FramelessWindowHint | Qt::Window);
		show();
	}
	else
	{
		setWindowFlags(Qt::Widget);
		if (m_bVertical)
		{
			setMinimumHeight(0);
			setMaximumHeight(QWIDGETSIZE_MAX);
			setMinimumWidth(hint.width());
			setMaximumWidth(hint.width());
		}
		else
		{
			setMinimumWidth(0);
			setMaximumWidth(QWIDGETSIZE_MAX);
			setMinimumHeight(hint.height());
			setMaximumHeight(hint.height());
		}
		setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
		show();
	}
}

void PanelDock::StyleHideCloseButton(QPushButton* button)
{
	button->setEnabled(false);
	button->setIcon(QIcon()); // remove icon
	button->setStyleSheet("color: transparent; background: transparent; border: none;");
}

void PanelDock::StyleCloseButton(QPushButton* button)
{
	button->setEnabled(true);
	button->setIcon(QIcon(m_CloseIconPixmap));
	button->setIconSize(button->size());

	button->setStyleSheet(R"(
	QPushButton {
        background-color: transparent;
		border: 1px solid transparent;
        color: white;
		outline: none;
    }
    QPushButton:hover {
        background-color: rgba(255, 255, 255, 30);
		border: 1px solid rgba(50, 50, 50, 255);
    }
	QPushButton:checked {
        background-color: rgba(255, 255, 255, 10);
		border: 1px solid rgba(50, 50, 50, 255);
    }
	QPushButton:pressed {
        background-color: rgba(50, 50, 50, 255);
		border: 1px solid rgba(50, 50, 50, 255);
    }
)");
}

void PanelDock::StylePanel()
{
	m_Titlebar->setStyleSheet(R"(
		background-color: #404040;
		border: 1px solid #333333;
		border-radius: 2px;
		color: white;
	)");



	setStyleSheet(R"(
		QDockWidget  {
            border: 0px solid #000000;
			border-radius: 2px;
            margin: 0px;
            padding: 0px;
        }
        QDockWidget::title {
            margin: 0px;
            padding: 0px;
        }
    )");
}

void PanelDock::CreateCloseIconPixmap()
{
	QPixmap pixmap(":/icons/close.png");
	m_CloseIconPixmap = QPixmap(pixmap.size());
	m_CloseIconPixmap.fill(Qt::transparent);

	QPainter painter(&m_CloseIconPixmap);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.drawPixmap(0, 0, pixmap);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.fillRect(m_CloseIconPixmap.rect(), QColor("#ababab"));
	painter.end();
}
