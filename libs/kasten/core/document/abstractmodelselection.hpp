/*
    This file is part of the Kasten Framework, made within the KDE community.

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

#ifndef ABSTRACTMODELSELECTION_HPP
#define ABSTRACTMODELSELECTION_HPP

// lib
#include <kasten/kastencore_export.hpp>
// Qt
#include <QObject>

namespace Kasten {

class KASTENCORE_EXPORT AbstractModelSelection
{
protected:
    AbstractModelSelection();

public:
    virtual ~AbstractModelSelection();

public: // API to be implemented
//     virtual bool isValid() const = 0;

protected:
    class Private;
    Private* const d;
};

}

#endif