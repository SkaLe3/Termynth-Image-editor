#include "CellItem.h"

#include <QPainter>

CellItem::CellItem(int32_t x, int32_t y, qreal size, QGraphicsItem* parent)
	: QGraphicsRectItem(x* size, y* size, size, size, parent)
	, m_GridX(x), m_GridY(y), m_Size(size)
{
	setAcceptHoverEvents(true);
	setPen(QPen(Qt::gray, 1));
	setParentItem(parent);
}

void CellItem::UpdateCell(const CellData& data)
{
	m_Data = data;
	update();
}

void CellItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	painter->fillRect(rect(), m_Data.BgColor);

	painter->setPen(QPen(Qt::gray, 1));
	painter->drawRect(rect());

	QFont font("Courier New", 16);
	font.setBold(m_Data.Bold);
	font.setItalic(m_Data.Italic);
	font.setUnderline(m_Data.Underline);
	painter->setFont(font);
	painter->setPen(m_Data.FgColor);

	QRectF textRect = rect();
	painter->drawText(textRect, Qt::AlignCenter, QString(m_Data.Character));

	qreal iconSize = 8;
	qreal margin = 2;
	int32_t attrCount = 0;

	if (m_Data.Bold)
	{
		painter->fillRect(QRectF(rect().right() - iconSize - margin,
			rect().top() + margin + attrCount * (iconSize + 1),
			iconSize, iconSize), Qt::red);
		attrCount++;
	}
	if (m_Data.Dim)
	{
		painter->fillRect(QRectF(rect().right() - iconSize - margin,
			rect().top() + margin + attrCount * (iconSize + 1),
			iconSize, iconSize), Qt::magenta);
		attrCount++;
	}
	if (m_Data.Italic)
	{
		painter->fillRect(QRectF(rect().right() - iconSize - margin,
			rect().top() + margin + attrCount * (iconSize + 1),
			iconSize, iconSize), Qt::blue);
		attrCount++;
	}
	if (m_Data.Underline)
	{
		painter->fillRect(QRect(rect().right() - iconSize - margin,
			rect().top() + margin + attrCount * (iconSize + 1),
			iconSize, iconSize), Qt::green);
		attrCount++;
	}
	if (m_Data.Blink)
	{
		painter->fillRect(QRect(rect().right() - iconSize - margin,
			rect().top() + margin + attrCount * (iconSize + 1),
			iconSize, iconSize), Qt::cyan);
		attrCount++;
	}
	if (m_Data.Inverse)
	{
		painter->fillRect(QRect(rect().right() - iconSize - margin,
			rect().top() + margin + attrCount * (iconSize + 1),
			iconSize, iconSize), Qt::darkRed);
		attrCount++;
	}
	if (m_Data.DefaultFg)
	{
		painter->fillRect(QRect(rect().right() - iconSize - margin,
			rect().top() + margin + attrCount * (iconSize + 1),
			iconSize, iconSize), Qt::darkGreen);
		attrCount++;
	}
	if (m_Data.DefaultBg)
	{
		painter->fillRect(QRect(rect().right() - iconSize - margin,
			rect().top() + margin + attrCount * (iconSize + 1),
			iconSize, iconSize), Qt::darkCyan);
		attrCount++;
	}
}
