TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    engine.cpp \
    texture.cpp \
    entity.cpp \
    vector2d.cpp \
    timer.cpp \
    camera.cpp \
    spaceship.cpp \
    planet.cpp \
    rocket.cpp \
    asteroidbig.cpp \
    asteroidmiddle.cpp \
    asteroidsmall.cpp \
    asteroid.cpp \
    particle.cpp \
    sellingpoint.cpp \
    animation.cpp \
    level.cpp \
    minimap.cpp \
    xmlhelper.cpp \
    button.cpp \
    state.cpp \
    ingamestate.cpp \
    menustate.cpp

LIBS += -lSDL2 -lSDL2_image -lSDL2_ttf -ltinyxml2

HEADERS += \
    engine.h \
    texture.h \
    entity.h \
    vector2d.h \
    timer.h \
    camera.h \
    spaceship.h \
    planet.h \
    rocket.h \
    asteroidbig.h \
    asteroidmiddle.h \
    asteroidsmall.h \
    asteroid.h \
    particle.h \
    sellingpoint.h \
    animation.h \
    level.h \
    minimap.h \
    xmlhelper.h \
    button.h \
    state.h \
    ingamestate.h \
    menustate.h
