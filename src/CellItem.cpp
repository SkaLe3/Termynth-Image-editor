#include "CellItem.h"

#include <QPainter>

CellItem::CellItem(int32_t x, int32_t y, qreal size, QGraphicsItem* parent)
	: QGraphicsRectItem(x* size * 0.5f, y* size, size * 0.5f, size, parent)
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

	QFont font("Courier New", 20);
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

void CellData::Serialize(QDataStream& out)
{
	quint8 attributes = 0;
	attributes = (Bold) | (Dim << 1) | (Italic << 2) | (Underline << 3) |
		(Blink << 4) | (Inverse << 5) | (0 << 6) | (0 << 7);

	bool outTransparent = true;
	if (BgColor.alpha() < 128)
		outTransparent = false;

	out << static_cast<quint16>(Character.unicode());
	out << quint8(outTransparent);
	out << attributes;
	out << static_cast<quint8>(FgColor.red()) << static_cast<quint8>(FgColor.green()) << static_cast<quint8>(FgColor.blue());
	out << static_cast<quint8>(BgColor.red()) << static_cast<quint8>(BgColor.green()) << static_cast<quint8>(BgColor.blue());
}

void CellData::Deserialize(QDataStream& in)
{
	quint8 attributes = 0;
	quint16 charCode = 0;
	quint8 fgR, fgG, fgB;
	quint8 bgR, bgG, bgB;
	bool transparent;

	in >> charCode;
	in >> transparent;
	in >> attributes;
	in >> fgR >> fgG >> fgB;
	in >> bgR >> bgG >> bgB;

	Character = QChar(static_cast<char16_t>(charCode));
	FgColor = QColor(fgR, fgG, fgB, 255);
	BgColor = QColor(bgR, bgG, bgB, 255);
	Bold = attributes & (1 << 0);
	Dim = attributes & (1 << 1);
	Italic = attributes & (1 << 2);
	Underline = attributes & (1 << 3);
	Blink = attributes & (1 << 4);
	Inverse = attributes & (1 << 5);
	DefaultFg = attributes & (1 << 6);
	DefaultBg = attributes & (1 << 7);

}
