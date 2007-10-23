/***************************************************************************
                          kClipboardinterface.h  -  description
                             -------------------
    begin                : Thu Nov 2 2006
    copyright            : 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/

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

Q_DECLARE_INTERFACE( KDE::If::Clipboard, "org.kde.if.clipboard/1.0" )

#endif
#endif
