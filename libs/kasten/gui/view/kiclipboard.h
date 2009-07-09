/*
    This file is part of the Kasten Framework, part of the KDE project.

    Copyright 2006 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#if 0
#ifndef KDE_IF_CLIPBOARD_H
#define KDE_IF_CLIPBOARD_H


namespace KDE
{
namespace If
{

// TODO: is this a valid interface? 
// or is only the tool valid, which could operate on interfaces selection and edit?
class Clipboard
{
  public:
    virtual ~Clipboard();

  public: // set
    /** tries to copy. If there is nothing to copy this call is a noop. */
    virtual void copy() = 0;
    /** tries to cut. If there is nothing to cut this call is a noop. */
    virtual void cut() = 0;
    /** tries to paste.
      * If there is nothing to paste or paste is not possible this call is a noop.
      */
    virtual void paste() = 0;

  public: // get
    virtual float ClipboardLevel() const = 0;

  public: // signal
    virtual void Changed( bool Level ) = 0;
};

inline Clipboard::~Clipboard() {}

}
}

Q_DECLARE_INTERFACE( If::Clipboard, "org.kde.if.clipboard/1.0" )

#endif
#endif
