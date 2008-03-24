/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2006-2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KBYTEARRAYDISPLAY_H
#define KBYTEARRAYDISPLAY_H

// lib
#include <kbytearrayselection.h>
// Kakao gui
#include <kabstractview.h>
#include <kizoomable.h> // TODO: able instead of interface? or else?
#include <kidataselectable.h>
#include <kiselecteddatawriteable.h>

class KByteArrayDocument;
class KCursorProxy;
namespace KHEUI {
class KByteArrayView;
}

class KByteArrayDisplay : public KAbstractView, public KDE::If::Zoomable, public KDE::If::DataSelectable, public KDE::If::SelectedDataWriteable
{
    Q_OBJECT
    Q_INTERFACES(KDE::If::Zoomable KDE::If::DataSelectable KDE::If::SelectedDataWriteable)

  public:
    KByteArrayDisplay( KByteArrayDocument *Document );
    virtual ~KByteArrayDisplay();

  public:
//     KCursorObject *cursor() const;

  public: // KAbstractView API
    virtual QWidget *widget() const;
    virtual KAbstractDocument *document() const;
    virtual QString title() const;

  public: // KDE::If::Zoomable API TODO: make all such methods slots?
    virtual void setZoomLevel( double Level );
    virtual double zoomLevel() const;
  Q_SIGNALS:
    virtual void zoomLevelChanged( double Level );

  public: // KDE::If::DataSelectable API
    virtual void selectAllData( bool selectAll );
    virtual bool hasSelectedData() const;
    virtual QMimeData *copySelectedData() const;
    virtual const KAbstractDocumentSelection *selection() const;
  Q_SIGNALS:
    virtual void hasSelectedDataChanged( bool hasSelectedData );

  public: // KDE::If::SelectedDataWriteable API
    virtual void insertData( const QMimeData *data );
    virtual QMimeData *cutSelectedData();
    virtual void deleteSelectedData();
    virtual bool canReadData( const QMimeData *data ) const;

  protected Q_SLOTS:
    void onSelectionChange( bool selected );

  protected:
    KHEUI::KByteArrayView *mWidget;
    KByteArrayDocument *mDocument;
    KByteArraySelection mSelection;
//     KCursorProxy *mCursorProxy;
};

#endif
