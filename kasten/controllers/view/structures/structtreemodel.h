/*
 *   This file is part of the Okteta Kasten module, part of the KDE project.
 *
 *   Copyright 2009 Alex Richardson <alex.richardson@gmx.de>
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

#ifndef STRUCTTREEMODEL_H_
#define STRUCTTREEMODEL_H_

#include <QAbstractItemModel>

class DataInformation;
namespace Kasten
{
class StructTool;

class StructTreeModel: public QAbstractItemModel
{
Q_OBJECT
public:
    //TODO add insertRow functionality
    explicit StructTreeModel(StructTool* tool, QObject* parent = NULL);
    virtual ~StructTreeModel();
    QVariant data(const QModelIndex& index, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role =
            Qt::DisplayRole) const;
    QModelIndex
            index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex& index) const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    void addChild(DataInformation*);
    bool
            setData(const QModelIndex& index, const QVariant& value, int role =
                    Qt::EditRole);
public Q_SLOTS:
    void onToolDataChange()
    {
        emit dataChanged(QModelIndex(), QModelIndex()); //FIXME emit correct ranges
    }
    void onToolDataClear()
    {
        reset();
    }
private:
    StructTool* mTool;
public:

};
}
#endif /* STRUCTTREEMODEL_H_ */
