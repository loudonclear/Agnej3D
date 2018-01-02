QT += core gui opengl
LIBS += -lGLU

TARGET = collision_debugger
TEMPLATE = app

win32 {
    DEFINES += GLEW_STATIC
    LIBS += -lopengl32
}

INCLUDEPATH += libs libs/glew-1.10.0/include
DEPENDPATH += libs libs/glew-1.10.0/include

SOURCES += main.cpp \
    mainwindow.cpp \
    view.cpp \
    raytracer.cpp \
    triangle.cpp

HEADERS += mainwindow.h \
    view.h \
    raytracer.h \
    triangle.h

FORMS += mainwindow.ui
