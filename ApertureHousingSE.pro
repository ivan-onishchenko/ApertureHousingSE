# Connect the necessary Qt modules.
QT += core widgets gui datavisualization 

# Choosing the C++ standard.
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    Calculation.h \
    MainWindow.h  \
    Surface.h

SOURCES += \
    Calculation.cpp \
    Main.cpp        \
    MainWindow.cpp  \
    Surface.cpp

FORMS += \
    MainWindow.ui

RESOURCES += \
    Resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
