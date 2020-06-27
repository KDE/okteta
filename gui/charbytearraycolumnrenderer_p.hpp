/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_CHARBYTEARRAYCOLUMNRENDERER_P_HPP
#define OKTETA_CHARBYTEARRAYCOLUMNRENDERER_P_HPP

// lib
#include "charbytearraycolumnrenderer.hpp"
#include "abstractbytearraycolumnrenderer_p.hpp"
#include "oktetagui.hpp"

namespace Okteta {

class CharByteArrayColumnRendererPrivate : public AbstractByteArrayColumnRendererPrivate
{
public:
    CharByteArrayColumnRendererPrivate(CharByteArrayColumnRenderer* q,
                                       AbstractColumnStylist* stylist,
                                       AbstractByteArrayModel* byteArrayModel,
                                       ByteArrayTableLayout* layout,
                                       ByteArrayTableRanges* ranges);

    ~CharByteArrayColumnRendererPrivate() override;

public:
    void init();

public: // modification access
    bool setShowingNonprinting(bool showingNonprinting = true);
    bool setSubstituteChar(QChar substituteChar);
    bool setUndefinedChar(QChar undefinedChar);

public: // value access
    bool isShowingNonprinting() const;
    QChar substituteChar() const;
    QChar undefinedChar() const;

protected: // AbstractByteArrayColumnRendererPrivate API
    void renderByteText(QPainter* painter, Byte byte, Character byteChar, const QColor& color) const override;

private:
    /** */
    QChar mSubstituteChar;
    /** */
    QChar mUndefinedChar;
    /** */
    bool mShowingNonprinting;

private:
    Q_DECLARE_PUBLIC(CharByteArrayColumnRenderer)
};


inline CharByteArrayColumnRendererPrivate::CharByteArrayColumnRendererPrivate(CharByteArrayColumnRenderer* q,
                                                                              AbstractColumnStylist* stylist,
                                                                              AbstractByteArrayModel* byteArrayModel,
                                                                              ByteArrayTableLayout* layout,
                                                                              ByteArrayTableRanges* ranges)
    : AbstractByteArrayColumnRendererPrivate(q, stylist, byteArrayModel, layout, ranges)
    , mSubstituteChar(DefaultSubstituteChar)
    , mUndefinedChar(DefaultUndefinedChar)
    , mShowingNonprinting(DefaultShowingNonprinting)
{
}

inline CharByteArrayColumnRendererPrivate::~CharByteArrayColumnRendererPrivate() = default;

inline void CharByteArrayColumnRendererPrivate::init()
{
    Q_Q(CharByteArrayColumnRenderer);

    q->setSpacing(0, 0, 0);
}

inline bool CharByteArrayColumnRendererPrivate::isShowingNonprinting()  const { return mShowingNonprinting; }
inline QChar CharByteArrayColumnRendererPrivate::substituteChar()       const { return mSubstituteChar; }
inline QChar CharByteArrayColumnRendererPrivate::undefinedChar()        const { return mUndefinedChar; }

inline bool CharByteArrayColumnRendererPrivate::setSubstituteChar(QChar substituteChar)
{
    if (mSubstituteChar == substituteChar) {
        return false;
    }

    mSubstituteChar = substituteChar;
    return true;
}

inline bool CharByteArrayColumnRendererPrivate::setUndefinedChar(QChar undefinedChar)
{
    if (mUndefinedChar == undefinedChar) {
        return false;
    }

    mUndefinedChar = undefinedChar;
    return true;
}

inline bool CharByteArrayColumnRendererPrivate::setShowingNonprinting(bool showingNonprinting)
{
    if (mShowingNonprinting == showingNonprinting) {
        return false;
    }

    mShowingNonprinting = showingNonprinting;
    return true;
}

}

#endif
