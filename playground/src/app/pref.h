/*
 * Copyright (C) 2003 Friedrich W. H.  Kossebau <Friedrich.W.H@Kossebau.de>
 */

#ifndef _KHEXEDIT2PREF_H_
#define _KHEXEDIT2PREF_H_

#include <kdialogbase.h>
#include <qframe.h>

class KHexEdit2PrefPageOne;
class KHexEdit2PrefPageTwo;

class KHexEdit2Preferences : public KDialogBase
{
  Q_OBJECT
  
  public:
    KHexEdit2Preferences();

  private:
    KHexEdit2PrefPageOne *m_pageOne;
    KHexEdit2PrefPageTwo *m_pageTwo;
};


class KHexEdit2PrefPageOne : public QFrame
{
    Q_OBJECT
  public:
    KHexEdit2PrefPageOne( QWidget *parent = 0 );
};


class KHexEdit2PrefPageTwo : public QFrame
{
    Q_OBJECT
  public:
    KHexEdit2PrefPageTwo( QWidget *parent = 0 );
};

#endif // _KHEXEDIT2PREF_H_
