/***************************************************************************
                          kbuffersection.h  -  description
                             -------------------
    begin                : 22.06.2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
****************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KSELECTION_H
#define KSELECTION_H

#include "ksection.h"

namespace KHE
{

/** This class describes a selected section of the buffer.
  * As it is used as selection controlled by
  * mouse and keyboard commands it offers two ways to set its range:
  * - by giving the startposition (of the cursor) of an interactive selection
  *   and the subsequent end positions (until selection is finished)
  * - direct setting (as provided by KSection)
  *
  * the interactive selection takes care that
  *
  *@author Friedrich W. H.  Kossebau
  */
class KSelection : public KSection
{
  public:
    KSelection( int Index );
    KSelection();
    ~KSelection();

  public:
    KSelection &operator=( const KSelection &S );
    KSelection &operator=( const KSection &S );

  public: // modification access
    /** */
    void setStart( int Index );
    void setEnd( int Index );
    /***/
    void cancel();

  public: // value access
    int anchor() const;

  public: // logic access
    bool started() const;
    bool justStarted() const;

  protected:
    /** cursor index where the selection starts */
    int Anchor;
};


inline KSelection::KSelection() : Anchor( -1 ) {}
inline KSelection::KSelection( int Index ) : Anchor( Index )  {}
inline KSelection::~KSelection() {}
inline KSelection &KSelection::operator=( const KSelection &S )
{ KSection::operator=(S); Anchor = S.Anchor; return *this; }
inline KSelection &KSelection::operator=( const KSection &S )
{ KSection::operator=(S); Anchor = start(); return *this; }


inline void KSelection::setStart( int Index )
{
  Anchor = Index;
  unset();
}


inline void KSelection::setEnd( int Index )
{
  // nothing selected?
  if( Index == Anchor )
    unset();
  // selecting forwards?
  else if( Index > Anchor )
  {
    KSection::setStart( Anchor );
    KSection::setEnd( Index-1 );
  }
  // selecting backwards
  else
  {
    KSection::setStart( Index );
    KSection::setEnd( Anchor-1 );
  }
}

inline int KSelection::anchor() const { return Anchor; }

inline void KSelection::cancel() { Anchor = -1; unset(); }

inline bool KSelection::started() const { return Anchor != -1; }

inline bool KSelection::justStarted() const { return Anchor != -1 && start() == -1; }
}

#endif
