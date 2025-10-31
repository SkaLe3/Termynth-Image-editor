#include "CellItem.h"

#include <QPainter>

CellItem::CellItem(int32_t x, int32_t y, qreal size, QGraphicsItem* parent)
	: QGraphicsRectItem(x* size, y* size * 2.f, size, size * 2.f, parent)
	, m_GridX(x), m_GridY(y), m_Size(size), m_bHovered(false)
{
	setAcceptHoverEvents(true);
	setPen(QPen(Qt::gray, 1));
	setParentItem(parent);
}

void CellItem::AddHighlight()
{
	m_bHovered = true;
}

void CellItem::RemoveHighlight()
{
	m_bHovered = false;
}

void CellItem::UpdateCell(const CellData& data)
{
	m_Data = data;
	if (m_Data.Character == QChar(' '))
	{
		m_Data.DisplayText = "space";
	}
	else
	{
		m_Data.DisplayText = m_Data.Character;
	}
	update();
}

void CellItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	if (!m_Data.Transparent)
	{
		painter->fillRect(rect(), m_Data.BgColor);
	}

	QColor frameColor = Qt::gray;

	painter->setPen(QPen(frameColor, 1));
	painter->drawRect(rect());

	if (m_Data.Transparent)
		return;

	QFont font("Consolas", 24);
	font.setBold(m_Data.Bold);
	font.setItalic(m_Data.Italic);
	font.setUnderline(m_Data.Underline);

	QFont font2("Consolas", 10);
	font2.setBold(m_Data.Bold);
	font2.setItalic(m_Data.Italic);
	font2.setUnderline(m_Data.Underline);

	if (m_Data.DisplayText == QString("space"))
	{
		painter->setFont(font2);
	}
	else
	{
		painter->setFont(font);
	}

	painter->setPen(m_Data.FgColor);

	QRectF textRect = rect();
	painter->drawText(textRect, Qt::AlignCenter, m_Data.DisplayText);

	qreal iconSize = 8;
	qreal margin = 2;
	int32_t attrCount = 0;

	qreal attrStartLeft = rect().left() + margin;
	qreal attrStartTop1 = rect().top() + margin;
	qreal attrStartTop2 = rect().bottom() - margin - iconSize;
	QRectF attrRect;

	auto recalculate_rect = [&attrRect, attrStartTop1, attrStartTop2, &attrCount, iconSize, attrStartLeft]()
		{
			qreal attrStartTop = attrCount >= 4 ? attrStartTop2 : attrStartTop1;
			attrRect = QRectF(attrStartLeft + (attrCount % 4) * (iconSize + 1), attrStartTop, iconSize, iconSize);
		};

	if (m_Data.Bold)
	{
		recalculate_rect();
		painter->fillRect(attrRect, Qt::red);
		attrCount++;
	}
	if (m_Data.Dim)
	{
		recalculate_rect();
		painter->fillRect(attrRect, Qt::magenta);
		attrCount++;
	}
	if (m_Data.Italic)
	{
		recalculate_rect();
		painter->fillRect(attrRect, Qt::blue);
		attrCount++;
	}
	if (m_Data.Underline)
	{
		recalculate_rect();
		painter->fillRect(attrRect, Qt::green);
		attrCount++;
	}
	if (m_Data.Blink)
	{
		recalculate_rect();
		painter->fillRect(attrRect, Qt::cyan);
		attrCount++;
	}
	if (m_Data.Inverse)
	{
		recalculate_rect();
		painter->fillRect(attrRect, Qt::darkRed);
		attrCount++;
	}
	if (m_Data.DefaultFg)
	{
		recalculate_rect();
		painter->fillRect(attrRect, Qt::darkGreen);
		attrCount++;
	}
	if (m_Data.DefaultBg)
	{
		recalculate_rect();
		painter->fillRect(attrRect, Qt::darkCyan);
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

CellHightlight::CellHightlight(qreal sizeX, qreal sizeY, QGraphicsItem* parent /*= nullptr*/)
	: QGraphicsRectItem(0.f,0.f, sizeX, sizeY, parent), m_bVisible(false)
{

}

void CellHightlight::Show()
{
	m_bVisible = true;
}

void CellHightlight::Hide()
{
	m_bVisible = false;
}

void CellHightlight::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	if (!m_bVisible) return;

	QColor frameColor = Qt::gray;
	frameColor = QColor("#FFFFFF");


	painter->setPen(QPen(frameColor, 2));
	painter->drawRect(rect());


}