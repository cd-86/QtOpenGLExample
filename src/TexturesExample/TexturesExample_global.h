#ifndef TEXTURESEXAMPLE_GLOBAL_H
#define TEXTURESEXAMPLE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TEXTURESEXAMPLE_LIBRARY)
#define TEXTURESEXAMPLE_EXPORT Q_DECL_EXPORT
#else
#define TEXTURESEXAMPLE_EXPORT Q_DECL_IMPORT
#endif

#endif // TEXTURESEXAMPLE_GLOBAL_H