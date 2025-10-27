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


#include "ui/ui_MainWindow.h"
#include "CanvasView.h"
#include "CellItem.h"
#include "ColorSwatch.h"

#include <cstdint>




MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, m_Width(0), m_Height(0)
	, m_CurrentTool(Tool::Brush)
	, m_CurrentChar('G')
	, m_CurrentFgColor(Qt::white)
	, m_CurrentBgColor(Qt::black)
	, m_CurrentAttribute(Attribute::Bold)
	, m_bIsModified(false)
	, m_bIsDrawing(false)
	, m_SelectedX(-1), m_SelectedY(-1)
	, m_Ui(new Ui::MainWindow)
{
	SetupUI();
	SetupMenuBar();

	setWindowTitle("Console Texture Editor");
	resize(1200, 800);


}

MainWindow::~MainWindow()
{
	delete m_Ui;
}

void MainWindow::SetupUI()
{
	// Cell info
	m_SelectedCellInfo = new QLabel("No cell selected");
	m_SelectedCellInfo->setWordWrap(true);
	//toolLayout->addWidget(m_SelectedCellInfo);

	m_Ui->setupUi(this);

	// Status bar
	m_StatusLabel = new QLabel("Ready");
	statusBar()->addWidget(m_StatusLabel);

	QVBoxLayout* mainLayout = new QVBoxLayout(m_Ui->centralwidget);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	m_Ui->centralwidget->setLayout(mainLayout);


	m_Scene = new QGraphicsScene(this);
	m_Scene->setSceneRect(0, 0, 6000, 6000);
	m_CanvasOriginX = 3000;
	m_CanvasOriginY = 3000;
	//m_Scene->setBackgroundBrush(Qt::lightGray);
	//m_Scene->setBackgroundBrush(QBrush(Qt::white));



	m_View = new CanvasView(this);
	m_View->setScene(m_Scene);

	mainLayout->addWidget(m_View, 1);

	// Tools	
	m_ToolPanel = m_Ui->ToolbarWidget;
	QVBoxLayout* toolLayout = new QVBoxLayout(m_Ui->ToolbarWidgetContents);
	m_ToolPanel->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	m_ToolPanel->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);




	QPushButton* selectButton = new QPushButton("Select");
	QPushButton* brushButton = new QPushButton("Brush");
	QPushButton* eraserButton = new QPushButton("Eraser");
	QPushButton* paintButton = new QPushButton("Paint");
	QPushButton* attrButton = new QPushButton("Attribute");

	selectButton->setCheckable(true);
	brushButton->setCheckable(true);
	eraserButton->setCheckable(true);
	paintButton->setCheckable(true);
	attrButton->setCheckable(true);

	QButtonGroup* toolGroup = new QButtonGroup(m_Ui->ToolbarWidgetContents);
	toolGroup->setExclusive(true);
	toolGroup->addButton(selectButton, 0);
	toolGroup->addButton(brushButton, 1);
	toolGroup->addButton(eraserButton, 2);
	toolGroup->addButton(paintButton, 3);
	toolGroup->addButton(attrButton, 4);

	connect(selectButton, &QPushButton::clicked, [this]() { SelectTool(Tool::Select); });
	connect(brushButton, &QPushButton::clicked, [this]() { SelectTool(Tool::Brush); });
	connect(eraserButton, &QPushButton::clicked, [this]() { SelectTool(Tool::Eraser); });
	connect(paintButton, &QPushButton::clicked, [this]() { SelectTool(Tool::Paint); });
	connect(attrButton, &QPushButton::clicked, [this]() { SelectTool(Tool::Attribute); });

	m_ColorSwatch = new ColorSwatch();

	toolLayout->addWidget(selectButton);
	toolLayout->addWidget(brushButton);
	toolLayout->addWidget(eraserButton);
	toolLayout->addWidget(paintButton);
	toolLayout->addWidget(attrButton);
	toolLayout->addWidget(m_ColorSwatch);
	toolLayout->addStretch();



	QObject::connect(m_ColorSwatch, &ColorSwatch::ColorChanged, [](const QColor& color) {
		qDebug() << "Color changed to" << color;
		});

	m_Ui->ToolbarWidgetContents->setLayout(toolLayout);

	QSize contentHint = m_Ui->ToolbarWidgetContents->sizeHint();

	m_ToolPanel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	m_ToolPanel->setMinimumWidth(contentHint.width());
	m_ToolPanel->setMaximumWidth(contentHint.width());

	QObject::connect(m_ToolPanel, &QDockWidget::topLevelChanged, [this](bool floating)
		{
			QWidget* content = m_ToolPanel->widget();
			QSize hint = content->sizeHint();

			if (floating)
			{
				m_ToolPanel->setMinimumSize(hint);
				m_ToolPanel->setMaximumSize(hint);
				m_ToolPanel->resize(hint);
				m_ToolPanel->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
				m_ToolPanel->show();
			}
			else
			{
				m_ToolPanel->setWindowFlags(Qt::Widget);
				m_ToolPanel->setMinimumHeight(0);
				m_ToolPanel->setMaximumHeight(QWIDGETSIZE_MAX);
				m_ToolPanel->setMinimumWidth(hint.width());
				m_ToolPanel->setMaximumWidth(hint.width());
				m_ToolPanel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
				m_ToolPanel->show();
			}
		});

	SelectTool(Tool::Brush);
}

