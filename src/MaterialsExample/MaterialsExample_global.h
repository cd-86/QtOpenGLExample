﻿#ifndef MATERIALSEXAMPLE_GLOBAL_H
#define MATERIALSEXAMPLE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MATERIALSEXAMPLE_LIBRARY)
#define MATERIALSEXAMPLE_EXPORT Q_DECL_EXPORT
#else
#define MATERIALSEXAMPLE_EXPORT Q_DECL_IMPORT
#endif

#endif // MATERIALSEXAMPLE_GLOBAL_H