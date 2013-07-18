#-------------------------------------------------
#
# Project created by QtCreator 2012-09-05T18:04:31
#
#-------------------------------------------------

QT       += core gui opengl network testlib

TARGET = knossos
TEMPLATE = app

SOURCES +=\
    mainwindow.cpp \
    eventmodel.cpp \
    client.cpp \
    loader.cpp \
    viewer.cpp \
    remote.cpp \
    skeletonizer.cpp \
    renderer.cpp \
    knossos.cpp \
    coordinate.cpp \
    Hashtable.cpp \
    sleeper.cpp \
    viewport.cpp \
    treeLUT_fallback.cpp \
    widgets/console.cpp \
    widgets/tracingtimewidget.cpp \
    widgets/commentswidget.cpp \
    widgets/commentshortcuts/commentshortcutstab.cpp \
    widgets/commentshortcuts/commentspreferencestab.cpp \
    widgets/zoomandmultireswidget.cpp \
    widgets/datasavingwidget.cpp \
    widgets/navigationwidget.cpp \
    widgets/viewportsettingswidget.cpp \
    widgets/toolswidget.cpp \
    widgets/tools/toolsquicktabwidget.cpp \
    widgets/tools/toolstreestabwidget.cpp \
    widgets/tools/toolsnodestabwidget.cpp \
    widgets/viewportsettings/vpsliceplaneviewportwidget.cpp \
    widgets/viewportsettings/vpskeletonviewportwidget.cpp \
    widgets/viewportsettings/vpgeneraltabwidget.cpp \
    widgets/synchronizationwidget.cpp \
    widgets/splashscreenwidget.cpp \
    widgets/coordinatebarwidget.cpp \
    functions.cpp \
    texturizer.cpp \
    slicer.cpp \
    widgetcontainer.cpp \
    scripting_engine.cpp \
    decorators/skeletonizerdecorator.cpp \
    decorators/mainwindowdecorator.cpp

HEADERS  += mainwindow.h \
    knossos-global.h \
    eventmodel.h \
    client.h \
    loader.h \
    viewer.h \
    remote.h \
    skeletonizer.h \
    renderer.h \
    knossos.h\
    sleeper.h \
    viewport.h \
    widgets/console.h \
    widgets/tracingtimewidget.h \
    widgets/commentswidget.h \
    widgets/commentshortcuts/commentshortcutstab.h \
    widgets/commentshortcuts/commentspreferencestab.h \
    widgets/zoomandmultireswidget.h \
    widgets/datasavingwidget.h \
    widgets/navigationwidget.h \
    widgets/viewportsettingswidget.h \
    widgets/toolswidget.h \
    widgets/tools/toolsquicktabwidget.h \
    widgets/tools/toolstreestabwidget.h \
    widgets/tools/toolsnodestabwidget.h \
    widgets/viewportsettings/vpsliceplaneviewportwidget.h \
    widgets/viewportsettings/vpskeletonviewportwidget.h \
    widgets/viewportsettings/vpgeneraltabwidget.h \
    widgets/synchronizationwidget.h \
    widgets/splashscreenwidget.h \
    widgets/coordinatebarwidget.h \
    functions.h \
    texturizer.h \
    slicer.h \
    GUIConstants.h \
    Plattform.h \
    widgetcontainer.h \     
    scripting_engine.h \
    decorators/skeletonizerdecorator.h \
    decorators/mainwindowdecorator.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    knossos.layout \
    iconv.dll \
    libfreetype-6.dll \
    libxml2.dll \
    pthreadVC2.dll \
    zlib1.dll \
    icon \
    LICENSE \
    Makefile \
    splash \
    knossos.depend \
    knossos.dev \
    default.lut \
    gmon.out \
    knossos.res \
    knossos_private.res \
    knossos.rc \
    knossos_private.rc \
    logo.ico \
    ChangeLog.txt \
    defaultSettings.xml \
    customCursor.xpm \
    config.y \

mac {
    INCLUDEPATH += /usr/include/Python2.7 \
                   /usr/lib/
                   /usr/include
    LIBS += -lxml2 \
            -framework GLUT \
            -framework Python \
            -lPythonQt

}

linux {
    LIBS += -lxml2 \
            -lGL \
            -lGLU \
            -L/usr/lib/i386-linux-gnu/mesa/lGL \

    INCLUDEPATH += /home/knossos/Dokumente/libxml \
                   /usr/include/GL/
}


win32 {
    LIBS += -L$$PWD/../../mingw/lib/ -lglut32 -lxml2
    INCLUDEPATH += $$PWD/../../mingw/include

}

RESOURCES += \
    Resources.qrc
