/***************************************************************************
                          khebrowserextension.h  -  description
                             -------------------
    begin                : Di Nov 16 2004
    copyright            : (C) 2004 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHEBROWSEREXTENSION_H
#define KHEBROWSEREXTENSION_H

// kde specific
#include <kparts/browserextension.h>

namespace KHE
{

// forward declaration
class KHexEditPart;

/**
 * @short Extension for better support for embedding in browsers
 * @author Friedrich W. H. Kossebau <Friedrich.W.H@Kossebau.de>
 */
class KHexEditBrowserExtension : public KParts::BrowserExtension
{
    Q_OBJECT

  public:
    KHexEditBrowserExtension( KHexEditPart *P );

  public: // KParts::BrowserExtension API
    virtual void saveState( QDataStream &stream );
    virtual void restoreState( QDataStream &stream );

  public slots:
    /** copy text to clipboard */
    void copy();

    /** selection has changed */
    void slotSelectionChanged();

  protected:
    KHexEditPart *HexEditPart;
};

}

#endif
