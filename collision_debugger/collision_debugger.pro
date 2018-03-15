QT += core gui opengl

TARGET = collision_debugger
TEMPLATE = app

unix:!macx {
    LIBS += -lGLU
}
macx {
    QMAKE_CFLAGS_X86_64 += -mmacosx-version-min=10.7
    QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64
    CONFIG += c++11
}
win32 {
    DEFINES += GLEW_STATIC
    LIBS += -lopengl32 -lglu32
}

INCLUDEPATH += libs libs/glew-1.10.0/include
DEPENDPATH += libs libs/glew-1.10.0/include

SOURCES += main.cpp \
    mainwindow.cpp \
    view.cpp \
    raytracer.cpp \
    triangle.cpp \
    libs/glew-1.10.0/src/glew.c \
    viewformat.cpp \
    collision/SphereShape.cpp \
    collision/TriangleShape.cpp \
    collision/Collision.cpp \
    Transform.cpp \
    RigidBody.cpp

HEADERS += mainwindow.h \
    view.h \
    raytracer.h \
    triangle.h \
    libs/glew-1.10.0/include/GL/glew.h \
    viewformat.h \
    collision/CollisionShape.h \
    collision/ConvexShape.h \
    collision/SphereShape.h \
    collision/PolyhedralConvexShape.h \
    collision/TriangleShape.h \
    collision/Collision.h \
    Transform.h \
    Ray.h \
    RigidBody.h \
    RaycastResult.h

FORMS += mainwindow.ui
