/*
    This file is part of the Okteta KPart module, part of the KDE project.

    Copyright 2003,2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

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
class ByteArrayColumnView;
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
    KHEUI::ByteArrayColumnView *view;
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
