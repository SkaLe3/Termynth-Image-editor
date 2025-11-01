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
#include "Attribute.h"
#include "SaveDialog.h"

#include <cstdint>

class CanvasView;
class CellItem;
class ColorSwatch;
class ToolPanelDock;
class ToolPropertiesPanelDock;
class TitleBarWidget;
class CellHightlight;




class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

public slots:
	void OnNewFile();
	void OnOpenFile();
	void OnSaveFile();
	void OnSaveFileAs();
	void OnCanvasClicked(int32_t x, int32_t y, bool shiftPressed);
	void OnCanvasDragged(int32_t x, int32_t y, bool shiftPressed);
	void OnToolSelected(ETool tool);
	void OnSelectedChar(QChar ch);
	void OnApplyToAll(QChar ch, bool bInvert);
	void OnAddRemoveAttributeToSet(EAttribute attr, bool bSelected);
	void OnTransparencySelected(bool bSelected);

protected:
	void paintEvent(QPaintEvent* event) override;
	void closeEvent(QCloseEvent* event) override;

private:
	void SetupUI();
	void SetupMenuBar();
	void CreateCanvas(int32_t width, int32_t height);
	void ApplyCurrentTool(int32_t x, int32_t y, bool shiftPressed);
	bool LoadFromFile(const QString& filename);
	bool SaveToFile(const QString& filename, EImageFormat format);

	void SetupTheme();

private:
	// UI Components
	TitleBarWidget* m_TitleBar;
	QGraphicsScene* m_Scene;
	CanvasView* m_View;
	ToolPanelDock* m_ToolPanel;
	ToolPropertiesPanelDock* m_ToolPropertiesPanel;
	QLabel* m_StatusLabel;

	// Canvas data
	int32_t m_Width;
	int32_t m_Height;
	QVector<QVector<CellItem*>> m_Cells;
	CellHightlight* m_CellHightlight;

	// Current tool state
	ETool m_CurrentTool;
	QChar m_CurrentChar;
	uint8_t m_CurrentAttributeSet;
	bool m_bCurrentTransparency;

	// File handling
	QString m_CurrentFilePath;
	EImageFormat m_CurrentImageFormat;
	bool m_bIsModified;

};