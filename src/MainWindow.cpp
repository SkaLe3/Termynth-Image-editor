#include "MainWindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QColorDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QPainter>
#include <QFont>
#include <QFile>
#include <QDataStream>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QStatusBar>
#include <QButtonGroup>
#include <QTimer>
#include <QSizeGrip>
#include <QFormLayout>
#include <QCloseEvent>


#include "CanvasView.h"
#include "CellItem.h"
#include "ColorSwatch.h"
#include "ToolPanelDock.h"
#include "ToolPropertiesPanelDock.h"
#include "TitleBarWidget.h"

#include <cstdint>




MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, m_Width(0), m_Height(0)
	, m_CurrentTool(ETool::Brush)
	, m_CurrentChar('G')
	, m_CurrentAttributeSet(0)
	, m_bIsModified(false)
	, m_bCurrentTransparency(false)
	, m_CurrentImageFormat(EImageFormat::AttributesColoredText)
{
	SetupUI();
	SetupMenuBar();

	setWindowTitle("Termynth Image Editor");
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
	setAttribute(Qt::WA_TranslucentBackground);
	resize(1200, 800);

	setContentsMargins(1, 1, 1, 1);
}

MainWindow::~MainWindow()
{

}

void MainWindow::SetupUI()
{
	m_TitleBar = new TitleBarWidget(this);
	setMenuWidget(m_TitleBar);

	QWidget* centralWidget = new QWidget(this);
	centralWidget->setObjectName("MainBackground");
	QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
	mainLayout->setContentsMargins(0, 0, 0,0);
	mainLayout->setSpacing(0);
	setCentralWidget(centralWidget);


	//Scene setup
	m_Scene = new QGraphicsScene(this);
	m_View = new CanvasView(this);
	m_View->setScene(m_Scene);
	m_Scene->setSceneRect(0, 0, 6000, 6000);
	mainLayout->addWidget(m_View, 1);

	connect(m_TitleBar, &TitleBarWidget::MinimizeRequested, this, &MainWindow::showMinimized);
	connect(m_TitleBar, &TitleBarWidget::MaximizeRequested, this, [this]() {
		isMaximized() ? showNormal() : showMaximized();
		});
	connect(m_TitleBar, &TitleBarWidget::CloseRequested, this, &MainWindow::close);


	// Tools	
	m_ToolPanel = new ToolPanelDock(this);
	addDockWidget(Qt::LeftDockWidgetArea, m_ToolPanel);

	QObject::connect(m_ToolPanel, &ToolPanelDock::ToolSelected, this, &MainWindow::OnToolSelected);

	QObject::connect(m_ToolPanel, &ToolPanelDock::ColorChanged, this, [](const QColor& color) {
		qDebug() << "Color changed to" << color;
		});

	m_ToolPropertiesPanel = new ToolPropertiesPanelDock(this);
	addDockWidget(Qt::TopDockWidgetArea, m_ToolPropertiesPanel);
	connect(m_ToolPanel, &ToolPanelDock::ToolSelected, m_ToolPropertiesPanel, &ToolPropertiesPanelDock::OnToolSelected);
	connect(m_ToolPropertiesPanel, &ToolPropertiesPanelDock::CharacterSelected, this, &MainWindow::OnSelectedChar);
	connect(m_ToolPropertiesPanel, &ToolPropertiesPanelDock::ApplyToAll, this, &MainWindow::OnApplyToAll);
	connect(m_ToolPropertiesPanel, &ToolPropertiesPanelDock::AttributeSelected, this, &MainWindow::OnAddRemoveAttributeToSet);
	connect(m_ToolPropertiesPanel, &ToolPropertiesPanelDock::ColorSelected, [this](const QColor& color)
		{
			m_ToolPanel->SelectColor(color);
		});
	connect(m_ToolPropertiesPanel, &ToolPropertiesPanelDock::TransparencySelected, this, &MainWindow::OnTransparencySelected);

	QTimer::singleShot(1, [this]() {m_ToolPanel->ActivateTool(ETool::Select); });

	// Status bar
	m_StatusLabel = new QLabel("Ready");
	statusBar()->addWidget(m_StatusLabel);
	

	SetupTheme();

}

void MainWindow::SetupMenuBar()
{
	connect(m_TitleBar, &TitleBarWidget::NewFile, this, &MainWindow::OnNewFile);
	connect(m_TitleBar, &TitleBarWidget::OpenFile, this, &MainWindow::OnOpenFile);
	connect(m_TitleBar, &TitleBarWidget::SaveFile, this, &MainWindow::OnSaveFile);
	connect(m_TitleBar, &TitleBarWidget::SaveFileAs, this, &MainWindow::OnSaveFileAs);
	connect(m_TitleBar, &TitleBarWidget::Close, this, &QWidget::close);

	m_TitleBar->AddWindowMenuAction(m_ToolPanel->toggleViewAction());
	m_TitleBar->AddWindowMenuAction(m_ToolPropertiesPanel->toggleViewAction());
}

