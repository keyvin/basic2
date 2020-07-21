TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        evaluate.c \
        expression.c \
        functions.c \
        line.c \
        main.c \
        tokens.c \
        variables.c

HEADERS += \
    expression.h \
    globals.h \
    tokens.h \
    variables.h
