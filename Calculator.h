#pragma once

#include <QWidget>
#include <QString>
#include <QToolButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QStyle>
#include <QIcon>

class Calculator : public QWidget
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = Q_NULLPTR);

private slots:
	void onDigitClicked();
	void onAdditiveClicked();
	void onMultiplicativeClicked();
	void onChangeSignClicked();
	void onClearClicked();
	void onMemoryClicked();
	void onEqualClicked();
	void onPointClicked();
	void onAddToMemoryClicked();

private:
	QGridLayout *layout;
	QLineEdit *display;
	double sumMemory, sumNow, factorNow;
	QString pendingAddOperation, pendingMulOperation;
	bool waitingOperand;

	void setupButtons();
	QToolButton* createButton(const QString &text, const char *member, const char *name);
	bool calculate(double operand, const QString &pendingOperator);
};
