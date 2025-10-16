/*
    SPDX-FileCopyrightText: 2009, 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTBYTEARRAYCHECKSUMALGORITHM_HPP
#define KASTEN_ABSTRACTBYTEARRAYCHECKSUMALGORITHM_HPP

// Okteta core
#include <Okteta/AddressRange>
// Qt
#include <QObject>
// Std
#include <memory>

class AbstractByteArrayChecksumParameterSet;
namespace Okteta {
class AbstractByteArrayModel;
}
class KConfigGroup;
class QString;

class AbstractByteArrayChecksumAlgorithm : public QObject
{
    Q_OBJECT

protected:
    static constexpr Okteta::Size CalculatedByteCountSignalLimit = 10000;

protected:
    explicit AbstractByteArrayChecksumAlgorithm(const QString& name, const QString& id);

public:
    ~AbstractByteArrayChecksumAlgorithm() override;

public: // API to be implemented
    [[nodiscard]]
    virtual bool calculateChecksum(QString* result, const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const = 0;
    /** used by the editor to get write access to the parameters */
    [[nodiscard]]
    virtual AbstractByteArrayChecksumParameterSet* parameterSet() = 0;
    virtual void loadConfig(const KConfigGroup& configGroup);
    virtual void saveConfig(KConfigGroup& configGroup) const;

public:
    [[nodiscard]]
    QString name() const;
    [[nodiscard]]
    QString id() const;

Q_SIGNALS: // TODO: add check for signal to tests
    void calculatedBytes(Okteta::Size bytes) const;

private:
    const std::unique_ptr<class AbstractByteArrayChecksumAlgorithmPrivate> d;
};

#endif
