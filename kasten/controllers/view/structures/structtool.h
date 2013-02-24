/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010, 2011 Alex Richardson <alex.richardson@gmx.de>
 *   Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>
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
#ifndef STRUCTTOOL_H_
#define STRUCTTOOL_H_

// lib
#include "oktetakastencontrollers_export.h"
// tool
#include "structviewpreferences.h"
#include "datatypes/topleveldatainformation.h"
// Kasten core
#include <abstracttool.h>
// Okteta core
#include <address.h>
//Qt
class QModelIndex;

class DataInformation;
namespace Okteta
{
class ArrayChangeMetricsList;
class AbstractByteArrayModel;
}

namespace Kasten2
{
class StructuresManager;
class StructToolPrivate;

class OKTETAKASTENCONTROLLERS_EXPORT StructTool : public AbstractTool
{
Q_OBJECT
    Q_DISABLE_COPY(StructTool)

public:
    StructTool();
    virtual ~StructTool();

    // AbstractTool API
    // virtual AbstractModel* targetModel() const;
    virtual QString title() const;
    virtual void setTargetModel(AbstractModel* model);

public:
    QSysInfo::Endian byteOrder() const;
    void setByteOrder(QSysInfo::Endian order);
    KDE_DEPRECATED void setByteOrder(StructViewPreferences::EnumByteOrder::type order);
    KDE_DEPRECATED int columnCount() const;
    bool setData(const QVariant& value, int role, DataInformation* item, uint row);
    Okteta::AbstractByteArrayModel* byteArrayModel() const;
    StructuresManager* manager() const;
    void lockStructure(const QModelIndex& idx);
    void unlockStructure(const QModelIndex& idx);
    bool isStructureLocked(const QModelIndex& idx) const;
    /** check if there is any ByteArrayModel available to lock the structure */
    bool canStructureBeLocked(const QModelIndex& idx) const;
    bool isFileLoaded() const;

    //interface for model
    QVariant headerData(int column, int role);
    int childCount() const;
    DataInformation* childAt(int idx) const;
    TopLevelDataInformation::List allData() const;

Q_SIGNALS:
    void dataChanged(int row, void* data); //actually a DataInformation*
    void dataCleared();
    void byteOrderChanged();
    void cursorIndexChanged();
    KDE_DEPRECATED void byteArrayModelChanged(bool modelIsValid); //TODO remove this one once BC is broken
    void byteArrayModelChanged(Okteta::AbstractByteArrayModel* model);
    /** items are inserted before @p startIndex */
    void childrenAboutToBeInserted(DataInformation* sender, uint startIndex, uint endIndex);
    /** items are inserted before @p startIndex */
    void childrenInserted(const DataInformation* sender, uint startIndex, uint endIndex);
    /** items are removed before @p startIndex */
    void childrenAboutToBeRemoved(DataInformation* sender, uint startIndex, uint endIndex);
    /** items are inserted before @p startIndex */
    void childrenRemoved(const DataInformation* sender, uint startIndex, uint endIndex);

public Q_SLOTS:
    void setByteOrder(int order);
    void mark(const QModelIndex& idx);
    void unmark(/*const QModelIndex& idx*/);
    void updateData(const Okteta::ArrayChangeMetricsList& list);
    void addChildItem(TopLevelDataInformation* child);
    void setSelectedStructuresInView();
    void validateAllStructures();

protected Q_SLOTS:
    void onByteOrderChanged();
    void onCursorPositionChange(Okteta::Address pos);
    void onContentsChange(const Okteta::ArrayChangeMetricsList&);
    void onChildItemDataChanged();

private:
    Okteta::Address startAddress(const TopLevelDataInformation* data);

protected:
    QScopedPointer<StructToolPrivate> d;
};

}
#endif /* STRUCTTOOL_H_ */
