HEADERS += \
    source/Silent/ComboBox.h
SOURCES += \
    source/Silent/ComboBox.cpp \
    source/Main.cpp
INCLUDEPATH += \
    ../../cpp/link/link \
	../../cpp/callback_store/atomic_callback_store \
	../../cpp/concurrency/concurrency \
	../../cpp/reactive/observable_pattern
QT += \
    core \ 
    gui \
    widgets