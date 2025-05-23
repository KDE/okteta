/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010, 2011 Alex Richardson <alex.richardson@gmx.de>
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRUCTURESTOOL_HPP
#define KASTEN_STRUCTURESTOOL_HPP

// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
// tool
#include "datatypes/topleveldatainformation.hpp"
// Kasten core
#include <Kasten/AbstractTool>
// Okteta core
#include <Okteta/AddressRange>
// Std
#include <memory>

class QModelIndex;
class QByteArray;

class DataInformation;

namespace Okteta {
class ArrayChangeMetricsList;
class AbstractByteArrayModel;
}

namespace Kasten {
class StructuresManager;

class StructuresTool : public AbstractTool
{
    Q_OBJECT

public:
    StructuresTool();
    ~StructuresTool() override;

public: // AbstractTool API
    // virtual AbstractModel* targetModel() const;
    [[nodiscard]]
    QString title() const override;
    void setTargetModel(AbstractModel* model) override;

public:
    [[nodiscard]]
    QSysInfo::Endian byteOrder() const;
    void setByteOrder(QSysInfo::Endian order);
    [[nodiscard]]
    bool setData(const QVariant& value, int role, DataInformation* item, uint row);
    [[nodiscard]]
    Okteta::AbstractByteArrayModel* byteArrayModel() const;
    [[nodiscard]]
    StructuresManager* manager() const;
    void lockStructure(const QModelIndex& idx);
    void unlockStructure(const QModelIndex& idx);
    [[nodiscard]]
    bool isStructureLocked(const QModelIndex& idx) const;
    /** check if there is any ByteArrayModel available to lock the structure */
    [[nodiscard]]
    bool canStructureBeLocked(const QModelIndex& idx) const;
    [[nodiscard]]
    bool isFileLoaded() const;
    [[nodiscard]]
    QByteArray bytes(const DataInformation* data) const;
    [[nodiscard]]
    QString dataAddressAsString(const DataInformation* data) const;

    void selectBytesInView(const QModelIndex& idx);
    [[nodiscard]]
    bool isStructureListEmpty() const;

    // interface for model
    [[nodiscard]]
    QVariant headerData(int column, int role) const;
    [[nodiscard]]
    int childCount() const;
    [[nodiscard]]
    DataInformation* childAt(int idx) const;
    [[nodiscard]]
    TopLevelDataInformation::List allData() const;

Q_SIGNALS:
    void dataChanged(int row, void* data); // actually a DataInformation*
    void dataCleared();
    void byteOrderChanged();
    void byteArrayModelChanged(Okteta::AbstractByteArrayModel* model);
    /** items are inserted before @p startIndex */
    void childrenAboutToBeInserted(DataInformation* sender, uint startIndex, uint endIndex);
    /** items are inserted before @p startIndex */
    void childrenInserted(const DataInformation* sender, uint startIndex, uint endIndex);
    /** items are removed before @p startIndex */
    void childrenAboutToBeRemoved(DataInformation* sender, uint startIndex, uint endIndex);
    /** items are inserted before @p startIndex */
    void childrenRemoved(const DataInformation* sender, uint startIndex, uint endIndex);
    void isStructureListEmptyChanged(bool isStructureListEmpty);

public Q_SLOTS:
    void setByteOrder(int order);
    void mark(const QModelIndex& idx);
    void unmark(/*const QModelIndex& idx*/);
    void updateData(const Okteta::ArrayChangeMetricsList& list);
    void addChildItem(TopLevelDataInformation* child);
    void setEnabledStructuresInView();
    void validateAllStructures();

private Q_SLOTS:
    void onByteOrderChanged();
    void onCursorPositionChange(Okteta::Address pos);
    void onContentsChange(const Okteta::ArrayChangeMetricsList&);
    void onChildItemDataChanged();

private:
    [[nodiscard]]
    Okteta::Address startAddress(const TopLevelDataInformation* data) const;
    [[nodiscard]]
    Okteta::AddressRange dataRange(const DataInformation* data) const;

private:
    // source
    ByteArrayView* mByteArrayView = nullptr;
    Okteta::AbstractByteArrayModel* mByteArrayModel = nullptr;
    Okteta::Address mCursorIndex = 0;
    Okteta::ArrayChangeMetricsList mArrayChangesWhileWriting;

    // settings
    QSysInfo::Endian mByteOrder;
    std::unique_ptr<StructuresManager> mManager;
    TopLevelDataInformation::List mData;
    TopLevelDataInformation::List mInvalidData;
    bool mWritingData : 1;
    bool mCurrentItemDataChanged : 1;
    bool mIsStructureMarked : 1;
};

}

#endif
