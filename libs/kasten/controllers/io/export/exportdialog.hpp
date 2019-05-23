/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2008,2013 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KASTEN_EXPORTDIALOG_HPP
#define KASTEN_EXPORTDIALOG_HPP

// Qt
#include <QDialog>

namespace Kasten {

class AbstractModelExporterConfigEditor;
class AbstractSelectionView;
class AbstractModelExporter;
class AbstractModelSelection;
class AbstractModel;

class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    ExportDialog(const QString& remoteTypeName, AbstractModelExporterConfigEditor* configEditor,
                 AbstractModelExporter* exporter,
                 QWidget* parent = nullptr);

    ~ExportDialog() override;

public:
    void setData(AbstractModel* model, const AbstractModelSelection* selection);

Q_SIGNALS:
    void exportAccepted(AbstractModelExporter* exporter, const AbstractModelSelection* selection);

private Q_SLOTS:
    void onFinished(int result);

private:
    AbstractModelExporterConfigEditor* const mConfigEditor;
    AbstractModelExporter* const m_exporter;
    AbstractSelectionView* mPreviewView;
    const AbstractModelSelection* m_selection = nullptr;
};

}

#endif
