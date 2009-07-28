/*
    This file is part of the Okteta KPart module, part of the KDE project.

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

// KDE
#include <kparts/part.h>

namespace Kasten {
class KByteArrayDisplay;
class KByteArrayDocument;
class PrintController;
class AbstractXmlGuiController;
class AbstractDocument;
}
class QVBoxLayout;
template <class T> class QList;


class OktetaPart : public KParts::ReadOnlyPart
{
    Q_OBJECT

    friend class OktetaBrowserExtension;

  public:
    OktetaPart( QWidget *parentWidget, QObject *parent,
                bool browserViewWanted );
    virtual ~OktetaPart();

  public:
    Kasten::PrintController* printController() const;

  Q_SIGNALS:
    void hasSelectedDataChanged( bool hasSelectedData );

  protected: // KParts::ReadOnlyPart API
    virtual bool openFile();

  protected:
    void setupActions( bool browserViewWanted );

  protected:
    Kasten::KByteArrayDisplay* byteArrayView() const;

  protected Q_SLOTS:
    void onDocumentLoaded( Kasten::AbstractDocument* document );

  private:
    QVBoxLayout* mLayout;

    Kasten::KByteArrayDocument* mDocument;
    Kasten::KByteArrayDisplay* mDisplay;

    Kasten::PrintController* mPrintController;
    QList<Kasten::AbstractXmlGuiController*> mControllers;
};


inline Kasten::KByteArrayDisplay* OktetaPart::byteArrayView() const { return mDisplay; }

#endif
