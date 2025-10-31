#include "InstantToggleButton.h"

#include <QMouseEvent>
#include <QTimer>

void InstantToggleButton::CloseToggleObject()
{
	HandleToggle();
}

bool InstantToggleButton::HandleToggle()
{
	if (isCheckable())
	{
		if (isChecked())
		{
			setChecked(false);
		}
		else
		{
			setChecked(!isChecked());
			emit toggled(isChecked());
		}
		return true;
	}
	return false;
}

void InstantToggleButton::mousePressEvent(QMouseEvent* event)
{
	if (isCheckable())
	{
		if (!m_ToggleObject)
		{
			// Default instant toggle logic if toggle object isn't set
			setChecked(!isChecked());
			emit toggled(isChecked());
			event->accept();
			return;
		}
		if (HandleToggle())
		{
			event->accept();
			return;
		}

	}
	// Default button behavior if the button isn't set to be checkable
	QPushButton::mousePressEvent(event);

}

void InstantToggleButton::mouseReleaseEvent(QMouseEvent* event)
{
	if (isCheckable())
	{
		event->accept();
		return;
	}
	QPushButton::mouseReleaseEvent(event);
}