void MainWindow::SetupMenuBar()
{
	QMenu* fileMenu = menuBar()->addMenu("&File");

	QAction* newAction = new QAction("&New", this);
	newAction->setShortcut(QKeySequence::New);
	connect(newAction, &QAction::triggered, this, &MainWindow::NewFile);

	QAction* openAction = new QAction("&Open", this);
	openAction->setShortcut(QKeySequence::Open);
	connect(openAction, &QAction::triggered, this, &MainWindow::OpenFile);

	QAction* saveAction = new QAction("&Save", this);
	saveAction->setShortcut(QKeySequence::Save);
	connect(saveAction, &QAction::triggered, this, &MainWindow::SaveFile);

	QAction* saveAsAction = new QAction("Save &As", this);
	saveAsAction->setShortcut(QKeySequence::SaveAs);
	connect(saveAsAction, &QAction::triggered, this, &MainWindow::SaveFileAs);

	QAction* exitAction = new QAction("&Exit", this);
	exitAction->setShortcut(QKeySequence::Quit);
	connect(exitAction, &QAction::triggered, this, &QWidget::close);

	fileMenu->addAction(newAction);
	fileMenu->addAction(openAction);
	fileMenu->addSeparator();
	fileMenu->addAction(saveAction);
	fileMenu->addAction(saveAsAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);

	QMenu* windowMenu = menuBar()->addMenu("&Window");
	windowMenu->addAction(m_Ui->ToolbarWidget->toggleViewAction());
	windowMenu->addAction(m_Ui->PropertiesWidget->toggleViewAction());

}

void MainWindow::NewFile()
{
	bool okWidth, okHeight;
	int width = QInputDialog::getInt(this, "New Canvas", "Width (1-400):",
		32, 1, 400, 1, &okWidth);
	if (!okWidth) return;

	int height = QInputDialog::getInt(this, "New Canvas", "Height (1-100):",
		16, 1, 100, 1, &okHeight);
	if (!okHeight) return;

	CreateCanvas(width, height);
	m_CurrentFilePath.clear();
	m_bIsModified = false;
	setWindowTitle("Console Texture Editor - Untitled");
}

void MainWindow::CreateCanvas(int32_t width, int32_t height)
{
	m_Scene->clear();
	m_Cells.clear();
	m_Width = width;
	m_Height = height;
	qreal cellSize = 40;

	m_CanvasOriginX = m_CanvasOriginX - (width * cellSize / 2);
	m_CanvasOriginY = m_CanvasOriginY - (height * cellSize / 2);

	QGraphicsRectItem* canvasBase = new QGraphicsRectItem(0, 0, width * cellSize, height * cellSize);
	canvasBase->setZValue(0);
	canvasBase->setPos(m_CanvasOriginX, m_CanvasOriginY);
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

	m_StatusLabel->setText(QString("Canvas: %1x%2").arg(width).arg(height));
}

