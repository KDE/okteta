/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010 Alex Richardson <alex.richardson@gmx.de>
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

#include <KDebug>
// lib
#include "oktetakastencontrollers_export.h"
// tool
#include "structviewpreferences.h"
// Kasten core
#include <abstracttool.h>
// Okteta core
#include <address.h>
#include <arraychangemetricslist.h>
//Qt
#include <QModelIndex>
#include <QFileInfo>
#include <QStringList>

class DataInformation;
class TopLevelDataInformation;
namespace Okteta
{
class CharCodec;
class AbstractByteArrayModel;
}

namespace Kasten
{
class ByteArrayView;
class StructureDefinitionFile;
class StructuresManager;

class OKTETAKASTENCONTROLLERS_EXPORT StructTool: public AbstractTool
{
Q_OBJECT
Q_DISABLE_COPY(StructTool)
public:
    StructTool();
    virtual ~StructTool();
public:
    // AbstractTool API
    // virtual AbstractModel* targetModel() const;
    virtual QString title() const;
    virtual void setTargetModel(AbstractModel* model);

public:
    StructViewPreferences::EnumByteOrder::type byteOrder() const
    {
        return mByteOrder;
    }
    void setByteOrder(StructViewPreferences::EnumByteOrder::type order);
    int columnCount() const;
    bool setData(const QVariant& value, int role, DataInformation* item);
    Okteta::AbstractByteArrayModel* byteArrayModel() const;
    inline StructuresManager* manager() const
    {
        return mManager;
    }
    void lockStructure(QModelIndex idx);
    void unlockStructure(QModelIndex idx);
    bool isStructureLocked(QModelIndex idx) const;
    /** check if there is any ByteArrayModel available to lock the structure */
    bool canStructureBeLocked(QModelIndex idx) const;
Q_SIGNALS:
    void dataChanged(int row, void* data); //actually a DataInformation*
    void dataCleared();
    void byteOrderChanged();
    void cursorIndexChanged();
    void byteArrayModelChanged(bool modelIsValid);

public Q_SLOTS:
    void setByteOrder(int order);
    void mark(const QModelIndex& idx);
    void unmark(/*const QModelIndex& idx*/);
    void updateData(const Okteta::ArrayChangeMetricsList& list =
            Okteta::ArrayChangeMetricsList());
    void addChildItem(TopLevelDataInformation* child);
    void setSelectedStructuresInView();
    void validateAllStructures();

protected Q_SLOTS:
    void onCursorPositionChange(Okteta::Address pos);
    void onContentsChange(const Okteta::ArrayChangeMetricsList&);
    //	void onCharCodecChange(const QString& codecName);
    void onChildItemDataChanged();

protected:
    // source
    ByteArrayView* mByteArrayView;
    Okteta::AbstractByteArrayModel* mByteArrayModel;
    Okteta::Address mCursorIndex;
    //	Okteta::CharCodec* mCharCodec;

    // settings
    StructViewPreferences::EnumByteOrder::type mByteOrder;
    StructuresManager* mManager;
    QList<TopLevelDataInformation*> mData;
    bool mWritingData : 1;
    bool mCurrentItemDataChanged : 1;
public:
    //interface for model
    QVariant headerData(int column, int role);
    int childCount() const;
    DataInformation* childAt(int idx) const;
    Qt::ItemFlags flags(int column, DataInformation* data) const;
    //	QTextCodec* mUtf8Codec; //XXX add utf8 strings sometime
};

inline Okteta::AbstractByteArrayModel* StructTool::byteArrayModel() const
{
    return mByteArrayModel;
}

}
#endif /* STRUCTTOOL_H_ */
