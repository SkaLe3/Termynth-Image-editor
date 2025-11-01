#pragma once

#include <QDialog>

class QLineEdit;
class QPushButton;
class QComboBox;

enum class EImageFormat : uint8_t
{
	DefaultText = 0,
	AttributesText,
	ColoredText,
	AttributesColoredText
};

class SaveDialog : public QDialog
{
	Q_OBJECT
public:
	explicit SaveDialog(QWidget* parent = nullptr);

	QString FilePath() const;
	EImageFormat SelectedFormat() const;

private slots:
	void OnSaveClicked();

private:
	void StyleDialog();

private:
	QComboBox* m_FormatCombo;
	QPushButton* m_SaveButton;
	QPushButton* m_CancelButton;
	QString m_Filename;
};