void MainWindow::OnNewFile()
{
	QDialog dialog(this);
	dialog.setWindowTitle("New Canvas");

	QLineEdit* widthEdit = new QLineEdit(&dialog);
	widthEdit->setValidator(new QIntValidator(1, 400, widthEdit));
	widthEdit->setText("32");
	widthEdit->setFixedWidth(60);

	QLineEdit* heightEdit = new QLineEdit(&dialog);
	heightEdit->setValidator(new QIntValidator(1, 100, heightEdit));
	heightEdit->setText("32");
	heightEdit->setFixedWidth(60);

	QFormLayout* formLayout = new QFormLayout;
	formLayout->addRow("Width", widthEdit);
	formLayout->addRow("Height", heightEdit);

	QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	QPushButton* createButton = buttons->button(QDialogButtonBox::Ok);
	QPushButton* closeButton = buttons->button(QDialogButtonBox::Cancel);

	createButton->setText("Create");
	closeButton->setText("Close");

	createButton->setObjectName("CreateButton");
	closeButton->setObjectName("CloseButton");
	createButton->setFixedSize(70, 28);
	closeButton->setFixedSize(70, 28);

	connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addLayout(formLayout);
	mainLayout->addWidget(buttons);

	dialog.setLayout(mainLayout);

	dialog.setStyleSheet(R"(
    /* Style labels (Width: / Height:) */
    QFormLayout QLabel {
        color: #9d9d9d;
        font-weight: bold;
        font-size: 14px;
    }

    /* Style line edits */
    QLineEdit {
        background-color: #333333;
        color: #ffffff;
        border: 1px solid #636363;
        border-radius: 4px;
        padding: 4px 14px;
        selection-background-color: #444444;
        selection-color: #ffffff;
        font-size: 14px;
    }

	QPushButton {
		color: #ffffff;
		border-radius: 12px;
		padding: 6px 12px;
		font-size: 14px;
		font-weight: bold;
		border: 2px;
	}

    /* Style QDialogButtonBox buttons */
    QPushButton#CreateButton {
        background-color: #7604d6;
		border: 2px solid #7604d6;
    }

    /* Hover state */
     QPushButton#CreateButton:hover {
        background-color: #6602c6;
		border: 2px solid #6602c6;
    }

    /* Pressed state */
     QPushButton#CreateButton:pressed {
        background-color: #6002b8;
		border: 2px solid #6002b8;
    }

	QPushButton#CloseButton	{
		background-color: transparent;
		border: 2px solid #dddddd;
	}
	QPushButton#CloseButton:hover {
		background-color: #dddddd;
		border: 2px solid #dddddd;
		color: #4b4b4b;
	}
	QPushButton#CloseButton:pressed {
		background-color: #ffffff;
		border: 2px solid #ffffff;
	}
)");


	if (dialog.exec() == QDialog::Accepted)
	{
		int32_t width = widthEdit->text().toInt();
		int32_t height = heightEdit->text().toInt();
		
		CreateCanvas(width, height);
		m_CurrentFilePath.clear();
		m_bIsModified = true;
		setWindowTitle("Termynth Image Editor - Untitled");
	}
}

