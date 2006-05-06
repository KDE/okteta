/***************************************************************************
                          ktabcontroller.h  -  description
                             -------------------
    begin                : So Dez 5 2004
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


#ifndef KHE_KTABCONTROLLER_H
#define KHE_KTABCONTROLLER_H


// lib specific
#include "kcontroller.h"


namespace KHE
{

class KTabController : public KController
{
  public:
    KTabController( KHexEdit *HE, KController *P );

  public: // API
    virtual bool handleKeyPress( QKeyEvent *KeyEvent );

  public:
    bool tabChangesFocus() const;
    void setTabChangesFocus( bool TCF );

  protected:
    /** flag if tab key should be ignored */
    bool TabChangesFocus:1;
};


inline bool KTabController::tabChangesFocus()    const { return TabChangesFocus; }
inline void KTabController::setTabChangesFocus( bool TCF ) { TabChangesFocus = TCF; }

}

#endif
