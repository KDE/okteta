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
