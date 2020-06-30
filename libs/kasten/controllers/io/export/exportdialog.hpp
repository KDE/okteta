/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008, 2013 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
    void exportAccepted(Kasten::AbstractModelExporter* exporter,
                        const Kasten::AbstractModelSelection* selection);

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
