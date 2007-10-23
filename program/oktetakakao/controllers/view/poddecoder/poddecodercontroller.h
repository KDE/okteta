/***************************************************************************
                          poddecodercontroller.h  -  description
                             -------------------
    begin                : Fri Nov 3 2006
    copyright            : 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef PODDECODERCONTROLLER_H
#define PODDECODERCONTROLLER_H


// kakao
#include <kviewcontroller.h>

class KXmlGuiWindow;
class KViewManager;
class KPrimitiveTypesView;
namespace KHECore {
class KAbstractByteArrayModel;
}
namespace KHEUI {
class KByteArrayView;
}


class PODDecoderController : public KViewController
{
  Q_OBJECT

  public:
    PODDecoderController( KViewManager *ViewManager, KXmlGuiWindow *MW );

  public: // KToolet API
    virtual void setView( KAbstractView *View );

  protected:
    void update();
  protected Q_SLOTS:
    void onCursorPositionChange( int Pos );
    void onContentsChange( int Start, int End );

  protected:
    static const int DecodeDataSize = 8;

  protected:
//     KProgram *Program;
    KXmlGuiWindow *MainWindow;
    KHEUI::KByteArrayView *HexEdit;
    KHECore::KAbstractByteArrayModel *ByteArray;
    int CursorIndex;

    KPrimitiveTypesView *PrimitiveTypesView;
    unsigned char Data[DecodeDataSize];
};

#endif