void MainWindow::CreateCanvas(int32_t width, int32_t height)
{
	m_Scene->clear();
	m_Cells.clear();
	m_View->Clear();
	m_Width = width;
	m_Height = height;
	qreal cellSize = 40;

	int32_t canvasOriginX = (m_Scene->sceneRect().width() - (width * cellSize))/2;
	int32_t canvasOriginY = (m_Scene->sceneRect().height() - (height * cellSize))/2;

	QGraphicsRectItem* canvasBase = new QGraphicsRectItem(0, 0, width * cellSize, height * cellSize * 2.f);
	canvasBase->setZValue(0);
	canvasBase->setPos(canvasOriginX, canvasOriginY);
	m_Scene->addItem(canvasBase);

	m_Cells.resize(height);

	for (int y = 0; y < height; ++y)
	{
		m_Cells[y].resize(width);
		for (int x = 0; x < width; ++x)
		{
			CellItem* cell = new CellItem(x, y, cellSize, canvasBase);
			cell->setZValue(1);
			m_Cells[y][x] = cell;

			CellData data;
			cell->UpdateCell(data);
		}
	}
	m_CellHightlight = new CellHightlight(cellSize, cellSize * 2.f, canvasBase);
	m_CellHightlight->setZValue(2);

	disconnect(m_View, nullptr, this, nullptr);

	connect(m_View, &CanvasView::HoveredCellChanged, [this, cellSize ](int32_t x, int32_t y)
		{
			if (!m_CellHightlight) return;
			m_CellHightlight->Show();
			m_CellHightlight->setPos(x * cellSize, y * cellSize * 2.f);
		});
	connect(m_View, &CanvasView::HoveredCellGone, [this, cellSize]()
		{
			if (!m_CellHightlight) return;
			m_CellHightlight->Hide();
		});
	disconnect(m_View, &CanvasView::CanvasDragged, this, &MainWindow::OnCanvasDragged);
	disconnect(m_View, &CanvasView::CanvasClicked, this, &MainWindow::OnCanvasClicked);
	connect(m_View, &CanvasView::CanvasDragged, this, &MainWindow::OnCanvasDragged);
	connect(m_View, &CanvasView::CanvasClicked, this, &MainWindow::OnCanvasClicked);
	m_StatusLabel->setText(QString("Canvas: %1x%2").arg(width).arg(height));
}

void MainWindow::OnCanvasClicked(int32_t x, int32_t y, bool shiftPressed)
{
	if (x < 0 || x >= m_Width || y < 0 || y >= m_Height) return;
	ApplyCurrentTool(x, y, shiftPressed);
}

void MainWindow::OnCanvasDragged(int32_t x, int32_t y, bool shiftPressed)
{
	if (x < 0 || x >= m_Width || y < 0 || y >= m_Height) return;
	ApplyCurrentTool(x, y, shiftPressed);
}

void MainWindow::ApplyCurrentTool(int32_t x, int32_t y, bool shiftPressed)
{
	CellData data = m_Cells[y][x]->GetData();

	switch (m_CurrentTool) {
	case ETool::Brush:
		if (!shiftPressed)
		{
			data.Character = m_CurrentChar;
		}
		else
		{
			data.Character = ' ';
		}
		break;
	case ETool::Select:

		return;

	case ETool::Paint:
		if (shiftPressed)
		{
			data.BgColor = m_ToolPanel->GetActiveColor();
		}
		else
		{
			data.FgColor = m_ToolPanel->GetActiveColor();
		}
		break;

	case ETool::Attribute:

		data.Transparent = m_bCurrentTransparency;

		if (m_CurrentAttributeSet & static_cast<uint8_t>(EAttribute::Bold))
		{
			data.Bold = !shiftPressed;
		}
		if (m_CurrentAttributeSet & static_cast<uint8_t>(EAttribute::Dim))
		{
			data.Dim = !shiftPressed;
		}
		if (m_CurrentAttributeSet & static_cast<uint8_t>(EAttribute::Italic))
		{
			data.Italic = !shiftPressed;
		}
		if (m_CurrentAttributeSet & static_cast<uint8_t>(EAttribute::Underline))
		{
			data.Underline = !shiftPressed;
		}
		if (m_CurrentAttributeSet & static_cast<uint8_t>(EAttribute::Blink))
		{
			data.Blink = !shiftPressed;
		}
		if (m_CurrentAttributeSet & static_cast<uint8_t>(EAttribute::Inverse))
		{
			data.Inverse = !shiftPressed;
		}
		if (m_CurrentAttributeSet & static_cast<uint8_t>(EAttribute::DefaultFg))
		{
			data.DefaultFg = !shiftPressed;
		}
		if (m_CurrentAttributeSet & static_cast<uint8_t>(EAttribute::DefaultBg))
		{
			data.DefaultBg = !shiftPressed;
		}
		break;
	}

	m_Cells[y][x]->UpdateCell(data);
	m_bIsModified = true;
}



void MainWindow::paintEvent(QPaintEvent* event)
{
	QMainWindow::paintEvent(event);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

	QColor backgroundColor = QColor("#4b4b4b");
	QColor borderColor = QColor("#333333");
	int32_t borderWidth = 1;
	int32_t radius = 5;
	
	QRectF rect = this->rect();
	rect.adjust(borderWidth / 2.0, borderWidth / 2.0,
		-borderWidth / 2.0, -borderWidth / 2.0);

	QPainterPath path;
	path.addRoundedRect(rect, radius, radius);

	painter.fillPath(path, backgroundColor);

	QPen pen(borderColor, borderWidth);
	painter.setPen(pen);
	painter.drawPath(path);
}


