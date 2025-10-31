#include "ToolPanelDock.h"

#include <QSizePolicy>
#include <QDebug>
#include <QOverload>
#include <QIcon>
#include <QTimer>

#include "ColorSwatch.h"
#include "DockTitleBar.h"


ToolPanelDock::ToolPanelDock(QWidget* parent /*= nullptr*/)
	: PanelDock("Tools",true, parent)
{
	SetupUI();
	setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
}

QColor ToolPanelDock::GetActiveColor() const
{
	return m_ColorSwatch->Color();
}



void ToolPanelDock::SelectColor(const QColor& color)
{
	m_ColorSwatch->SetColor(color);
}

void ToolPanelDock::ActivateTool(ETool tool)
{
 	QAbstractButton* toolButton = m_ToolGroup->button(static_cast<int32_t>(tool));
	if (toolButton)
	{
		toolButton->click();
	}

}

void ToolPanelDock::SetupUI()
{
	connect(this, &QDockWidget::topLevelChanged, [this](bool floating) {
		if (floating)
		{
			StyleCloseButton(m_CloseButton);
		}
		else
		{
			StyleHideCloseButton(m_CloseButton);
		}
		});


	
	QWidget* content = new QWidget(m_Content);
	m_PanelLayout->addWidget(content);
	m_ToolLayout = new QVBoxLayout(content);
	m_ToolLayout->setContentsMargins(0, 0, 0, 0);
	m_ToolLayout->setSpacing(0);
	content->setLayout(m_ToolLayout);

	QPushButton* selectButton = new QPushButton("Select");
	QPushButton* brushButton = new QPushButton("Brush");
	QPushButton* paintButton = new QPushButton("Paint");
	QPushButton* attrButton = new QPushButton("Attrib");

	selectButton->setCheckable(true);
	brushButton->setCheckable(true);
	paintButton->setCheckable(true);
	attrButton->setCheckable(true);

	m_ToolGroup = new QButtonGroup(this);
	m_ToolGroup->setExclusive(true);
	m_ToolGroup->addButton(selectButton, static_cast<int>(ETool::Select));
	m_ToolGroup->addButton(brushButton, static_cast<int>(ETool::Brush));
	m_ToolGroup->addButton(paintButton, static_cast<int>(ETool::Paint));
	m_ToolGroup->addButton(attrButton, static_cast<int>(ETool::Attribute));

	connect(m_ToolGroup, &QButtonGroup::idClicked,
		this, [this](int id)
		{
			emit ToolSelected(static_cast<ETool>(id));
		});

	m_ColorSwatch = new ColorSwatch();
	connect(m_ColorSwatch, &ColorSwatch::ColorChanged, this, &ToolPanelDock::ColorChanged);

	m_ToolLayout->addWidget(selectButton);
	m_ToolLayout->addWidget(brushButton);
	m_ToolLayout->addWidget(paintButton);
	m_ToolLayout->addWidget(attrButton);
	m_ToolLayout->addSpacing(10);
	m_ToolLayout->addWidget(m_ColorSwatch, 0, Qt::AlignHCenter);
	m_ToolLayout->addSpacing(4);
	m_ToolLayout->addStretch();



	for (QPushButton* b : { selectButton, brushButton, paintButton, attrButton })
	{
		StyleToolButton(b);
	}

}



void ToolPanelDock::StyleToolButton(QPushButton* button)
{
	QString style = R"(
    QPushButton {
        background-color: transparent;
        border: 1px solid transparent;
        border-radius: 4px;
        color: white;
		padding: 4px;
    }
    QPushButton:hover {
        background-color: #404040;
		border: 1px solid #636363;
    }
    QPushButton:checked {
        background-color: #333333;
		border: 1px solid#636363;
    }
    QPushButton:pressed {
        background-color: #333333;
		border: 1px solid #636363;
    }
    )";
	button->setStyleSheet(style);
	button->setFixedSize(40, 32);
}



