/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_TAPNAVIGATOR_HPP
#define OKTETA_TAPNAVIGATOR_HPP

class QTapGesture;

namespace Okteta {
class AbstractByteArrayView;

class TapNavigator
{
public:
    explicit TapNavigator(AbstractByteArrayView* view);

public:
    bool handleTapGesture(QTapGesture* tapGesture);

private:
    AbstractByteArrayView* mView;
};

}

#endif
