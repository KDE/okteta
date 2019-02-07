/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef CHECKSUMTOOL_HPP
#define CHECKSUMTOOL_HPP

// lib
#include <config-qca2.hpp>
// Kasten core
#include <Kasten/AbstractTool>
// Okteta core
#include <Okteta/AddressRange>
// Qt
#include <QVector>

class AbstractByteArrayChecksumParameterSet;
class AbstractByteArrayChecksumAlgorithm;

namespace Okteta {
class AbstractByteArrayModel;
}
namespace QCA {
class Initializer;
}

namespace Kasten {

class ByteArrayView;

class ChecksumTool : public AbstractTool
{
    Q_OBJECT

public:
    ChecksumTool();
    ~ChecksumTool() override;

public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    QString title() const override;

    void setTargetModel(AbstractModel* model) override;

public: // status
    QString checkSum() const;
    int algorithmId() const;
    bool isApplyable() const; // candidate for AbstractTool API
    bool isUptodate() const;

    QVector<AbstractByteArrayChecksumAlgorithm*> algorithmList() const;

public:
    AbstractByteArrayChecksumParameterSet* parameterSet();

public Q_SLOTS: // actions
    void calculateChecksum();

    void setAlgorithm(int algorithmId);
    // TODO: hack, see checksum source
    void resetSourceTool();

Q_SIGNALS:
    void checksumChanged(const QString& checksum);
    void uptodateChanged(bool isUptodate);
    void isApplyableChanged(bool isApplyable); // candidate for AbstractTool API

private:
    void checkUptoDate();

private Q_SLOTS:
    void onSelectionChanged();
    void onSourceChanged();
    void onSourceDestroyed();

private: // created data
    QString mCheckSum;
    bool mChecksumUptodate : 1;
    bool mSourceByteArrayModelUptodate : 1;

private: // settings
    QVector<AbstractByteArrayChecksumAlgorithm*> mAlgorithmList;
#ifdef HAVE_QCA2
    QCA::Initializer* mQcaInitializer;
#endif
    int mAlgorithmId = 0;

private: // sources
    ByteArrayView* mByteArrayView = nullptr;
    // current
    Okteta::AbstractByteArrayModel* mByteArrayModel = nullptr;

    int mSourceAlgorithmId = -1;
    Okteta::AddressRange mSourceSelection;
    Okteta::AbstractByteArrayModel* mSourceByteArrayModel = nullptr;
};

inline int ChecksumTool::algorithmId() const { return mAlgorithmId; }
inline QString ChecksumTool::checkSum()  const { return mCheckSum; }
inline bool ChecksumTool::isUptodate()   const { return mChecksumUptodate; }

}

#endif
