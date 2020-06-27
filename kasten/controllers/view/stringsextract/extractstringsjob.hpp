/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_EXTRACTSTRINGSJOB_HPP
#define KASTEN_EXTRACTSTRINGSJOB_HPP

// tool
#include "containedstring.hpp"
// Okteta core
#include <Okteta/AddressRange>
// Qt
#include <QObject>
#include <QList>

namespace Okteta {
class AbstractByteArrayModel;
class CharCodec;
}

namespace Kasten {

class ExtractStringsJob : public QObject // not yet: KJob
{
    Q_OBJECT

public:
    ExtractStringsJob(const Okteta::AbstractByteArrayModel* model,
                      const Okteta::AddressRange& selection,
                      const Okteta::CharCodec* charCodec,
                      int minLength,
                      QList<ContainedString>* containedStringList);

public:
    void exec();

private:
    const Okteta::AbstractByteArrayModel* mByteArrayModel;
    const Okteta::AddressRange mSelection;
    const Okteta::CharCodec* mCharCodec;
    const int mMinLength;

    QList<ContainedString>* mContainedStringList;
};

inline ExtractStringsJob::ExtractStringsJob(const Okteta::AbstractByteArrayModel* model,
                                            const Okteta::AddressRange& selection,
                                            const Okteta::CharCodec* charCodec,
                                            int minLength,
                                            QList<ContainedString>* containedStringList)
    : mByteArrayModel(model)
    , mSelection(selection)
    , mCharCodec(charCodec)
    , mMinLength(minLength)
    , mContainedStringList(containedStringList)
{}

}

#endif