void MainWindow::OnCanvasClicked(int32_t x, int32_t y, bool shiftPressed)
{
	if (x < 0 || x >= m_Width || y < 0 || y >= m_Height) return;

	m_bIsDrawing = true;
	ApplyCurrentTool(x, y, shiftPressed);
}

void MainWindow::OnCanvasDragged(int32_t x, int32_t y, bool shiftPressed)
{
	if (!m_bIsDrawing || x < 0 || x >= m_Width || y < 0 || y >= m_Height) return;
	ApplyCurrentTool(x, y, shiftPressed);
}

void MainWindow::ApplyCurrentTool(int32_t x, int32_t y, bool shiftPressed)
{
	CellData data = m_Cells[y][x]->GetData();

	switch (m_CurrentTool) {
	case Tool::Brush:
		data.Character = m_CurrentChar;
		break;
	case Tool::Eraser:
		data.Character = ' ';
		break;
	case Tool::Select:
		ShowCellProperties(x, y);
		m_SelectedX = x;
		m_SelectedY = y;
		return;

	case Tool::Paint:
		if (shiftPressed)
		{
			data.BgColor = m_ColorSwatch->Color();
		}
		else
		{
			data.FgColor = m_ColorSwatch->Color();
		}
		break;

	case Tool::Attribute:
		switch (m_CurrentAttribute) {
		case Attribute::Bold:
			data.Bold = !shiftPressed;
			break;
		case Attribute::Dim:
			data.Dim = !shiftPressed;
			break;
		case Attribute::Italic:
			data.Italic = !shiftPressed;
			break;
		case Attribute::Underline:
			data.Underline = !shiftPressed;
			break;
		case Attribute::Blink:
			data.Blink = !shiftPressed;
			break;
		case Attribute::Inverse:
			data.Inverse = !shiftPressed;
			break;
		case Attribute::DefaultFg:
			data.DefaultFg = !shiftPressed;
			break;
		case Attribute::DefaultBg:
			data.DefaultBg = !shiftPressed;
			break;
		}
		break;
	}

	m_Cells[y][x]->UpdateCell(data);
	m_bIsModified = true;
}

void MainWindow::ShowCellProperties(int32_t x, int32_t y)
{
	CellData data = m_Cells[y][x]->GetData();
	QString info = QString("Cell [%1, %2]\n").arg(x).arg(y);
	info += QString("Char: '%1'\n").arg(data.Character);
	info += QString("FG: RGB(%1,%2,%3)\n").arg(data.FgColor.red())
		.arg(data.FgColor.green())
		.arg(data.FgColor.blue());
	info += QString("BG: RGB(%1,%2,%3)\n").arg(data.BgColor.red())
		.arg(data.BgColor.green())
		.arg(data.BgColor.blue());
	info += "Attributes:\n";
	if (data.Bold) info += "  • Bold\n";
	if (data.Dim) info += "  • Dim\n";
	if (data.Italic) info += "  • Italic\n";
	if (data.Underline) info += "  • Underline\n";
	if (data.Blink) info += "  • Blink\n";
	if (data.Inverse) info += "  • Inverse\n";
	if (data.DefaultFg) info += "  • DefaultFg\n";
	if (data.DefaultBg) info += "  • DefaultBg\n";

	m_SelectedCellInfo->setText(info);
}

void MainWindow::SelectTool(Tool tool)
{
	m_CurrentTool = tool;
	QString toolName;
	switch (tool)
	{
	case Tool::Brush: toolName = "Brush"; break;
	case Tool::Eraser: toolName = "Eraser"; break;
	case Tool::Select: toolName = "Select"; break;
	case Tool::Paint: toolName = "Paint"; break;
	case Tool::Attribute: toolName = "Attribute"; break;
	}
	m_StatusLabel->setText("Current tool: " + toolName);
}

void MainWindow::SelectCharacter(const QString& ch)
{
	if (!ch.isEmpty())
	{
		m_CurrentChar = ch[0];
	}
}

void MainWindow::SelectFgColor()
{
	QColor color = QColorDialog::getColor(m_CurrentFgColor, this, "Select Foreground Color");
	if (color.isValid())
	{
		m_CurrentFgColor = color;
		m_FgColorBtn->setStyleSheet(QString("background-color: %1; color: %2;")
			.arg(color.name())
			.arg(color.lightness() > 128 ? "black" : "white"));
	}
}