void MainWindow::closeEvent(QCloseEvent* event)
{
	if (!m_bIsModified)
	{
		event->accept();
		return;
	}
	QMessageBox msgBox(this);
	msgBox.setWindowTitle("Confirm Exit");
	msgBox.setText("Do you want to save changes to your current file before exiting?");
	msgBox.setIcon(QMessageBox::Question);
	QPushButton* saveButton = msgBox.addButton("Save", QMessageBox::AcceptRole);
	QPushButton* dontSaveButton = msgBox.addButton("Don't Save", QMessageBox::DestructiveRole);
	QPushButton* cancelButton = msgBox.addButton("Cancel", QMessageBox::RejectRole);

	msgBox.exec();

	if (msgBox.clickedButton() == saveButton)
	{
		OnSaveFile();
		if (m_bIsModified)
		{
			event->ignore();
			return;
		}
		event->accept();
	}
	else if (msgBox.clickedButton() == dontSaveButton) 
	{
		event->accept();
	}
	else if (msgBox.clickedButton() == cancelButton)
	{
		event->ignore();
	}
}

void MainWindow::OnToolSelected(ETool tool)
{
	m_CurrentTool = tool;
	QString toolName;
	switch (tool)
	{
	case ETool::Brush: toolName = "Brush"; break;
	case ETool::Select: toolName = "Select"; break;
	case ETool::Paint: toolName = "Paint"; break;
	case ETool::Attribute: toolName = "Attribute"; break;
	}
	m_StatusLabel->setText("Current tool: " + toolName);
}

void MainWindow::OnSelectedChar(QChar ch)
{
	m_CurrentChar = ch;
}

void MainWindow::OnApplyToAll(QChar ch, bool bInvert)
{
	QChar savedChar = m_CurrentChar;
	m_CurrentChar = ch;
	for (int32_t x = 0; x < m_Width; ++x)
	{
		for (int32_t y = 0; y < m_Height; ++y)
		{
			ApplyCurrentTool(x, y, bInvert);
		}
	}
	m_CurrentChar = savedChar;
}




void MainWindow::OnAddRemoveAttributeToSet(EAttribute attr, bool bSelected)
{
	QString attrName;
	switch (attr)
	{
	case EAttribute::Bold: attrName = "Bold"; break;
	case EAttribute::Dim: attrName = "Dim"; break;
	case EAttribute::Italic: attrName = "Italic"; break;
	case EAttribute::Underline: attrName = "Underline"; break;
	case EAttribute::Blink: attrName = "Blink"; break;
	case EAttribute::Inverse: attrName = "Inverse"; break;
	case EAttribute::DefaultFg: attrName = "DefaultFg"; break;
	case EAttribute::DefaultBg: attrName = "DefaultBg"; break;
	}

	if (bSelected)
	{
		m_CurrentAttributeSet = m_CurrentAttributeSet | static_cast<uint8_t>(attr);
		m_StatusLabel->setText("Attribute added to set: " + attrName);
	}
	else
	{
		m_CurrentAttributeSet = m_CurrentAttributeSet & ~static_cast<uint8_t>(attr);
		m_StatusLabel->setText("Attribute removed from set: " + attrName);
	}
}

void MainWindow::OnTransparencySelected(bool bSelected)
{
	m_bCurrentTransparency = bSelected;
}


void MainWindow::OnSaveFile()
{
	if (m_Cells.isEmpty())
	{
		return;
	}
	if (m_CurrentFilePath.isEmpty())
	{
		OnSaveFileAs();
	}
	else
	{
		SaveToFile(m_CurrentFilePath, m_CurrentImageFormat);
	}
}

void MainWindow::OnSaveFileAs()
{
	if (m_Cells.isEmpty())
	{
		return;
	}

	SaveDialog dlg(this);
	if (dlg.exec() == QDialog::Accepted)
	{
		QString filename = dlg.FilePath();
		if (filename.isEmpty()) return;
		EImageFormat format = dlg.SelectedFormat();
		m_CurrentImageFormat = format;
		if (SaveToFile(filename, format))
		{
			m_CurrentFilePath = filename;
			setWindowTitle("Termynth Image Editor - " + QFileInfo(filename).fileName());
		}

	}

}

void MainWindow::OnOpenFile()
{
	QString filename = QFileDialog::getOpenFileName(this, "Open Texture",
		"", "Texture Files (*.thtx)");
	if (!filename.isEmpty())
	{
		if (LoadFromFile(filename))
		{
			m_CurrentFilePath = filename;
			setWindowTitle("Termynth Image Editor - " + QFileInfo(filename).fileName());
		}
	}
}

