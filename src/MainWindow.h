#pragma once
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QColor>
#include <QString>
#include <QVector>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QDockWidget>

#include "Tool.h"

#include <cstdint>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CanvasView;
class CellItem;
class ColorSwatch;
class ToolPanelDock;
class ToolPropertiesPanelDock;



enum class Attribute
{
	Bold,
	Dim,
	Italic,
	Underline,
	Blink,
	Inverse,
	DefaultFg,
	DefaultBg
};


class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

public slots:
	void NewFile();
	void OpenFile();
	void SaveFile();
	void SaveFileAs();
	void OnCanvasClicked(int32_t x, int32_t y, bool shiftPressed);
	void OnCanvasDragged(int32_t x, int32_t y, bool shiftPressed);
	void SelectTool(ETool tool);
	void SelectCharacter(const QString& ch);
	void SelectFgColor();
	void SelectBgColor();
	void SelectAttribute(Attribute attr);
	void ShowCellProperties(int32_t x, int32_t y);

private:
	void SetupUI();
	void SetupMenuBar();
	void CreateCanvas(int32_t width, int32_t height);
	void UpdateCell(int32_t x, int32_t y);
	void ApplyCurrentTool(int32_t x, int32_t y, bool shiftPressed);
	bool LoadFromFile(const QString& filename);
	bool SaveToFile(const QString& filename);

	void SetupTheme();

private:
	// UI Components
	QGraphicsScene* m_Scene;
	CanvasView* m_View;
	ToolPanelDock* m_ToolPanel;
	ToolPropertiesPanelDock* m_ToolPropertiesPanel;

	QComboBox* m_CharSelector;
	QPushButton* m_FgColorBtn;
	QPushButton* m_BgColorBtn;
	QVector<QPushButton*> m_AttrButtons;
	QLabel* m_StatusLabel;
	QLabel* m_SelectedCellInfo;

	// Canvas data
	int32_t m_Width;
	int32_t m_Height;
	QVector<QVector<CellItem*>> m_Cells;
	int32_t m_CanvasOriginX;
	int32_t m_CanvasOriginY;

	// Current tool state
	ETool m_CurrentTool;
	QChar m_CurrentChar;
	QColor m_CurrentFgColor;
	QColor m_CurrentBgColor;
	Attribute m_CurrentAttribute;

	// File handling
	QString m_CurrentFilePath;
	bool m_bIsModified;

	bool m_bIsDrawing;
	int32_t m_SelectedX;
	int32_t m_SelectedY;
};