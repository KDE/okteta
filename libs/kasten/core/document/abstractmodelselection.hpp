/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTMODELSELECTION_HPP
#define KASTEN_ABSTRACTMODELSELECTION_HPP

// lib
#include "kastencore_export.hpp"
// Qt
#include <QScopedPointer>

namespace Kasten {

class KASTENCORE_EXPORT AbstractModelSelection
{
protected:
    AbstractModelSelection();

public:
    virtual ~AbstractModelSelection();

public: // API to be implemented
//     virtual bool isValid() const = 0;

private:
    const QScopedPointer<class AbstractModelSelectionPrivate> d_ptr;
};

}

#endif
