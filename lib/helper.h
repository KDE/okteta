#ifndef KHEXEDIT_HELPER
#define KHEXEDIT_HELPER

// c specific
#include <ctype.h>
// qt specific
#include <qcolor.h>

// temporary solution until syntax highlighting is implemented
static inline QColor colorForByte( const char Byte )
{
  return ispunct(Byte) ? Qt::red: isprint(Byte) ? Qt::black : Qt::blue;
}

#endif
