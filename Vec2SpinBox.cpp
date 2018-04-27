#include <QHBoxLayout>
#include "Vec2SpinBox.h"

Vec2SpinBox::Vec2SpinBox(QWidget *parent)
        : QWidget(parent),
          _x{ new QDoubleSpinBox(this) },
          _y{ new QDoubleSpinBox(this) }
{
	auto layout = new QHBoxLayout( this );
	layout->setMargin(0);
	setLayout( layout );
	layout->addWidget(_x);
	layout->addWidget(_y);
	connect( _x, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &Vec2SpinBox::xChanged );
	connect( _y, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &Vec2SpinBox::yChanged );
}

void Vec2SpinBox::setSingleStep(double step)
{
	_x->setSingleStep(step);
	_y->setSingleStep(step);
}

void Vec2SpinBox::setMinimum(double x, double y)
{
	_x->setMinimum(x);
	_y->setMinimum(y);
}

void Vec2SpinBox::setValue(double x, double y)
{
	_x->setValue(x);
	_y->setValue(y);
}
