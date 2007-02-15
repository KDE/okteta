/***************************************************************************
                          khepart.h  -  description
                             -------------------
    begin                : Don Jun 19 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
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
// part specific
#include <kfilebytearraymodel.h>

// forward declarations
class KToggleAction;
class KSelectAction;

namespace KHEUI
{
class KByteArrayView;
}

/**
 * This is a "Part".  It that does all the real work in a KPart
 * application.
 *
 * @short Main Part
 * @author Friedrich W. H. Kossebau <kossebau@kde.org>
 * @version 0.1.0
 */
class KHexEditPart : public KParts::ReadOnlyPart
{
    Q_OBJECT

    friend class KHexEditBrowserExtension;

  public:
    KHexEditPart( QWidget *ParentWidget, QObject *Parent,
                  bool BrowserViewWanted );
    virtual ~KHexEditPart();


  protected: // KParts::ReadOnlyPart API
    virtual bool openFile();

  protected:
    void setupActions( bool BrowserViewWanted );
    void fitActionSettings();

  protected Q_SLOTS: // action slots
    void onSelectAll();
    void onUnselect();
    void onSetCoding( int Coding );
    void onSetEncoding( int Encoding );
    void onSetShowUnprintable();
    void onSetResizeStyle( int Style );
    void onToggleOffsetColumn();
    void onToggleValueCharColumns( int VisibleColunms );

  private Q_SLOTS:
    // used to catch changes in the HexEdit widget
    void onSelectionChanged( bool HasSelection );

  private:
    KHEUI::KByteArrayView *HexEdit;
    KHECore::KFileByteArrayModel Wrapping;

    // edit menu
    KAction *CopyAction;
    KAction *DeselectAction;
    // view menu
    KSelectAction *CodingAction;
    KSelectAction *EncodingAction;
    KToggleAction *ShowUnprintableAction;
    // settings menu
    KSelectAction *ResizeStyleAction;
    KToggleAction *ShowOffsetColumnAction;
    KSelectAction *ToggleColumnsAction;
};

#endif
