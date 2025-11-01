#pragma once
#include <QGraphicsRectItem>
#include "SaveDialog.h"

struct CellData
{
	QChar Character = ' ';
	QString DisplayText = "";
	QColor FgColor = Qt::white;
	QColor BgColor = Qt::black;
	bool Bold = false;
	bool Dim = false;
	bool Italic = false;
	bool Underline = false;
	bool Blink = false;
	bool Inverse = false;
	bool DefaultFg = true;
	bool DefaultBg = true;
	bool Transparent = false;

	void Serialize(QDataStream& out, EImageFormat format);
	void Deserialize(QDataStream& in, EImageFormat format);
};

class CellHightlight : public QGraphicsRectItem
{
public:
	CellHightlight(qreal sizeX, qreal sizeY, QGraphicsItem* parent = nullptr);

	void Show();
	void Hide();

protected:
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
private:
	bool m_bVisible;
};


class CellItem : public QGraphicsRectItem
{
public:
	CellItem(int32_t x, int32_t y, qreal size, QGraphicsItem* parent = nullptr);

	void AddHighlight();
	void RemoveHighlight();

	void UpdateCell(const CellData& data);
	CellData GetData() const { return m_Data; }
	qreal GetSize() const { return m_Size; }
	int32_t GetGridX() const { return m_GridX; }
	int32_t GetGridY() const { return m_GridY; }
protected:
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
	CellData m_Data;
	int32_t m_GridX, m_GridY;
	qreal m_Size;
	bool m_bHovered;
};
