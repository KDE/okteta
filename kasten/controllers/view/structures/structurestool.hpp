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

#ifndef STRUCTURESTOOL_HPP
#define STRUCTURESTOOL_HPP

// lib
#include <kasten/okteta/oktetakastencontrollers_export.hpp>
#include <kasten/okteta/bytearrayview.hpp>
// tool
#include "datatypes/topleveldatainformation.hpp"
// Kasten core
#include <kasten/abstracttool.hpp>
// Okteta core
#include <okteta/address.hpp>

class QModelIndex;

class DataInformation;

namespace Okteta {
class ArrayChangeMetricsList;
class AbstractByteArrayModel;
}

namespace Kasten {
class StructuresManager;
class StructuresToolPrivate;

class OKTETAKASTENCONTROLLERS_EXPORT StructuresTool : public AbstractTool
{
    Q_OBJECT

public:
    StructuresTool();
    ~StructuresTool() override;

public: // AbstractTool API
    // virtual AbstractModel* targetModel() const;
    QString title() const override;
    void setTargetModel(AbstractModel* model) override;

public:
    QSysInfo::Endian byteOrder() const;
    void setByteOrder(QSysInfo::Endian order);
    bool setData(const QVariant& value, int role, DataInformation* item, uint row);
    Okteta::AbstractByteArrayModel* byteArrayModel() const;
    StructuresManager* manager() const;
    void lockStructure(const QModelIndex& idx);
    void unlockStructure(const QModelIndex& idx);
    bool isStructureLocked(const QModelIndex& idx) const;
    /** check if there is any ByteArrayModel available to lock the structure */
    bool canStructureBeLocked(const QModelIndex& idx) const;
    bool isFileLoaded() const;

    // interface for model
    QVariant headerData(int column, int role) const;
    int childCount() const;
    DataInformation* childAt(int idx) const;
    TopLevelDataInformation::List allData() const;

Q_SIGNALS:
    void dataChanged(int row, void* data); // actually a DataInformation*
    void dataCleared();
    void byteOrderChanged();
    void cursorIndexChanged();
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

private Q_SLOTS:
    void onByteOrderChanged();
    void onCursorPositionChange(Okteta::Address pos);
    void onContentsChange(const Okteta::ArrayChangeMetricsList&);
    void onChildItemDataChanged();

private:
    Okteta::Address startAddress(const TopLevelDataInformation* data) const;

private:
    // source
    ByteArrayView* mByteArrayView = nullptr;
    Okteta::AbstractByteArrayModel* mByteArrayModel = nullptr;
    Okteta::Address mCursorIndex = 0;

    // settings
    QSysInfo::Endian mByteOrder;
    QScopedPointer<StructuresManager> mManager;
    TopLevelDataInformation::List mData;
    TopLevelDataInformation::List mInvalidData;
    bool mWritingData : 1;
    bool mCurrentItemDataChanged : 1;
};

}

#endif
