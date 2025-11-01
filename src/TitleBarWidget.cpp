#include "TitleBarWidget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QApplication>
#include <QStyle>
#include <QToolButton>

TitleBarWidget::TitleBarWidget(QWidget* parent /*= nullptr*/)
	: QWidget(parent)
{
	setFixedHeight(28);
	setObjectName("TitleBar");

	// Layout
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setContentsMargins(6, 0, 6, 0);
	layout->setSpacing(6);

	// Menu Bar
	m_MenuBar = new QMenuBar(this);
	m_MenuBar->setObjectName("TitleBarMenu");
	m_MenuBar->setNativeMenuBar(false);

	QWidget* menuContainer = new QWidget(this);
	QHBoxLayout* menuLayout = new QHBoxLayout(menuContainer);
	menuLayout->setContentsMargins(0, 0, 0, 0);
	menuLayout->addWidget(m_MenuBar, 0, Qt::AlignLeft);

	m_MenuBar->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
	layout->addWidget(menuContainer, 1, Qt::AlignVCenter);

	layout->addStretch();


	QWidget* buttonsWidget = new QWidget(this);
	QHBoxLayout* buttonsLayout = new QHBoxLayout(buttonsWidget);
	buttonsLayout->setContentsMargins(0, 0, 0, 0);
	buttonsLayout->setSpacing(0);
	buttonsLayout->setAlignment(Qt::AlignTop);


	QToolButton* minButton = new QToolButton(this);
	QToolButton* maxButton = new QToolButton(this);
	QToolButton* closeButton = new QToolButton(this);

	minButton->setIcon(QIcon(CreateMinimizeIconPixmap()));
	minButton->setIconSize(minButton->size());
	minButton->setObjectName("MinimizeButton");
	maxButton->setIcon(QIcon(CreateMaximizeIconPixmap()));
	maxButton->setIconSize(maxButton->size());
	maxButton->setObjectName("MaximizeButton");
	closeButton->setIcon(QIcon(CreateCloseIconPixmap()));
	closeButton->setIconSize(closeButton->size());
	closeButton->setObjectName("CloseButton");

	minButton->setFixedSize(30, 20);
	maxButton->setFixedSize(30, 20);
	closeButton->setFixedSize(40, 20);

	buttonsLayout->addWidget(minButton);
	buttonsLayout->addWidget(maxButton);
	buttonsLayout->addWidget(closeButton);

	layout->addWidget(buttonsWidget);

	connect(minButton, &QPushButton::clicked, this, &TitleBarWidget::MinimizeRequested);
	connect(maxButton, &QPushButton::clicked, this, &TitleBarWidget::MaximizeRequested);
	connect(closeButton, &QPushButton::clicked, this, &TitleBarWidget::CloseRequested);

	StyleTitleBar();

	SetupMenuBar(m_MenuBar);
}


void TitleBarWidget::AddWindowMenuAction(QAction* action)
{
	m_WindowMenu->addAction(action);
}

void TitleBarWidget::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_IsDragging = true;
		m_DragPosition = event->globalPosition().toPoint() - window()->frameGeometry().topLeft();
	}
}

void TitleBarWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (m_IsDragging && (event->buttons() & Qt::LeftButton))
	{
		window()->move(event->globalPosition().toPoint() - m_DragPosition);

	}
}


void TitleBarWidget::mouseReleaseEvent(QMouseEvent*)
{
	m_IsDragging = false;
}


void TitleBarWidget::SetupMenuBar(QMenuBar* menuBar)
{
	QMenu* fileMenu = menuBar->addMenu("&File");

	QAction* newAction = new QAction("&New", this);
	newAction->setShortcut(QKeySequence::New);
	connect(newAction, &QAction::triggered, [this]() {emit NewFile(); });

	QAction* openAction = new QAction("&Open", this);
	openAction->setShortcut(QKeySequence::Open);
	connect(openAction, &QAction::triggered, [this]() {emit OpenFile(); });

	QAction* saveAction = new QAction("&Save", this);
	saveAction->setShortcut(QKeySequence::Save);
	connect(saveAction, &QAction::triggered, [this]() {emit SaveFile(); });

	QAction* saveAsAction = new QAction("Save &As", this);
	saveAsAction->setShortcut(QKeySequence::SaveAs);
	connect(saveAsAction, &QAction::triggered, [this]() {emit SaveFileAs(); });

	QAction* exitAction = new QAction("&Exit", this);
	exitAction->setShortcut(QKeySequence::Quit);
	connect(exitAction, &QAction::triggered, [this]() {emit Close(); });

	fileMenu->addAction(newAction);
	fileMenu->addAction(openAction);
	fileMenu->addSeparator();
	fileMenu->addAction(saveAction);
	fileMenu->addAction(saveAsAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);

	m_WindowMenu = menuBar->addMenu("&Window");


	StyleMenu(fileMenu);
	StyleMenu(m_WindowMenu);
}

void TitleBarWidget::StyleTitleBar()
{
	setStyleSheet(R"(
        QMenuBar {
            background-color: transparent;
            color: white;
			margin: 0px;
        }

        QToolButton {
            background: transparent;
            color: white;
            border: 1px solid #333333;
			border-left: none;
			border-top: none;
        }
        QToolButton:hover {
            background: #404040;
        }
        QToolButton:pressed {
            background: #333333;
			padding-top: 0px;
			padding-left: 0px;
        }
		#MinimizeButton {
			border-left: 1px solid #333333;
			border-bottom-left-radius: 2px;
		}
		#CloseButton {
			border-bottom-right-radius: 2px;
		}
    )");

	m_MenuBar->setStyleSheet(R"(
		QMenuBar::item {
			background-color: transparent;
			padding: 1px 8px;
		}
		QMenuBar::item:open {
			border: 1px solid #636363;
			border-radius: 2px;
            background: #333333;
        }
        QMenuBar::item:selected {
			border: 1px solid #636363;
			border-radius: 2px;
            background: #404040;
        }
)");
}

void TitleBarWidget::StyleMenu(QMenu* menu)
{
	menu->setStyleSheet(R"(
	QMenu {
		background-color: #4b4b4b;
		border: 1px solid #333333;
		padding: 2px;
	}
	QMenu::item {
		padding: 4px 20px;
		background-color: transparent;
		color white;
	}
	QMenu::item::selected {
		background-color: #636363;
	}
	QMenu::separator {
		height: 1px;
		background: #333333;
		margin: 4px 0;
	}
	)");
}

QPixmap TitleBarWidget::CreateMinimizeIconPixmap()
{
	QPixmap pixmap(":/icons/minimize.png");
	CreateIconPixmap(pixmap, QColor("#ababab"));
	return pixmap;
}

QPixmap TitleBarWidget::CreateMaximizeIconPixmap()
{
	QPixmap pixmap(":/icons/maximize.png");
	CreateIconPixmap(pixmap, QColor("#ababab"));
	return pixmap;
}

QPixmap TitleBarWidget::CreateCloseIconPixmap()
{
	QPixmap pixmap(":/icons/closewindow.png");
	CreateIconPixmap(pixmap, QColor("#ababab"));
	return pixmap;
}

void TitleBarWidget::CreateIconPixmap(QPixmap& iconPixmap, const QColor& color)
{

	QPixmap pixmap = QPixmap(iconPixmap.size());
	pixmap.fill(Qt::transparent);

	QPainter painter(&pixmap);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.drawPixmap(0, 0, iconPixmap);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.fillRect(pixmap.rect(), color);
	painter.end();
	iconPixmap = pixmap;
}

