/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_PRINTTOOL_HPP
#define KASTEN_PRINTTOOL_HPP

// Qt
#include <QObject>

namespace Okteta {
class AbstractByteArrayModel;
}

class QPrinter;

namespace Kasten {

class ByteArrayView;
class ByteArrayDocument;
class AbstractModel;

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
    ~PrintTool() override;

public:
    void setTargetModel(AbstractModel* model);

public Q_SLOTS:
    void print();

Q_SIGNALS:
    void viewChanged(bool hasView);

private Q_SLOTS:
    void triggerPrint(QPrinter* printer);

private:
    ByteArrayDocument* mDocument = nullptr;

    ByteArrayView* mByteArrayView = nullptr;
    Okteta::AbstractByteArrayModel* mByteArrayModel = nullptr;
};

}

#endif
