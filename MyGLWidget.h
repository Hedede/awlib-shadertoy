#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include <QTimer>

#include "renderer.h"

struct MyGLWidget : QOpenGLWidget {
    MyGLWidget( QWidget* parent )
        : QOpenGLWidget( parent )
    {
        QSurfaceFormat fmt;
        fmt.setVersion( 3, 3 );
        fmt.setRenderableType(QSurfaceFormat::OpenGL);
        fmt.setProfile( QSurfaceFormat::CoreProfile );
        fmt.setSamples( 16 );
        QSurfaceFormat::setDefaultFormat( fmt );
        setFormat( fmt );
        connect(&timer, &QTimer::timeout, this, &MyGLWidget::render);
    }
    void initializeGL() override
    {
        graphics::load();
        graphics::initialize();
        timer.start(16); // Approximately 60 FPS
    }

    void resizeGL(int w, int h) override
    {
        graphics::reshape(w, h);
    }

    void paintGL() override
    {
        graphics::clear();
        graphics::render();
    }

public slots:
    void render()
    {
        graphics::update();
        update();
    }

private:
    QTimer timer;

};

#endif // MYGLWIDGET_H
