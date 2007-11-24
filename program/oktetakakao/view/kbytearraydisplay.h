/***************************************************************************
                          kbytearraydisplay.h  -  description
                             -------------------
    begin                : Fri Jun 2 2006
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


#ifndef KBYTEARRAYDISPLAY_H
#define KBYTEARRAYDISPLAY_H


// lib
#include <kbytearrayselection.h>
// Okteta gui
#include <kbytearrayview.h>
// kakao
#include <kabstractview.h>
#include <kizoomable.h> // TODO: able instead of interface? or else?
#include <kidataselectable.h>
#include <kiselecteddatawriteable.h>

class KByteArrayDocument;
class KCursorProxy;


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

  protected Q_SLOTS:
    void onSelectionChange( bool selected );

  protected:
    KHEUI::KByteArrayView *mWidget;
    KByteArrayDocument *mDocument;
    KByteArraySelection mSelection;
//     KCursorProxy *mCursorProxy;
};

#endif
