/***************************************************************************
                          kiselecteddatawriteable.h  -  description
                             -------------------
    begin                : Mon Nov 13 2006
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


#ifndef KDE_IF_SELECTEDDATAWRITEABLE_H
#define KDE_IF_SELECTEDDATAWRITEABLE_H

// Qt
#include <QtCore/QtPlugin>

class QMimeData;


namespace KDE
{
namespace If
{
// TODO: this interface is strongly related to the selection interface
// needs further thinking
// for now the mimedata is the write extension to the selection interface
// as "createCopyOfSelectedData()" got moved to selection
// rename both interfaces to Selection and SelectionWriteable?
class SelectedDataWriteable
{
  public:
    virtual ~SelectedDataWriteable();

  public: // operate
    // TODO: inserting works on selection if available
    virtual void insertData( const QMimeData *data ) = 0;
    virtual QMimeData *cutSelectedData() = 0;
    virtual void deleteSelectedData() = 0;
};

inline SelectedDataWriteable::~SelectedDataWriteable() {}

}
}

Q_DECLARE_INTERFACE( KDE::If::SelectedDataWriteable, "org.kde.if.selecteddatawriteable/1.0" )

#endif
