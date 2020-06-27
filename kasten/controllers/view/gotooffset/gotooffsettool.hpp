/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_GOTOOFFSETTOOL_HPP
#define KASTEN_GOTOOFFSETTOOL_HPP

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
class GotoOffsetTool : public AbstractTool
{
    Q_OBJECT

public:
    GotoOffsetTool();
    ~GotoOffsetTool() override;

public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    QString title() const override;

    void setTargetModel(AbstractModel* model) override;

public: // status
    int currentOffset() const;
    int targetOffset() const;
    bool isRelative() const;
    bool isSelectionToExtent() const;
    bool isBackwards() const;

    bool isUsable() const;
    bool isApplyable() const; // candidate for AbstractTool API

public Q_SLOTS: // settings
    void setTargetOffset(Okteta::Address targetOffset);
    void setIsRelative(bool isRelative);
    void setIsSelectionToExtent(bool isSelectionToExtent);
    void setIsBackwards(bool isBackwards);

public Q_SLOTS: // actions
    void gotoOffset();

Q_SIGNALS:
    void isUsableChanged(bool isUsable);
    void isApplyableChanged(bool isApplyable);    // candidate for AbstractTool API
    // TODO: isAtLine useful, to prevent noop actions, or should they be allowed, too?

private Q_SLOTS:
    void onContentsChanged();

private:
    int finalTargetOffset() const;

private: // settings
    int mTargetOffset = 0;
    bool mIsRelative = false;
    bool mIsSelectionToExtent = false;
    bool mIsBackwards = false;

private: // target
    ByteArrayView* mByteArrayView = nullptr;
    Okteta::AbstractByteArrayModel* mByteArrayModel = nullptr;
};

inline int GotoOffsetTool::targetOffset()         const { return mTargetOffset; }
inline bool GotoOffsetTool::isRelative()          const { return mIsRelative; }
inline bool GotoOffsetTool::isSelectionToExtent() const { return mIsSelectionToExtent; }
inline bool GotoOffsetTool::isBackwards()         const { return mIsBackwards; }

}

#endif
