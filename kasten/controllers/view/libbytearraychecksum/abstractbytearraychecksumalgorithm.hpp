/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTBYTEARRAYCHECKSUMALGORITHM_HPP
#define KASTEN_ABSTRACTBYTEARRAYCHECKSUMALGORITHM_HPP

// Okteta core
#include <Okteta/AddressRange>
// Qt
#include <QObject>

class AbstractByteArrayChecksumParameterSet;
namespace Okteta {
class AbstractByteArrayModel;
}
class QString;

class AbstractByteArrayChecksumAlgorithm : public QObject
{
    Q_OBJECT

protected:
    static constexpr int CalculatedByteCountSignalLimit = 10000;

protected:
    explicit AbstractByteArrayChecksumAlgorithm(const QString& name);

public:
    ~AbstractByteArrayChecksumAlgorithm() override;

public: // API to be implemented
    virtual bool calculateChecksum(QString* result, const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const = 0;
    /** used by the editor to get write access to the parameters */
    virtual AbstractByteArrayChecksumParameterSet* parameterSet() = 0;

public:
    QString name() const;

Q_SIGNALS: // TODO: add check for signal to tests
    void calculatedBytes(int bytes) const;

private:
    QScopedPointer<class AbstractByteArrayChecksumAlgorithmPrivate> const d;
};

#endif
