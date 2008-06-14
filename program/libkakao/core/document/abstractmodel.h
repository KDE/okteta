/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef ABSTRACTMODEL_H
#define ABSTRACTMODEL_H

// Qt
#include <QtCore/QObject>

class QString;

// TODO: reasons not to name it AbtractObjectModel, but just as it is
// 
class AbstractModel : public QObject
{
    Q_OBJECT

  protected:
    AbstractModel();
  public:
    virtual ~AbstractModel();

  public: // API to be implemented
    virtual QString title() const = 0;

    /** Default returns false */
    virtual bool isModifiable() const;
    /** default returns true */
    virtual bool isReadOnly() const;
    /** default does nothing */
    virtual void setReadOnly( bool isReadOnly );

  Q_SIGNALS:
    // TODO: readonly and modifiable should be turned into flags, also get/set methods
    void readOnlyChanged( bool isReadOnly );
    void modifiableChanged( bool isModifiable );
    void titleChanged( const QString &newTitel );

  protected:
    class Private;
    Private * const d;
};

#endif
