

PROJECT_FILE_NAME = $$replace(_PRO_FILE_, $$_PRO_FILE_PWD_/, )
PROJECT_NAME      = $$replace(PROJECT_FILE_NAME, .pro, )
TARGETX = x32

win32:{
contains(QMAKE_TARGET.arch, x86_64):{
	 message("Platform x64")
	 CONFIG += target_x86_64
	 TARGETX = x64
} else {
	 message("Platform x86")
	 CONFIG += target_x86
}
}
                   
               
#paths
INCLUDE_PATH = $$DEV_ROOT/src/_Include
EXTS         = $$DEV_ROOT/ext
BINS         = $$DEV_ROOT/bin

target_x86_64:{
	 EXT_LIB      = $$EXTS/lib64
	 BINS = $$BINS"64"
} else {
	 EXT_LIB  = $$EXTS/lib
}

EXT_INCLUDE  = $$EXTS/include

##########################
# vcproj generation
contains(TEMPLATE, lib)|contains(TEMPLATE, app) {
    CONFIG -= debug_and_release
    CONFIG -= debug_and_release_target
} else:contains(TEMPLATE, vclib)|contains(TEMPLATE, vcapp) {
    CONFIG += debug_and_release
}

##########################
# debug/release management
CONFIG(debug,debug|release){
    BUILD_PATH = $$BINS/debug
    CONFIG    += debug
    CONFIG    -= release

    DEBUG_CFG   = True
    PATH_SUFFIX = debug

    unix:QMAKE_CXXFLAGS += -O0
    win32:QMAKE_CXXFLAGS += -Od

    INCLUDEPATH += $$EXT_INCLUDE/vld
    LIBS += -L$$EXT_LIB/vld

    message("Debug Configuration")
} else {
    BUILD_PATH = $$BINS/release
    CONFIG    += release
    CONFIG    -= debug

    RELEASE_CFG = True
    PATH_SUFFIX = release

    unix:QMAKE_CXXFLAGS += -O2
    win32:QMAKE_CXXFLAGS += -Ob2 -Oi -Ot /Oy- -fp:fast -fp:except-

    DEFINES *= NDEBUG
    win32:QMAKE_LFLAGS *= /DEBUG # for generating PDB files

    message("Release Configuration")
}

win32:{
#DEFINES -= UNICODE
DEFINES += DISABLE_ROBUST_COMPUTATIONS
DEFINES += BOOST_ALL_NO_LIB
DEFINES += CG_PRIMITIVES
DEFINES += QT
}

MISC_PATH = $$BUILD_PATH/$$PROJECT_NAME

#############
# build paths
OUT_PWD     = $$MISC_PATH
DESTDIR     = $$BUILD_PATH
OBJECTS_DIR = $$MISC_PATH
MOC_DIR     = $$MISC_PATH
UI_DIR      = $$MISC_PATH
RCC_DIR     = $$MISC_PATH


######################
# common configuration
CONFIG *= precompile_header
CONFIG -= qt qt_no_framework
CONFIG -= flat
CONFIG += grouped

PRECOMPILED_HEADER = precompiled.h
DEFINES += precompile_header

#LIBS += -Wl,--start-group -lasync_services -llogger -lalloc -Wl,--end-group

win32{
    QMAKE_CXXFLAGS -= -Zc:wchar_t-
    QMAKE_CXXFLAGS += -Zc:wchar_t

#    equals(RELEASE_CFG, True):QMAKE_CXXFLAGS += $$QMAKE_CXXFLAGS_MT_DLL
#    equals(DEBUG_CFG  , True):QMAKE_CXXFLAGS += $$QMAKE_CXXFLAGS_MT_DLLDBG

    #correct versions for Windows 7
    QMAKE_CXXFLAGS += -DWINVER=0x0601
    QMAKE_CXXFLAGS += -D_WIN32_WINNT=0x0601
    QMAKE_CXXFLAGS += -DWIN32_LEAN_AND_MEAN

    # non dll-interface class ...
    QMAKE_CXXFLAGS += /wd4275
    # ... class 'boost::shared_ptr<T>' needs to have dll-interface to be used by clients of struct ...
    QMAKE_CXXFLAGS += /wd4251
    # ... 'this' : used in base member initializer list
    QMAKE_CXXFLAGS += /wd4355
    
    QMAKE_CXXFLAGS += /wd4996
    

    QMAKE_CXXFLAGS += -DNOMINMAX

    QMAKE_CXXFLAGS += /MP8
    QMAKE_CXXFLAGS -= -Zm200
    QMAKE_CXXFLAGS *= -Zm512

    LIBS += /NODEFAULTLIB:libcmt.lib
}

#############
# include paths

INCLUDEPATH += ./
INCLUDEPATH += $$SOLUTION_ROOT/ext/pugixml-1.4/src            \
               $$(OSG_DIR)/3rdparty/include                   \
               $$(OSG_DIR)/OpenSceneGraph-3.2.1/build/include \
               $$(OSG_DIR)/OpenSceneGraph-3.2.1/include       \

INCLUDEPATH += \
               $$SOLUTION_ROOT/utils                          \
               $$SOLUTION_ROOT/_include                       \
               $$SOLUTION_ROOT/_include/objects               \
               $$SOLUTION_ROOT/ext                            \
               $$SOLUTION_ROOT/core                           \
               $$SOLUTION_ROOT/objects   			   
			   
#############
# lib paths

LIBS += -L$$(BULLET_DIR)/lib
win32:LIBS += -L$$(BULLET_DIR)/build/lib/$$PATH_SUFFIX
LIBS += -L$$EXT_LIB/boost
LIBS += -L$$EXT_LIB/qt/$$PATH_SUFFIX
LIBS += -L$$(OSG_DIR)/OpenSceneGraph-3.2.1/build/lib
LIBS += -L$$BINS/$$PATH_SUFFIX
LIBS += -L$$PWD/ext/pugixml-1.4/scripts/vs2010/$$TARGETX
LIBS += -L$$(OSG_DIR)/SPARK-1.5.5/lib/vc2008/static
LIBS += -L$$(OSG_DIR)/3rdparty/lib
LIBS += -L$$SIMEX_DIR_FULL/ext/lib/tinyxml2/$$PATH_SUFFIX
LIBS += -L$$SIMEX_DIR_FULL/bin/$$PATH_SUFFIX


CONFIG(debug,debug|release){
   LIBS += -lpugixmld -ltinyxml2 -lnet_layer -lfms -llogger -lasync_services -lalloc -lmeteo
} else {
   LIBS += -lpugixml -ltinyxml2 -lnet_layer -lfms -llogger -lasync_services -lalloc -lmeteo
}		   
			   