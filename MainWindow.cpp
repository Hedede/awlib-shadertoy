#include "MainWindow.h"

#include "MyGLWidget.h"
#include "Vec2SpinBox.h"

#include <QSurfaceFormat>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QFormLayout>
#include <QColorDialog>
#include <QPushButton>
#include <QSpinBox>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QSlider>

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
{
	setMinimumSize( 1280, 720 );

	setCentralWidget( new QWidget( this ) );
	auto layout = new QHBoxLayout;
	centralWidget()->setLayout(layout);

	auto ogl = new MyGLWidget( this );
	layout->addWidget(ogl,5);

	auto input = new QWidget( this );
	auto input_layout = new QFormLayout;
	input->setLayout(input_layout);
	layout->addWidget(input,1);

	auto new_double = [ogl](float& r, double step = 5.0, double min = -100, double max = 100) {
		auto v = new QDoubleSpinBox;
		v->setSingleStep( step );
		v->setDecimals(3);
		v->setMinimum( min );
		v->setMaximum( max );
		v->setValue(r);
		connect( v, qOverload<double>(&QDoubleSpinBox::valueChanged), [&] (double v) { r = v; });
		return v;
	};

	auto new_degree = [ogl](float& r, double step = 5.0) {
		auto v = new QDoubleSpinBox;
		v->setSuffix("°");
		v->setSingleStep( step );
		v->setDecimals(1);
		v->setMinimum( -180 );
		v->setMaximum( 180 );
		v->setWrapping(true);
		v->setValue(r);
		connect( v, qOverload<double>(&QDoubleSpinBox::valueChanged), [&] (double v) { r = v; });
		return v;
	};
	using namespace graphics;

    input_layout->addWidget(new QLabel("<b>Camera pos</b>"));
    input_layout->addRow( "Distance:", new_double(obs.r, 0.1) );
    input_layout->addRow( "Vertical angle:", new_degree(obs.p, 5.0) );
    input_layout->addRow( "Horizontal angle:", new_degree(obs.t, 5.0) );

    input_layout->addWidget(new QLabel("<b>Light pos</b>"));
    input_layout->addRow( "Distance:", new_double(src.r, 0.1, -10000, 10000) );
    input_layout->addRow( "Vertical angle:", new_degree(src.p, 5.0) );
    input_layout->addRow( "Horizontal angle:", new_degree(src.t, 5.0) );

	auto color_btn = new QPushButton;
	color_btn->setStyleSheet("background: white");
    input_layout->addRow( tr("Color:"), color_btn );
	auto dialog = new QColorDialog(color_btn);
	auto on_click = [=] {
		dialog->show();
	};
	auto on_color_changed = [=] {
		auto color = dialog->selectedColor();
		color_btn->setStyleSheet("background: " + color.name());
		light.r = color.redF();
		light.g = color.greenF();
		light.b = color.blueF();
	};
	connect(color_btn, &QPushButton::clicked, on_click);
	connect(dialog, &QColorDialog::colorSelected, on_color_changed);

	auto new_slider = [ogl](float& r, float max) {
		auto l = new QLabel;
		auto v = new QSlider (Qt::Horizontal);
		v->setFixedWidth(100);
		l->setFixedWidth(45);
		v->setMinimum( 0 );
		v->setMaximum( 10000 );
		v->setValue(r/max * 10000);
		l->setText(QString::number(r,'f',3));
		auto onc = [=,&r] (int v) {
			r = float(v)/10000 * max;
			l->setText(QString::number(r,'f',3));
		};
		connect( v, &QSlider::valueChanged, onc);
		connect( v, &QSlider::valueChanged, ogl, &MyGLWidget::render);
		auto lay = new QHBoxLayout;
		lay->addWidget(v);
		lay->addWidget(l);
		return lay;
	};

    input_layout->addWidget(new QLabel("<b>Shader parameters</b>"));
	auto algo = new QComboBox;
    algo->addItem("Flat");
    algo->addItem("Gouraud");
    algo->addItem("Lambert");
    algo->addItem("Phong");
    algo->addItem("Blinn-Phong");
    algo->addItem("Gaussian");
    algo->addItem("Cook-Torrance");
    algo->addItem("Cook-Torrance (2)");
    input_layout->addRow(tr("Shader:"), algo);
    connect( algo, qOverload<int>(&QComboBox::currentIndexChanged), this, [=] (int i) { change_shader(i); ogl->render();} );
	algo->setCurrentIndex(2);
    input_layout->addRow( tr("Power:"),  new_slider(light.p, 10) );
    input_layout->addRow( tr("Attenuation:"), new_slider(light.a, 1) );

    input_layout->addRow( tr("Shininess:"), new_slider(light.s, 100) );

    input_layout->addWidget(new QLabel("<b>Objects</b>"));
	auto chb1 = new QCheckBox;
	chb1->setChecked(true);
    connect(chb1, &QCheckBox::toggled, this, [=] (bool) { toggle_object(2); });
    input_layout->addRow( tr("Show Cylinder:"), chb1);
	auto chb = new QCheckBox;
    connect(chb, &QCheckBox::toggled, this, [=] (bool) { toggle_object(3); });
    input_layout->addRow( tr("Show Plane:"), chb);
}

MainWindow::~MainWindow()
{

}
