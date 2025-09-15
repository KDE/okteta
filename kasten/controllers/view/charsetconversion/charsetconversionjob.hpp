/*
    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CHARSETCONVERSIONJOB_HPP
#define KASTEN_CHARSETCONVERSIONJOB_HPP

// Okteta core
#include <Okteta/AddressRange>
#include <Okteta/Byte>
// Qt
#include <QObject>
// Std
#include <map>

namespace Okteta {
class AbstractByteArrayModel;
class CharCodec;
}

namespace Kasten {

class CharsetConversionJob : public QObject // not yet: KJob
{
    Q_OBJECT

public:
    CharsetConversionJob(Okteta::Byte* result,
                         Okteta::AbstractByteArrayModel* model,
                         const Okteta::AddressRange& range,
                         const Okteta::CharCodec* sourceCharCodec,
                         const Okteta::CharCodec* targetCharCodec,
                         bool isSubstitutingMissingChars,
                         Okteta::Byte substituteByte);

public:
    [[nodiscard]]
    bool exec();

public:
    [[nodiscard]]
    int convertedBytesCount() const;
    [[nodiscard]]
    const std::map<Okteta::Byte, int>& failedPerByteCount() const;

private:
    Okteta::Byte* const mResult;
    Okteta::AbstractByteArrayModel* const mByteArrayModel;
    const Okteta::AddressRange mRange;

    const Okteta::CharCodec* const mSourceCharCodec;
    const Okteta::CharCodec* const mTargetCharCodec;

    bool mSubstitutingMissingChars;
    Okteta::Byte mSubstituteByte;

    int mConvertedBytesCount;
    std::map<Okteta::Byte, int> mFailedPerByteCount;
};

inline CharsetConversionJob::CharsetConversionJob(Okteta::Byte* result,
                                                  Okteta::AbstractByteArrayModel* model,
                                                  const Okteta::AddressRange& range,
                                                  const Okteta::CharCodec* sourceCharCodec,
                                                  const Okteta::CharCodec* targetCharCodec,
                                                  bool isSubstitutingMissingChars,
                                                  Okteta::Byte substituteByte)
    : mResult(result)
    , mByteArrayModel(model)
    , mRange(range)
    , mSourceCharCodec(sourceCharCodec)
    , mTargetCharCodec(targetCharCodec)
    , mSubstitutingMissingChars(isSubstitutingMissingChars)
    , mSubstituteByte(substituteByte)
{}

inline int CharsetConversionJob::convertedBytesCount() const { return mConvertedBytesCount; }

inline const std::map<Okteta::Byte, int>& CharsetConversionJob::failedPerByteCount() const { return mFailedPerByteCount; }

}

#endif
