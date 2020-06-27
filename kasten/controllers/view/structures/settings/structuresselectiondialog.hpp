/*
 This file is part of the Okteta Kasten Framework, made within the KDE community.

 SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>
 SPDX-FileCopyrightText: 2009, 2012 Alex Richardson <alex.richardson@gmx.de>

 SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KASTEN_STRUCTURESSELECTIONDIALOG_HPP
#define KASTEN_STRUCTURESSELECTIONDIALOG_HPP

// Qt
#include <QDialog>

namespace Kasten {
class StructuresTool;
}
class StructureAddRemoveWidget;
class QStringList;

class StructuresSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StructuresSelectionDialog(const QStringList& selected, Kasten::StructuresTool* tool,
                                       QWidget* parent = nullptr);

Q_SIGNALS:
    void structuresAccepted(const QStringList& structures);

private Q_SLOTS:
    void onFinished(int result);

private:
    StructureAddRemoveWidget* m_structureAddRemoveWidget;
};

#endif

