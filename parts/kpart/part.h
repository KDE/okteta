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


#ifndef OKTETAPART_H
#define OKTETAPART_H

// Okteta core
#include <kfilebytearraymodel.h>
// KDE
#include <kparts/part.h>

class KToggleAction;
class KSelectAction;
class KAction;

namespace KHEUI
{
class KByteArrayView;
}


class OktetaPart : public KParts::ReadOnlyPart
{
    Q_OBJECT

    friend class OktetaBrowserExtension;

  public:
    OktetaPart( QWidget *parentWidget, QObject *parent,
                bool browserViewWanted );
    virtual ~OktetaPart();


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
    void onSetShowsNonprinting( bool on );
    void onSetResizeStyle( int Style );
    void onToggleOffsetColumn( bool on );
    void onToggleValueCharColumns( int VisibleColunms );

  private Q_SLOTS:
    // used to catch changes in the bytearray widget
    void onSelectionChanged( bool HasSelection );

  private:
    KHEUI::KByteArrayView *view;
    KHECore::KFileByteArrayModel fileByteArray;

    // edit menu
    KAction *copyAction;
    KAction *deselectAction;
    // view menu
    KSelectAction *codingAction;
    KSelectAction *encodingAction;
    KToggleAction *showNonprintingAction;
    // settings menu
    KSelectAction *resizeStyleAction;
    KToggleAction *showOffsetColumnAction;
    KSelectAction *toggleColumnsAction;
};

#endif
