/***************************************************************************
                          keditor.h  -  description
                             -------------------
    begin                : Sa Dez 4 2004
    copyright            : (C) 2004 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHE_KEDITOR_H
#define KHE_KEDITOR_H


// lib specific
#include "kcontroller.h"


namespace KHE
{

class KBufferCursor;

class KEditor : public KController
{
  protected:
    enum KEditAction { CharDelete, WordDelete, CharBackspace, WordBackspace };

  protected:
    KEditor( KBufferCursor *BC, KHexEdit *HE, KController *P );

  public: // API
    virtual bool handleKeyPress( QKeyEvent *KeyEvent );

  protected:
    /** executes keyboard Action \a Action. This is normally called by a key event handler. */
    void doEditAction( KEditAction Action );

  protected:
    KBufferCursor *BufferCursor;
};

}

#endif
