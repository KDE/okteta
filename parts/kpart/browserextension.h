/***************************************************************************
                          browserextension.h  -  description
                             -------------------
    begin                : Di Nov 16 2004
    copyright            : (C) 2004 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef OKTETABROWSEREXTENSION_H
#define OKTETABROWSEREXTENSION_H


// KDE
#include <kparts/browserextension.h>

class OktetaPart;


/**
 * @short Extension for better support for embedding in browsers
 * @author Friedrich W. H. Kossebau <kossebau@kde.org>
 */
class OktetaBrowserExtension : public KParts::BrowserExtension
{
    Q_OBJECT

  public:
    explicit OktetaBrowserExtension( OktetaPart *part );

  public: // KParts::BrowserExtension API
    virtual void saveState( QDataStream &stream );
    virtual void restoreState( QDataStream &stream );

  public Q_SLOTS:
    /** copy text to clipboard */
    void copy();

  private Q_SLOTS:
    /** selection has changed */
    void onSelectionChanged( bool );

  protected:
    OktetaPart *part;
};

#endif
