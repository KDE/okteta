/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRINGSEXTRACTTOOL_HPP
#define KASTEN_STRINGSEXTRACTTOOL_HPP

// tool
#include "containedstring.hpp"
// Kasten core
#include <Kasten/AbstractTool>
// Okteta core
#include <Okteta/AddressRange>
// Qt
#include <QList>

namespace Okteta {
class AbstractByteArrayModel;
}

namespace Kasten {

class ByteArrayView;

/**
 */
class StringsExtractTool : public AbstractTool
{
    Q_OBJECT

private:
    static inline constexpr int DefaultMinLength = 3;

    static inline constexpr char ConfigGroupId[] = "StringsExtractTool";
    static inline constexpr char MinimumLengthConfigKey[] = "MinimumLength";

public:
    StringsExtractTool();
    ~StringsExtractTool() override;

public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    [[nodiscard]]
    QString title() const override;

    void setTargetModel(AbstractModel* model) override;

public: // status
    [[nodiscard]]
    const QList<ContainedString>* containedStringList() const;
    [[nodiscard]]
    int minLength() const;
    [[nodiscard]]
    bool isApplyable() const; // candidate for AbstractTool API
    [[nodiscard]]
    bool isUptodate() const;
    [[nodiscard]]
    bool canHighlightString() const;
    [[nodiscard]]
    int offsetCoding() const;

    [[nodiscard]]
    AbstractModel* targetModel() const;

public Q_SLOTS: // settings
    void setMinLength(int minLength);
    void markString(int stringId);
    void unmarkString();
    void selectString(int stringId);

public Q_SLOTS: // actions
    void extractStrings();

Q_SIGNALS:
    void uptodateChanged(bool isUptodate);
    void isApplyableChanged(bool isApplyable);    // candidate for AbstractTool API
    void extractionDone(int extractedStringSCount);
    void canHighlightStringChanged(bool canHighlightString);
    void offsetCodingChanged(int offsetCoding);

private:
    void checkUptoDate();

private Q_SLOTS:
    void onSelectionChanged();
    void onSourceChanged();
    void onSourceDestroyed();
    void onSourceViewDestroyed();

private: // created data
    QList<ContainedString> mContainedStringList;
    bool mExtractedStringsUptodate : 1;
    bool mSourceByteArrayModelUptodate : 1;

private: // settings
    int mMinLength;

private: // sources
    ByteArrayView* mByteArrayView = nullptr;
    // current
    Okteta::AbstractByteArrayModel* mByteArrayModel = nullptr;

    // marked view
    ByteArrayView* mSourceByteArrayView = nullptr;
    // selection source
    Okteta::AddressRange mSourceSelection;
    // source of strings
    Okteta::AbstractByteArrayModel* mSourceByteArrayModel = nullptr;
    // minLength source
    int mSourceMinLength = 0;
};

inline const QList<ContainedString>* StringsExtractTool::containedStringList() const { return &mContainedStringList; }
inline int StringsExtractTool::minLength()     const { return mMinLength; }
inline bool StringsExtractTool::isUptodate()   const { return mExtractedStringsUptodate; }

}

#endif
