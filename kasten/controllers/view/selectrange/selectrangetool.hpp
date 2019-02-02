/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef SELECTRANGETOOL_HPP
#define SELECTRANGETOOL_HPP

// Okteta core
#include <okteta/address.hpp>
// Kasten core
#include <kasten/abstracttool.hpp>

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
