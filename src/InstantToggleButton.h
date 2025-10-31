#pragma once

#include <QPushButton>

class InstantToggleButton : public QPushButton
{
	Q_OBJECT
public:
	using QPushButton::QPushButton;

	void SetToggleObject(QWidget* obj) { m_ToggleObject = obj; }

	void CloseToggleObject();
	bool HandleToggle();
protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

private:
	QWidget* m_ToggleObject = nullptr;
	bool m_bBlockToggleThisFrame = false;
};