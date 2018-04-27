#ifndef VEC2SPINBOX_H
#define VEC2SPINBOX_H

#include <QWidget>
#include <QSpinBox>

class Vec2SpinBox : public QWidget
{
	Q_OBJECT
	QDoubleSpinBox* _x;
	QDoubleSpinBox* _y;
public:
	explicit Vec2SpinBox(QWidget *parent = nullptr);

	void setSingleStep( double step );
	void setMinimum( double x, double y );
	void setValue( double x, double y );
	double x() const { return _x->value(); }
	double y() const { return _y->value(); }

signals:
	//void valueChanged(double x, double y);
	void xChanged( double x );
	void yChanged( double y );

public slots:

private slots:
};

#endif // VEC2SPINBOX_H
