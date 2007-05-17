/***************************************************************************
                          kebcdic1047charcodeckcharcodeciftest.h  -  description
                            -------------------
    begin                : Son Mai 7 2006
    copyright            : (C) 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This library is free software; you can redistribute it and/or         *
*   modify it under the terms of the GNU Library General Public           *
*   License version 2 as published by the Free Software Foundation.       *
*                                                                         *
***************************************************************************/



#ifndef KEBCDIC1047CHARCODECKCHARCODECIFTEST_H
#define KEBCDIC1047CHARCODECKCHARCODECIFTEST_H


// test
#include "kcharcodeciftest.h"


class KEBCDIC1047CharCodecKCharCodecIfTest : public KCharCodecIfTest
{
  protected: // KCharCodecIfTest API
    virtual KHECore::KCharCodec *createCodec();
    virtual void deleteCodec( KHECore::KCharCodec *Codec );
};

#endif
