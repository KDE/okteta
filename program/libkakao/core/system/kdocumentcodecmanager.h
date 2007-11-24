/***************************************************************************
                          kdocumentcodecmanager.h  -  description
                             -------------------
    begin                : Thu Nov 22 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef KDOCUMENTEXPORTMANAGER_H
#define KDOCUMENTEXPORTMANAGER_H

// Qt
#include <QtCore/QObject>
#include <QtCore/QList>

class QWidget;
class KAbstractDocumentSelection;
class KAbstractDocumentStreamEncoder;
class KAbstractDocumentStreamDecoder;
class KDocumentManager;

class KDocumentCodecManager : public QObject
{
  Q_OBJECT

  public:
    explicit KDocumentCodecManager( KDocumentManager *manager );
    virtual ~KDocumentCodecManager();

  public:
    // or use the viewmodel here? on what should the export be based?
    void encodeToStream( KAbstractDocumentStreamEncoder *encoder, const KAbstractDocumentSelection *selection );

  public:
    QList<KAbstractDocumentStreamEncoder*> encoderList() const;
    QList<KAbstractDocumentStreamDecoder*> decoderList() const;

  public:
    void setEncoders( QList<KAbstractDocumentStreamEncoder*> &encoderList );
    void setDecoders( QList<KAbstractDocumentStreamDecoder*> &decoderList );
    void setWidget( QWidget *widget );

  protected:
    // unless there is a singleton
    KDocumentManager *mManager;
    // used for dialogs, TODO: create (or use?) global instance for this
    QWidget *mWidget;

    // temporary hack: hard coded codecs for byte arrays
    QList<KAbstractDocumentStreamEncoder*> mEncoderList;
    QList<KAbstractDocumentStreamDecoder*> mDecoderList;
};

#endif
