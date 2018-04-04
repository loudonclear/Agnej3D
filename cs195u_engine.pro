QT += core gui opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cs195u_engine
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++14
CONFIG += c++14

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

SOURCES += \
    libs/glew-1.10.0/src/glew.c \
    src/main.cpp \
    src/mainwindow.cpp \
    src/view.cpp \
    src/viewformat.cpp \
    src/engine/graphics/ResourceLoader.cpp \
    src/engine/graphics/FBO.cpp \
    src/engine/graphics/IBO.cpp \
    src/engine/graphics/Texture.cpp \
    src/engine/graphics/Texture1D.cpp \
    src/engine/graphics/Texture2D.cpp \
    src/engine/graphics/Texture3D.cpp \
    src/engine/graphics/VAO.cpp \
    src/engine/graphics/VBO.cpp \
    src/engine/graphics/VBOAttribMarker.cpp \
    src/engine/graphics/Font.cpp \
    src/engine/graphics/DepthBuffer.cpp \
    src/engine/graphics/RenderBuffer.cpp \
    src/engine/graphics/GraphicsDebug.cpp \
    src/engine/graphics/Shader.cpp \
    src/engine/graphics/Camera.cpp \
    src/engine/graphics/Shape.cpp \
    src/engine/graphics/Graphics.cpp \
    src/engine/graphics/Material.cpp \
    src/engine/graphics/Light.cpp \
    src/engine/Application.cpp \
    src/engine/Screen.cpp \
    src/engine/input/Input.cpp \
    src/engine/world/World.cpp \
    src/engine/world/GameObject.cpp \
    src/engine/components/Transform.cpp \
    src/engine/components/GraphicsComponent.cpp \
    src/engine/systems/GraphicsSystem.cpp \
    src/engine/components/GraphicsShapeComponent.cpp \
    src/engine/systems/TimingSystem.cpp \
    src/engine/physics/Collision.cpp \
    src/engine/physics/RigidBody.cpp \
    src/engine/physics/ShapeCollider.cpp \
    src/engine/physics/CylinderCollider.cpp \
    src/engine/physics/BoxCollider.cpp \
    src/engine/physics/SphereCollider.cpp \
    src/engine/voxel/Chunk.cpp \
    src/engine/voxel/PerlinNoise.cpp \
    src/engine/components/InputComponent.cpp \
    src/engine/systems/InputSystem.cpp \
    src/engine/systems/CollisionSystem.cpp \
    src/engine/components/SoundComponent.cpp \
    src/engine/voxel/ChunkSystem.cpp \
    src/engine/components/UIComponent.cpp \
    src/engine/components/OrthographicUIComponent.cpp \
    src/engine/components/HUDComponent.cpp \
    src/engine/components/Button.cpp \
    src/engine/graphics/Obj.cpp \
    src/platformer/PlatformerApplication.cpp \
    src/platformer/PlatformerScreen.cpp \
    src/platformer/PlatformerWorld.cpp \
    src/platformer/Player.cpp \
    src/platformer/PlayerMoveComponent.cpp \
    src/engine/physics/TriangleCollider.cpp \
    src/engine/systems/OBJCollisionSystem.cpp \
    src/platformer/PlatformerMenuScreen.cpp \
    src/platformer/MenuWorld.cpp \
    src/platformer/MenuButton.cpp \
    src/engine/world/NavMesh.cpp

