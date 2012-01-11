/*
    This file is part of the Okteta KPart module, made within the KDE community.

    Copyright 2003,2007,2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

// Kasten
#include <kastencore.h>
// KDE
#include <KParts/ReadWritePart>

namespace Kasten2 {
class ByteArrayView;
class ByteArrayDocument;
class PrintController;
class AbstractXmlGuiController;
class AbstractDocument;
}
class QVBoxLayout;
template <class T> class QList;


class OktetaPart : public KParts::ReadWritePart
{
    Q_OBJECT

    friend class OktetaBrowserExtension;

  public:
    enum Modus { ReadOnlyModus = 0, BrowserViewModus = 1, ReadWriteModus = 2 };

  public:
    OktetaPart( QObject* parent,
                const KComponentData& componentData,
                Modus modus );

    virtual ~OktetaPart();

  public:
    Kasten2::PrintController* printController() const;

  public: // KParts::ReadWritePart API
    virtual void setReadWrite( bool readWrite = true );

  Q_SIGNALS:
    void hasSelectedDataChanged( bool hasSelectedData );

  protected: // KParts::ReadWritePart API
    virtual bool saveFile();

  protected: // KParts::ReadOnlyPart API
    virtual bool openFile();

  protected:
    void setupActions( bool browserViewWanted );

  protected:
    Kasten2::ByteArrayView* byteArrayView() const;

  protected Q_SLOTS:
    void onDocumentLoaded( Kasten2::AbstractDocument* document );
    void onModified( Kasten2::LocalSyncState state );

  private:
    const Modus mModus;
    QVBoxLayout* mLayout;

    Kasten2::ByteArrayDocument* mDocument;
    Kasten2::ByteArrayView* mByteArrayView;

    Kasten2::PrintController* mPrintController;
    QList<Kasten2::AbstractXmlGuiController*> mControllers;
};


inline Kasten2::ByteArrayView* OktetaPart::byteArrayView() const { return mByteArrayView; }

#endif
