/***************************************************************************
                          khepart.h  -  description
                             -------------------
    begin                : Don Jun 19 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHEPART_H
#define KHEPART_H

// kde specific
#include <kparts/part.h>
// app specific
#include "kbigbuffer.h"

// forward declarations
class KRadioAction;
class KToggleAction;

namespace KHE
{

// forward declarations
class KHexEdit;

/**
 * This is a "Part".  It that does all the real work in a KPart
 * application.
 *
 * @short Main Part
 * @author Friedrich W. H. Kossebau <Friedrich.W.H@Kossebau.de>
 * @version 0.1.0
 */
class KHexEditPart : public KParts::ReadOnlyPart
{
    Q_OBJECT

  public:
    KHexEditPart( QWidget *ParentWidget, const char *WidgetName, QObject *Parent, const char *Name );
    virtual ~KHexEditPart();


  protected: // KParts::ReadOnlyPart API
    virtual bool openFile();

  protected:
    void setupActions();

  protected slots:
    // used to catch changes in the HexEdit widget
    void slotSelectionChanged();
  protected slots: // action slots
    void slotSelectAll();
    void slotUnselect();
    void slotSetCoding();
    void slotSetEncoding();
    void slotSetShowUnprintable();
    void slotSetResizeStyle();

  private:
    KHexEdit *m_HexEdit;
    KBigBuffer m_Wrapping;

    // edit menu
    KAction *CopyAction;
    // view menu
    KRadioAction *HexCodingAction;
    KRadioAction *DecCodingAction;
    KRadioAction *OctCodingAction;
    KRadioAction *BinCodingAction;
    KRadioAction *LocalEncodingAction;
    KRadioAction *AsciiEncodingAction;
    KToggleAction *ShowUnprintableAction;
    // settings menu
    KRadioAction *NoResizeAction;
    KRadioAction *LockGroupsAction;
    KRadioAction *FullSizeUsageAction;
};

}

#endif
