/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CHECKSUMTOOL_HPP
#define KASTEN_CHECKSUMTOOL_HPP

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
