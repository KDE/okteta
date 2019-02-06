/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef STRINGSEXTRACTTOOL_HPP
#define STRINGSEXTRACTTOOL_HPP

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
class CharCodec;
}

namespace Kasten {

class ByteArrayView;

/**
 */
class StringsExtractTool : public AbstractTool
{
    Q_OBJECT

public:
    StringsExtractTool();
    ~StringsExtractTool() override;

public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    QString title() const override;

    void setTargetModel(AbstractModel* model) override;

public: // status
    const QList<ContainedString>* containedStringList() const;
    int minLength() const;
    bool isApplyable() const; // candidate for AbstractTool API
    bool isUptodate() const;
    bool canHighlightString() const;
    int offsetCoding() const;

public Q_SLOTS: // settings
    void setMinLength(int minLength);
    void markString(int stringId);
    void unmarkString();

public Q_SLOTS: // actions
    void extractStrings();

Q_SIGNALS:
    void uptodateChanged(bool isUptodate);
    void isApplyableChanged(bool isApplyable);    // candidate for AbstractTool API
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
