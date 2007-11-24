/***************************************************************************
                          kabstractdocumentselection.cpp  -  description
                             -------------------
    begin                : Thu Nov 22 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "kabstractdocumentselection.h"


class KAbstractDocumentSelection::Private
{
};

KAbstractDocumentSelection::KAbstractDocumentSelection()
 : d( new Private() )
{}

KAbstractDocumentSelection::~KAbstractDocumentSelection()
{
    delete d;
}
