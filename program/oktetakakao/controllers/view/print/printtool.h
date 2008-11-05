/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef PRINTTOOL_H
#define PRINTTOOL_H

// Qt
#include <QtCore/QObject>

class KByteArrayDisplay;
class KByteArrayDocument;
class AbstractModel;
namespace KHECore {
class AbstractByteArrayModel;
}

/**
This tool cares for the printing of the byte array to a series of papers.
This is done by creating a series of print commands, which may also
be in form of a pdf document. We don't care for this, it's done
by Qt and the print dialog.

The content is printed into a series of frames. By default there are
a header frame, the content frame and the footer frame on each page.


-> Header printer, footer printer, content printer
-> called by a pageprinter which knows about the layout
-> pageprinter controls the page settings and informs the embedded printer
-> frameprinter return number of frames they need for their content
-> endless frames vs. ending frames
-> vars for each frame: see KWrite
-> 

*/
class PrintTool : public QObject
{
  Q_OBJECT

  public:
    PrintTool();
    virtual ~PrintTool();

  public:
    void setTargetModel( AbstractModel* model );

  public Q_SLOTS:
    void print();

  Q_SIGNALS:
    void viewChanged( bool hasView );

  protected:
    KByteArrayDocument *mDocument;

    KByteArrayDisplay* mByteArrayDisplay;
    KHECore::AbstractByteArrayModel *mByteArrayModel;
};

#endif
