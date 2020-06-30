/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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
    QString title() const override;

    void setTargetModel(AbstractModel* model) override;

public: // status
    int currentSelectionStart() const;
    int currentSelectionEnd() const;
    int targetSelectionStart() const;
    int targetSelectionEnd() const;
    bool isEndRelative() const;
    bool isEndBackwards() const;

    bool isUsable() const;
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
    int finalTargetSelectionStart() const;
    int finalTargetSelectionEnd() const;

private Q_SLOTS:
    void onContentsChanged();

private: // settings
    int mTargetStart = 0;
    int mTargetEnd = -1;
    bool mIsEndRelative : 1;
    bool mIsEndBackwards : 1;

private: // target
    ByteArrayView* mByteArrayView = nullptr;
    Okteta::AbstractByteArrayModel* mByteArrayModel = nullptr;
};

inline int SelectRangeTool::targetSelectionStart()   const { return mTargetStart; }
inline int SelectRangeTool::targetSelectionEnd()     const { return mTargetEnd; }
inline bool SelectRangeTool::isEndRelative() const { return mIsEndRelative; }
inline bool SelectRangeTool::isEndBackwards() const { return mIsEndBackwards; }

}

#endif
