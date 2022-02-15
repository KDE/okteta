/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008, 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTBYTEARRAYFILTER_HPP
#define KASTEN_ABSTRACTBYTEARRAYFILTER_HPP

// Okteta core
#include <Okteta/AddressRange>
#include <Okteta/Byte>
// Qt
#include <QObject>

class AbstractByteArrayFilterParameterSet;
namespace Okteta {
class AbstractByteArrayModel;
}
class KConfigGroup;
class QString;

class AbstractByteArrayFilter : public QObject
{
    Q_OBJECT

protected:
    static constexpr int FilteredByteCountSignalLimit = 10000;

protected:
    explicit AbstractByteArrayFilter(const QString& name, const QString& id);

public:
    ~AbstractByteArrayFilter() override;

public: // API to be implemented
    virtual bool filter(Okteta::Byte* result, Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const = 0;
    /** used by the editor to get write access to the parameters */
    virtual AbstractByteArrayFilterParameterSet* parameterSet() = 0;
    virtual void loadConfig(const KConfigGroup& configGroup);
    virtual void saveConfig(KConfigGroup& configGroup) const;

public:
    QString name() const;
    QString id() const;

Q_SIGNALS: // TODO: add check for signal to tests
    void filteredBytes(int bytes) const;

private:
    QScopedPointer<class AbstractByteArrayFilterPrivate> const d;
};

#endif
