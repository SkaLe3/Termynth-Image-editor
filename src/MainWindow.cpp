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

#include "ui/ui_MainWindow.h"
#include "CanvasView.h"
#include "CellItem.h"

#include <cstdint>




MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, m_Width(0), m_Height(0)
	, m_CurrentTool(Tool::Fill)
	, m_CurrentChar('█')
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
	QWidget* central = new QWidget(this);
	//QHBoxLayout* mainLayout = new QHBoxLayout(central);


	// Tool panel
	m_ToolPanel = new QWidget();
	QVBoxLayout* toolLayout = new QVBoxLayout(m_ToolPanel);
	m_ToolPanel->setMaximumWidth(300);

	// Tools group
	QGroupBox* toolsGroup = new QGroupBox("Tools");
	QVBoxLayout* toolsLayout = new QVBoxLayout(toolsGroup);

	QPushButton* fillBtn = new QPushButton("Fill Tool (F)");
	QPushButton* selectBtn = new QPushButton("Select Tool (S)");
	QPushButton* fgBtn = new QPushButton("FG Color Tool (G)");
	QPushButton* bgBtn = new QPushButton("BG Color Tool (B)");
	QPushButton* attrBtn = new QPushButton("Attribute Tool (A)");

	connect(fillBtn, &QPushButton::clicked, [this]() { SelectTool(Tool::Fill); });
	connect(selectBtn, &QPushButton::clicked, [this]() { SelectTool(Tool::Select); });
	connect(fgBtn, &QPushButton::clicked, [this]() { SelectTool(Tool::FgColor); });
	connect(bgBtn, &QPushButton::clicked, [this]() { SelectTool(Tool::BgColor); });
	connect(attrBtn, &QPushButton::clicked, [this]() { SelectTool(Tool::Attribute); });

	toolsLayout->addWidget(fillBtn);
	toolsLayout->addWidget(selectBtn);
	toolsLayout->addWidget(fgBtn);
	toolsLayout->addWidget(bgBtn);
	toolsLayout->addWidget(attrBtn);
	toolLayout->addWidget(toolsGroup);
	 
	// Cell info
	m_SelectedCellInfo = new QLabel("No cell selected");
	m_SelectedCellInfo->setWordWrap(true);
	toolLayout->addWidget(m_SelectedCellInfo);

	toolLayout->addStretch();
	//mainLayout->addWidget(m_ToolPanel);



	setCentralWidget(central);

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
	
	SelectTool(Tool::Fill);
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
	case Tool::Fill:
		data.Character = m_CurrentChar;
		break;

	case Tool::Select:
		ShowCellProperties(x, y);
		m_SelectedX = x;
		m_SelectedY = y;
		return;

	case Tool::FgColor:
		data.FgColor = m_CurrentFgColor;
		break;

	case Tool::BgColor:
		data.BgColor = m_CurrentBgColor;
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
	case Tool::Fill: toolName = "Fill"; break;
	case Tool::Select: toolName = "Select"; break;
	case Tool::FgColor: toolName = "FG Color"; break;
	case Tool::BgColor: toolName = "BG Color"; break;
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