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


#ifndef _KHEPART_H_
#define _KHEPART_H_

// kde specific
#include <kparts/part.h>
// app specific
#include "kbigbuffer.h"

// forward declarations
class QWidget;
class QPainter;
class KURL;
class KAction;
class KRadioAction;
namespace KHE { class KHexEdit; }

/**
 * This is a "Part".  It that does all the real work in a KPart
 * application.
 *
 * @short Main Part
 * @author Friedrich W. H.  Kossebau <Friedrich.W.H@Kossebau.de>
 * @version 0.1.0
 */
class KHexEditPart : public KParts::ReadWritePart
{
    Q_OBJECT

  public:
    /**
     * Default constructor
     */
    KHexEditPart( QWidget *ParentWidget, const char *WidgetName, QObject *Parent, const char *Name );

    /**
     * Destructor
     */
    virtual ~KHexEditPart();

    /**
     * This is a virtual function inherited from KParts::ReadWritePart.
     * A shell will use this to inform this Part if it should act
     * read-only
     */
    virtual void setReadWrite( bool rw );

    /**
     * Reimplemented to disable and enable Save action
     */
    virtual void setModified( bool modified );


  protected:
    /**
     * This must be implemented by each part
     */
    virtual bool openFile();

    /**
     * This must be implemented by each read-write part
     */
    virtual bool saveFile();


  protected:
    void setupActions();


  protected slots:
    void fileOpen();
    void fileSaveAs();
    void slotSelectionChanged();
    void selectAll();
    void unselect();
    void slotSetCoding();


  private:
    KHE::KHexEdit *m_HexEdit;
    char* m_Buffer;
    KHE::KBigBuffer m_Wrapping;

    KAction *SaveAction;
    KAction *CutAction;
    KAction *CopyAction;

    KRadioAction *HexCodingAction;
    KRadioAction *DecCodingAction;
    KRadioAction *OctCodingAction;
    KRadioAction *BinCodingAction;
};

#endif
