/*
    This file is part of the Okteta KPart module, made within the KDE community.

    Copyright 2004,2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETABROWSEREXTENSION_H
#define OKTETABROWSEREXTENSION_H


// KDE
#include <KParts/BrowserExtension>

class OktetaPart;


/**
 * @short Extension for better support for embedding in browsers
 * @author Friedrich W. H. Kossebau <kossebau@kde.org>
 */
class OktetaBrowserExtension : public KParts::BrowserExtension
{
    Q_OBJECT

  public:
    explicit OktetaBrowserExtension( OktetaPart* part );

  public: // KParts::BrowserExtension API
    virtual void saveState( QDataStream& stream );
    virtual void restoreState( QDataStream& stream );

  public Q_SLOTS:
    /** copy text to clipboard */
    void copy();
    void print();

  private Q_SLOTS:
    /** selection has changed */
    void onSelectionChanged( bool hasSelection );

  protected:
    OktetaPart* mPart;
};

#endif
