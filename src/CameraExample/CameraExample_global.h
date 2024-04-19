#ifndef CAMERAEXAMPLE_GLOBAL_H
#define CAMERAEXAMPLE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CAMERAEXAMPLE_LIBRARY)
#define CAMERAEXAMPLE_EXPORT Q_DECL_EXPORT
#else
#define CAMERAEXAMPLE_EXPORT Q_DECL_IMPORT
#endif

#endif // CAMERAEXAMPLE_GLOBAL_H