void MainWindow::SelectBgColor()
{
	QColor color = QColorDialog::getColor(m_CurrentBgColor, this, "Select Background Color");
	if (color.isValid())
	{
		m_CurrentBgColor = color;
		m_BgColorBtn->setStyleSheet(QString("background-color: %1; color: %2;")
			.arg(color.name())
			.arg(color.lightness() > 128 ? "black" : "white"));
	}
}


void MainWindow::SelectAttribute(Attribute attr)
{
	m_CurrentAttribute = attr;
	QString attrName;
	switch (attr)
	{
	case Attribute::Bold: attrName = "Bold"; break;
	case Attribute::Dim: attrName = "Dim"; break;
	case Attribute::Italic: attrName = "Italic"; break;
	case Attribute::Underline: attrName = "Underline"; break;
	case Attribute::Blink: attrName = "Blink"; break;
	case Attribute::Inverse: attrName = "Inverse"; break;
	case Attribute::DefaultFg: attrName = "DefaultFg"; break;
	case Attribute::DefaultBg: attrName = "DefaultBg"; break;
	}
	m_StatusLabel->setText("Attribute: " + attrName + " (Hold Shift to remove)");
}

void MainWindow::SaveFile()
{
	if (m_CurrentFilePath.isEmpty())
	{
		SaveFileAs();
	}
	else
	{
		SaveToFile(m_CurrentFilePath);
	}
}

void MainWindow::SaveFileAs()
{
	QString filename = QFileDialog::getSaveFileName(this, "Save Texture",
		"", "Texture Files (*.ctx)");
	if (!filename.isEmpty())
	{
		if (SaveToFile(filename))
		{
			m_CurrentFilePath = filename;
			setWindowTitle("Console Texture Editor - " + QFileInfo(filename).fileName());
		}
	}
}

void MainWindow::OpenFile()
{
	QString filename = QFileDialog::getOpenFileName(this, "Open Texture",
		"", "Texture Files (*.ctx)");
	if (!filename.isEmpty())
	{
		if (LoadFromFile(filename))
		{
			m_CurrentFilePath = filename;
			setWindowTitle("Console Texture Editor - " + QFileInfo(filename).fileName());
		}
	}
}

bool MainWindow::SaveToFile(const QString& filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly)) {
		QMessageBox::critical(this, "Error", "Could not save file");
		return false;
	}

	QDataStream out(&file);
	out << quint32(0xCAFEBABE); // Magic number
	out << quint32(m_Width) << quint32(m_Height);

	for (int y = 0; y < m_Height; ++y) {
		for (int x = 0; x < m_Width; ++x) {
			CellData data = m_Cells[y][x]->GetData();
			out << data.Character;
			out << data.FgColor;
			out << data.BgColor;
			out << data.Bold << data.Dim << data.Italic << data.Underline << data.Blink << data.Inverse << data.DefaultFg << data.DefaultBg;
		}
	}

	m_bIsModified = false;
	m_StatusLabel->setText("File saved: " + filename);
	return true;
}

bool MainWindow::LoadFromFile(const QString& filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly)) {
		QMessageBox::critical(this, "Error", "Could not open file");
		return false;
	}

	QDataStream in(&file);
	quint32 magic, width, height;
	in >> magic;

	if (magic != 0xCAFEBABE) {
		QMessageBox::critical(this, "Error", "Invalid file format");
		return false;
	}

	in >> width >> height;
	CreateCanvas(width, height);

	for (int y = 0; y < m_Height; ++y) {
		for (int x = 0; x < m_Width; ++x) {
			CellData data;
			in >> data.Character;
			in >> data.FgColor >> data.BgColor;
			in >> data.Bold >> data.Dim >> data.Italic >> data.Underline >> data.Blink >> data.Inverse >> data.DefaultFg >> data.DefaultBg;
			m_Cells[y][x]->UpdateCell(data);
		}
	}

	m_bIsModified = false;
	m_StatusLabel->setText("File loaded: " + filename);
	return true;
}