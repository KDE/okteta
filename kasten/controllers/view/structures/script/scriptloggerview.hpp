/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SCRIPTLOGGERVIEW_HPP
#define KASTEN_SCRIPTLOGGERVIEW_HPP

#include <QWidget>
#include "../datatypes/topleveldatainformation.hpp"

class QTableView;
class KComboBox;

class ScriptLoggerView : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(ScriptLoggerView)

public:
    explicit ScriptLoggerView(const TopLevelDataInformation::List& data, QWidget* parent = nullptr);
    ~ScriptLoggerView() override;

private Q_SLOTS:
    void updateModel(int index);

private:
    KComboBox* mSelector;
    QTableView* mView;
    // QSharedPointers so they remain valid until this widget is deleted
    // even if the backing data has changed
    const TopLevelDataInformation::List mList;
};

#endif /* KASTEN_SCRIPTLOGGERVIEW_HPP */
