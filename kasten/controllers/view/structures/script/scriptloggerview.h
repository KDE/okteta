/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2012 Alex Richardson <alex.richardson@gmx.de>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SCRIPTLOGGERVIEW_H_
#define SCRIPTLOGGERVIEW_H_

#include <QWidget>
#include "../datatypes/topleveldatainformation.h"

class QTableView;
class KComboBox;

class ScriptLoggerView : public QWidget
{
Q_OBJECT
    Q_DISABLE_COPY(ScriptLoggerView)
public:
    explicit ScriptLoggerView(const TopLevelDataInformation::List& data, QWidget* parent = 0);
    virtual ~ScriptLoggerView();

protected Q_SLOTS:
    void updateModel(int index);

private:
    KComboBox* mSelector;
    QTableView* mView;
    //QSharedPointers so they remain valid until this widget is deleted
    //even if the backing data has changed
    const TopLevelDataInformation::List mList;
};

#endif /* SCRIPTLOGGERVIEW_H_ */