bool MainWindow::SaveToFile(const QString& filename, EImageFormat format)
{
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly)) {
		QMessageBox::critical(this, "Error", "Could not save file");
		return false;
	}

	quint8 channelNum = 10;
	switch (format)
	{
	case EImageFormat::DefaultText:
		channelNum = 3;
		break;
	case EImageFormat::AttributesText:
		channelNum = 4;
		break;
	case EImageFormat::ColoredText:
		channelNum = 9;
		break;
	case EImageFormat::AttributesColoredText:
	default:
		channelNum = 10;
		break;
	}


	quint32 magic = (static_cast<quint32>('T') << 24) |
		(static_cast<quint32>('H') << 16) |
		(static_cast<quint32>('T') << 8) |
		(static_cast<quint32>('X'));

	QDataStream out(&file);
	out.setByteOrder(QDataStream::LittleEndian);
	out << magic; // Magic number
	out << quint32(m_Width) << quint32(m_Height);
	out << channelNum; // channel

	for (int y = 0; y < m_Height; ++y) {
		for (int x = 0; x < m_Width; ++x) {
			CellData data = m_Cells[y][x]->GetData();
			data.Serialize(out, format);

		}
	}

	m_bIsModified = false;
	m_StatusLabel->setText("File saved: " + filename);
	return true;
}

void MainWindow::SetupTheme()
{
	m_Scene->setBackgroundBrush(QColor(45, 45, 45));
	setStyleSheet(R"(
    QMainWindow::separator {
        width: 1px;
        background: #222;
    }
    QMainWindow::dockwidget {
        margin: 0;
        padding: 0;
    }
	)");
	statusBar()->setStyleSheet("QStatusBar {background: transparent;}");
	statusBar()->findChild<QSizeGrip*>()->setStyleSheet("background: transparent; border:none;");
	centralWidget()->setStyleSheet("#MainBackground {background: transparent;}");

	m_View->setStyleSheet(R"(
	QScrollBar:vertical {
		background: transparent;
		width: 16px;
		margin: 0px;
		border-radius: 0px;
		border: 2px solid #474747;
	}
	QScrollBar::handle:vertical {
        background: #636363;
        min-height: 20px;
        border-radius: 4px;
    }
    QScrollBar::handle:vertical:hover {
        background: #656565;
    }
    QScrollBar::add-line:vertical,
    QScrollBar::sub-line:vertical {
        height: 0px; /* hide arrow buttons */
    }
    QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
        background: #474747;  /* area behind handle (track) */
    }

    QScrollBar:horizontal {
        background: transparent;
        height: 16px;
        margin: 0px;
        border-radius: 0px;
		border: 2px solid #474747;
    }
    QScrollBar::handle:horizontal {
        background: #636363;
        min-width: 20px;
        border-radius: 4px;
    }
    QScrollBar::handle:horizontal:hover {
        background: #656565;
    }
    QScrollBar::add-line:horizontal,
    QScrollBar::sub-line:horizontal {
        width: 0px; /* hide arrow buttons */
    }
	QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
        background: #474747;  /* area behind handle (track) */
    }
	)");

}

bool MainWindow::LoadFromFile(const QString& filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly)) {
		QMessageBox::critical(this, "Error", "Could not open file");
		return false;
	}

	quint32 myMagic = (static_cast<quint32>('T') << 24) |
		(static_cast<quint32>('H') << 16) |
		(static_cast<quint32>('T') << 8) |
		(static_cast<quint32>('X'));

	QDataStream in(&file);
	in.setByteOrder(QDataStream::LittleEndian);
	quint32 magic, width, height;
	in >> magic;

	if (magic != myMagic) {
		QMessageBox::critical(this, "Error", "Invalid file format");
		return false;
	}

	in >> width >> height;
	quint8 channel;
	in >> channel;

	EImageFormat imageFormat = EImageFormat::AttributesColoredText;
	switch (channel)
	{
	case 3:
		imageFormat = EImageFormat::DefaultText;
		break;
	case 4:
		imageFormat = EImageFormat::AttributesText;
		break;
	case 9:
		imageFormat = EImageFormat::ColoredText;
		break;
	case 10:
		imageFormat = EImageFormat::AttributesColoredText;
		break;
	}

	CreateCanvas(width, height);
	for (int y = 0; y < m_Height; ++y) {
		for (int x = 0; x < m_Width; ++x) {
			CellData data;
			data.Deserialize(in, imageFormat);
			m_Cells[y][x]->UpdateCell(data);
		}
	}

	m_bIsModified = false;
	m_StatusLabel->setText("File loaded: " + filename);
	return true;
}