HEADERS += \
    libs/glew-1.10.0/include/GL/glew.h \
    libs/stb/stb_rect_pack.h \
    libs/stb/stb_truetype.h \
    src/mainwindow.h \
    src/view.h \
    src/viewformat.h \
    src/engine/util/CommonIncludes.h \
    src/engine/graphics/ResourceLoader.h \
    src/engine/graphics/CylinderData.h \
    src/engine/graphics/SphereData.h \
    src/engine/graphics/FBO.h \
    src/engine/graphics/IBO.h \
    src/engine/graphics/Texture.h \
    src/engine/graphics/Texture1D.h \
    src/engine/graphics/Texture2D.h \
    src/engine/graphics/Texture3D.h \
    src/engine/graphics/VAO.h \
    src/engine/graphics/VBO.h \
    src/engine/graphics/VBOAttribMarker.h \
    src/engine/graphics/CubeData.h \
    src/engine/graphics/Font.h \
    src/engine/graphics/DepthBuffer.h \
    src/engine/graphics/RenderBuffer.h \
    src/engine/graphics/GraphicsDebug.h \
    src/engine/graphics/Shader.h \
    src/engine/graphics/ShaderAttribLocations.h \
    src/engine/graphics/Camera.h \
    src/engine/graphics/Graphics.h \
    src/engine/graphics/Shape.h \
    src/engine/graphics/Material.h \
    src/engine/graphics/Light.h \
    src/engine/graphics/Constants.h \
    src/engine/Application.h \
    src/engine/Screen.h \
    src/engine/input/Input.h \
    src/engine/world/World.h \
    src/engine/world/GameObject.h \
    src/engine/systems/System.h \
    src/engine/components/Component.h \
    src/engine/components/TypeMap.h \
    src/engine/components/Transform.h \
    src/engine/physics/Ray.h \
    src/engine/physics/RaycastResult.h \
    src/engine/components/GraphicsComponent.h \
    src/engine/systems/GraphicsSystem.h \
    src/engine/components/GraphicsShapeComponent.h \
    src/engine/systems/TimingSystem.h \
    src/engine/components/TickComponent.h \
    src/engine/physics/Collision.h \
    src/engine/physics/RigidBody.h \
    src/engine/physics/ShapeCollider.h \
    src/engine/physics/CylinderCollider.h \
    src/engine/physics/BoxCollider.h \
    src/engine/physics/SphereCollider.h \
    src/engine/voxel/Chunk.h \
    src/engine/voxel/PerlinNoise.h \
    src/engine/components/InputComponent.h \
    src/engine/systems/InputSystem.h \
    src/engine/systems/CollisionSystem.h \
    src/engine/components/SoundComponent.h \
    src/engine/voxel/ChunkSystem.h \
    src/engine/components/UIComponent.h \
    src/engine/components/OrthographicUIComponent.h \
    src/engine/components/HUDComponent.h \
    src/engine/components/Button.h \
    src/engine/graphics/Obj.h \
    src/platformer/PlatformerApplication.h \
    src/platformer/PlatformerScreen.h \
    src/platformer/PlatformerWorld.h \
    src/platformer/Player.h \
    src/platformer/PlayerMoveComponent.h \
    src/engine/physics/TriangleCollider.h \
    src/engine/systems/OBJCollisionSystem.h \
    src/platformer/PlatformerMenuScreen.h \
    src/platformer/MenuWorld.h \
    src/platformer/MenuButton.h \
    src/engine/world/NavMesh.h \
    src/engine/world/Graph.h

FORMS += src/mainwindow.ui

RESOURCES += \
    res/shaders/shaders.qrc \
    res/fonts/fonts.qrc \
    res/images/images.qrc \
    res/sounds/sounds.qrc \
    res/platformer/platformer.qrc

OTHER_FILES += \
    res/images/grass.png \

DISTFILES += \
    res/shaders/shader.vert \
    res/shaders/shader.frag \

INCLUDEPATH += src libs glm libs/glew-1.10.0/include
DEPENDPATH += src libs glm libs/glew-1.10.0/include

# Don't add the -pg flag unless you know what you are doing. It makes QThreadPool freeze on Mac OS X
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_WARN_ON -= -Wall
QMAKE_CXXFLAGS_WARN_ON += -Waddress -Warray-bounds -Wc++0x-compat -Wchar-subscripts -Wformat\
                          -Wmain -Wmissing-braces -Wparentheses -Wreorder -Wreturn-type \
                          -Wsequence-point -Wsign-compare -Wstrict-overflow=1 -Wswitch \
                          -Wtrigraphs -Wuninitialized -Wunused-label -Wunused-variable \
                          -Wvolatile-register-var -Wno-extra

QMAKE_CXXFLAGS += -g
