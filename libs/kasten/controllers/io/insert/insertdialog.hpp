/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2013 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_INSERTDIALOG_HPP
#define KASTEN_INSERTDIALOG_HPP

// Qt
#include <QDialog>

namespace Kasten {

class AbstractModelDataGeneratorConfigEditor;
class AbstractModelDataGenerator;

class InsertDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InsertDialog(AbstractModelDataGeneratorConfigEditor* configEditor,
                          AbstractModelDataGenerator* generator,
                          QWidget* parent = nullptr);
    ~InsertDialog() override;

Q_SIGNALS:
    void insertAccepted(Kasten::AbstractModelDataGenerator* generator);

private Q_SLOTS:
    void onFinished(int result);

private:
    AbstractModelDataGeneratorConfigEditor* const mConfigEditor;
    AbstractModelDataGenerator* const m_generator;
};

}
#endif
