#ifndef KHEXEDIT_HELPER
#define KHEXEDIT_HELPER

#include <qcolor.h>

static const int DefaultByteSpacingWidth = 1;
static const int GroupSpacingWidth = 3;

static inline QColor colorForByte( const char Byte )
{
  if( ispunct(Byte) )
    return Qt::red;
  else if( isprint(Byte) )
    return Qt::black;
  else
    return Qt::blue;
}

#endif
