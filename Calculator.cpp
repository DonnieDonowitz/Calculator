#include "Calculator.h"

Calculator::Calculator(QWidget *parent)
    : QWidget(parent),
	layout(new QGridLayout),
	display(new QLineEdit("0.0")),
	sumMemory(0.0), 
	sumNow(0.0),
	factorNow(0.0),
	waitingOperand(true)
{
	setObjectName("Calculator");

	display->setReadOnly(true);
	display->setAlignment(Qt::AlignRight);
	display->setMaxLength(15);

	setupButtons();
	setLayout(layout);
	setWindowTitle("CAM Calculator");
	setWindowIcon(QIcon("x64/icon.png"));
}

void Calculator::onDigitClicked()
{
	QToolButton *clicked = qobject_cast<QToolButton*> (sender());
	int val = clicked->text().toInt();
	if (display->text() == "0.0" && val == 0)
	{
		return;
	}
	if (waitingOperand)
	{
		display->clear();
		waitingOperand = false;
	}
	display->setText(display->text() + QString::number(val));
}

void Calculator::onAdditiveClicked()
{
	QToolButton *clickedButton = qobject_cast<QToolButton *>(sender());
	if (!clickedButton)
		return;
	QString clickedOperator = clickedButton->text();
	double operand = display->text().toDouble();

	if (!pendingMulOperation.isEmpty()) {
		if (!calculate(operand, pendingMulOperation)) {
			display->setText("Math Error");
			return;
		}
		display->setText(QString::number(factorNow));
		operand = factorNow;
		factorNow = 0.0;
		pendingMulOperation.clear();
	}

	if (!pendingAddOperation.isEmpty()) {
		if (!calculate(operand, pendingAddOperation)) {
			display->setText("Math Error");
			return;
		}
		display->setText(QString::number(sumNow));
	}
	else {
		sumNow = operand;
	}

	pendingAddOperation = clickedOperator;
	waitingOperand = true;
}

void Calculator::onMultiplicativeClicked()
{
	QToolButton *clickedButton = qobject_cast<QToolButton *>(sender());
	if (!clickedButton)
		return;
	QString clickedOperator = clickedButton->text();
	double operand = display->text().toDouble();

	if (!pendingMulOperation.isEmpty()) {
		if (!calculate(operand, pendingMulOperation)) {
			display->setText("Math Error");
			return;
		}
		display->setText(QString::number(factorNow));
	}
	else {
		factorNow = operand;
	}

	pendingMulOperation = clickedOperator;
	waitingOperand = true;
}

void Calculator::onChangeSignClicked()
{
	QString text = display->text();
	double value = text.toDouble();

	if (value > 0.0) {
		text.prepend(tr("-"));
	}
	else if (value < 0.0) {
		text.remove(0, 1);
	}
	display->setText(text);
}

void Calculator::onClearClicked()
{
	sumNow = 0.0;
	factorNow = 0.0;
	sumMemory = 0.0;
	pendingAddOperation.clear();
	pendingMulOperation.clear();
	display->setText("0.0");
	waitingOperand = true;
}

void Calculator::onMemoryClicked()
{
	display->setText(QString::number(sumMemory));
	waitingOperand = true;
}

void Calculator::onEqualClicked()
{
	double operand = display->text().toDouble();

	if (!pendingMulOperation.isEmpty()) {
		if (!calculate(operand, pendingMulOperation)) {
			display->setText("Math Error");
			return;
		}
		operand = factorNow;
		factorNow = 0.0;
		pendingMulOperation.clear();
	}
	if (!pendingAddOperation.isEmpty()) {
		if (!calculate(operand, pendingAddOperation)) {
			display->setText("Math Error");
			return;
		}
		pendingAddOperation.clear();
	}
	else {
		sumNow = operand;
	}

	display->setText(QString::number(sumNow));
	sumNow = 0.0;
	waitingOperand = true;
}

void Calculator::onPointClicked()
{
	if (waitingOperand)
		display->setText("0");
	if (!display->text().contains('.'))
		display->setText(display->text() + tr("."));
	waitingOperand = false;
}

void Calculator::onAddToMemoryClicked()
{
	onEqualClicked();
	sumMemory += display->text().toDouble();
}

void Calculator::setupButtons()
{
	QToolButton* digits[10];
	for (int i = 0; i < 10; ++i)
	{
		digits[i] = createButton(QString::number(i), SLOT(onDigitClicked()), "digit");
	}
	
	QToolButton* point = createButton(tr("."), SLOT(onPointClicked()), "point");
	QToolButton* changeSign = createButton(tr("\302\261"), SLOT(onChangeSignClicked()), "changeSign");
	QToolButton* clearAll = createButton(tr("AC"), SLOT(onClearClicked()), "clearAll");
	QToolButton* addToMemory = createButton(tr("M+"), SLOT(onAddToMemoryClicked()), "addToMemory");
	QToolButton* memory = createButton(tr("M"), SLOT(onMemoryClicked()), "memory");
	QToolButton* add = createButton(tr("+"), SLOT(onAdditiveClicked()), "add");
	QToolButton* minus = createButton(tr("-"), SLOT(onAdditiveClicked()), "minus");
	QToolButton* mult = createButton(tr("*"), SLOT(onMultiplicativeClicked()), "mult");
	QToolButton* div = createButton(tr("\303\267"), SLOT(onMultiplicativeClicked()), "div");
	QToolButton* equal = createButton(tr("="), SLOT(onEqualClicked()), "equal");

	//First Row
	layout->addWidget(display, 0, 0, 1, 5);

	//Second Row
	layout->addWidget(digits[7], 1, 0);
	layout->addWidget(digits[8], 1, 1);
	layout->addWidget(digits[9], 1, 2);
	layout->addWidget(add, 1, 3);
	layout->addWidget(memory, 1, 4);

	//Third Row
	layout->addWidget(digits[4], 2, 0);
	layout->addWidget(digits[5], 2, 1);
	layout->addWidget(digits[6], 2, 2);
	layout->addWidget(minus, 2, 3);
	layout->addWidget(addToMemory, 2, 4);

	//Fourth Row
	layout->addWidget(digits[3], 3, 0);
	layout->addWidget(digits[2], 3, 1);
	layout->addWidget(digits[1], 3, 2);
	layout->addWidget(mult, 3, 3);
	layout->addWidget(clearAll, 3, 4);

	//Fifth Row
	layout->addWidget(changeSign, 4, 0);
	layout->addWidget(digits[0], 4, 1);
	layout->addWidget(point, 4, 2);
	layout->addWidget(div, 4, 3);
	layout->addWidget(equal, 4, 4);
}

QToolButton* Calculator::createButton(const QString &text, const char *member, const char *name)
{
	QToolButton* b = new QToolButton;
	b->setText(text);
	b->setObjectName(name);
	QSize s;
	s.setHeight(50);
	s.setWidth(50);
	b->setFixedSize(s);
	QObject::connect(b, SIGNAL(clicked()), this, member);
	return b;
}

bool Calculator::calculate(double rightOperand, const QString &pendingOperator)
{
	if (pendingOperator == tr("+")) {
		sumNow += rightOperand;
	}
	else if (pendingOperator == tr("-")) {
		sumNow -= rightOperand;
	}
	else if (pendingOperator == tr("*")) {
		factorNow *= rightOperand;
	}
	else if (pendingOperator == tr("\303\267")) {
		if (rightOperand == 0.0)
			return false;
		factorNow /= rightOperand;
	}
	return true;
}