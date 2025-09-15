/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SELECTRANGETOOL_HPP
#define KASTEN_SELECTRANGETOOL_HPP

// Okteta core
#include <Okteta/Address>
// Kasten core
#include <Kasten/AbstractTool>

namespace Okteta {
class AbstractByteArrayModel;
}

namespace Kasten {

class ByteArrayView;

/**
 */
class SelectRangeTool : public AbstractTool
{
    Q_OBJECT

public:
    SelectRangeTool();
    ~SelectRangeTool() override;

public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    [[nodiscard]]
    QString title() const override;

    void setTargetModel(AbstractModel* model) override;

public: // status
    [[nodiscard]]
    Okteta::Address currentSelectionStart() const;
    [[nodiscard]]
    Okteta::Address currentSelectionEnd() const;
    [[nodiscard]]
    Okteta::Address targetSelectionStart() const;
    [[nodiscard]]
    Okteta::Address targetSelectionEnd() const;
    [[nodiscard]]
    bool isEndRelative() const;
    [[nodiscard]]
    bool isEndBackwards() const;

    [[nodiscard]]
    bool isUsable() const;
    [[nodiscard]]
    bool isApplyable() const; // candidate for AbstractTool API

public Q_SLOTS: // settings
    void setTargetStart(Okteta::Address start);
    void setTargetEnd(Okteta::Address end);
    void setIsEndRelative(bool isEndRelative);
    void setIsEndBackwards(bool isEndBackwards);

public Q_SLOTS: // actions
    void select();

Q_SIGNALS:
    void isUsableChanged(bool isUsable);
    void isApplyableChanged(bool isApplyable);    // candidate for AbstractTool API
    // TODO: isAtLine useful, to prevent noop actions, or should they be allowed, too?

private:
    [[nodiscard]]
    Okteta::Address finalTargetSelectionStart() const;
    [[nodiscard]]
    Okteta::Address finalTargetSelectionEnd() const;

private Q_SLOTS:
    void onContentsChanged();

private: // settings
    Okteta::Address mTargetStart = 0;
    Okteta::Address mTargetEnd = -1;
    bool mIsEndRelative : 1;
    bool mIsEndBackwards : 1;

private: // target
    ByteArrayView* mByteArrayView = nullptr;
    Okteta::AbstractByteArrayModel* mByteArrayModel = nullptr;
};

inline Okteta::Address SelectRangeTool::targetSelectionStart() const { return mTargetStart; }
inline Okteta::Address SelectRangeTool::targetSelectionEnd()   const { return mTargetEnd; }
inline bool SelectRangeTool::isEndRelative() const { return mIsEndRelative; }
inline bool SelectRangeTool::isEndBackwards() const { return mIsEndBackwards; }

}

#endif
