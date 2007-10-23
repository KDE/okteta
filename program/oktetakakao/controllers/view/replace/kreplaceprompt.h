/***************************************************************************
                          kreplaceprompt.h  -  description
                             -------------------
    begin                : Fri Nov 24 2006
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


#ifndef KREPLACEPROMPT_H
#define KREPLACEPROMPT_H


// KDE
#include <KDialog>


class KReplacePrompt : public KDialog
{
  public:
    KReplacePrompt( QWidget *parent );
};

#endif
