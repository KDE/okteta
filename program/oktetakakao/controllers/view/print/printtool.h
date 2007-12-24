/***************************************************************************
                          printtool.h  -  description
                             -------------------
    begin                : Thu Dec 6 2007
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


#ifndef PRINTTOOL_H
#define PRINTTOOL_H

class KByteArrayDocument;
class KXmlGuiWindow;

/**
This tool cares for the printing of the byte array to a series of papers.
This is done by creating a series of print commands, which may also
be in form of a pdf document. We don't care for this, it's done
by Qt and the print dialog.

The content is printed into a series of frames. By default there are
a header frame, the content frame and the footer frame on each page.


-> Header printer, footer printer, content printer
-> called by a pageprinter which knows about the layout
-> pageprinter controls the page settings and informs the embedded printer
-> frameprinter return number of frames they need for their content
-> endless frames vs. ending frames
-> vars for each frame: see KWrite
-> 

*/
class PrintTool
{
  public:
    explicit PrintTool( KXmlGuiWindow *window );
    ~PrintTool();

  public:
    void print( KByteArrayDocument *document );

  protected:
    KXmlGuiWindow *mWindow;
};
#endif
