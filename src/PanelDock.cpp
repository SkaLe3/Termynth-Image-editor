#include "PanelDock.h"

#include <QTimer>
#include <QPainter>

PanelDock::PanelDock(const QString& title, bool bVertical, QWidget* parent /*= nullptr*/)
	: QDockWidget(title, parent), m_bVertical(bVertical)
{
	setAttribute(Qt::WA_TranslucentBackground, true);

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
	m_Content->setObjectName("DockWidgetContent");
	m_Content->setAttribute(Qt::WA_StyledBackground, true);
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
	m_Titlebar->setObjectName("DockWidgetTitlebar");
	QBoxLayout* titlebarLayout;
	if (m_bVertical)
	{
		titlebarLayout = new QHBoxLayout(m_Titlebar);
		m_Titlebar->setFixedHeight(14);
	}
	else
	{
		titlebarLayout = new QVBoxLayout(m_Titlebar);
		m_Titlebar->setFixedWidth(14);
	}
	titlebarLayout->setContentsMargins(0, 0, 0, 0);

	m_CloseButton = new QPushButton(m_Titlebar);
	m_CloseButton->setFixedSize(14, 14);

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
		border: 1px solid #333333;
    }
	QPushButton:checked {
        background-color: rgba(255, 255, 255, 10);
		border: 1px solid #333333;
    }
	QPushButton:pressed {
        background-color: #333333;
		border: 1px solid #333333;
    }
)");
}

void PanelDock::StylePanel()
{
	if (m_bVertical)
	{
		m_Titlebar->setStyleSheet(R"(
		QWidget {
		border-top-right-radius: 2px;		
		}
		#DockWidgetTitlebar{
		background-color: #404040;
		border: 1px solid #333333;
		border-top-left-radius: 2px;
		border-top-right-radius: 2px;
		border-bottom-left-radius: 0px;
		border-bottom-right-radius: 0px;
		color: white;
		}
	)");
	}
	else
	{
		m_Titlebar->setStyleSheet(R"(
		#DockWidgetTitlebar {
		background-color: #404040;
		border: 1px solid #333333;
		border-top-left-radius: 2px;
		border-top-right-radius: 0px;
		border-bottom-left-radius: 2px;
		border-bottom-right-radius: 0px;
		color: white;
		}
	)");
	}

	m_Content->setStyleSheet(R"(
	QWidget #DockWidgetContent {
		border: 1px solid #333333;
	}
	)");

	setStyleSheet(R"(
		QDockWidget 
		{
			background: transparent;

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
