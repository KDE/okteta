/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KHE_UI_KSELECTION_H
#define KHE_UI_KSELECTION_H

// commonlib
#include <ksection.h>


namespace KHEUI
{

/** This class describes a selected section of the buffer.
  * As it is used as selection controlled by
  * mouse and keyboard commands it offers two ways to set its range:
  * - by giving the startposition (of the cursor) of an interactive selection
  *   and the subsequent end positions (until selection is finished)
  * - direct setting (as provided by KHE::KSection)
  *
  * the interactive selection takes care that
  *
  *@author Friedrich W. H.  Kossebau
  */
class KSelection
{
  public:
    /** creates a selection with a given start.
      * @param Index index in front of which the selection begins
      */
    explicit KSelection( int Index );
    /** creates an invalid selection */
    KSelection();
    ~KSelection();

  public:
    KSelection &operator=( const KSelection &S );
    KSelection &operator=( const KHE::KSection &S );

  public: // modification access
    /** starts the selection.
      * For this the anchor, start and end are set to the given index,
      * so the initial selection is empty.
      * @param Index index in front of which the selection begins
      */
    void setStart( int Index );
    /** sets the end of the current selection.
      * If the end is before the start the selection will reach from the given index 
      * @param Index index in front of which the selection ends
      */
    void setEnd( int Index );
    /** sets the selection to be invalid
      */
    void cancel();
    /** sets the anchor to the start or the end.
      * @param if true to the start, otherwise to the end
      * If the selection has not started the behaviour is undefined.
      */
    void setForward( bool Forward = true );
    /** swaps anchor from start to end or vice versa.
      * If the selection has not started the behaviour is undefined.
      */
    void reverse();

    void adaptToChange( int Pos, int RemovedLength, int InsertedLength );

  public: // value access
    /** 
      * @return anchor value
      */
    int anchor() const;
    int start() const;
    int end() const;
    /** 
      * @return section
      */
    const KHE::KSection &section() const;


  public: // logic access
    bool isValid() const;
    /** 
      * @return @c true if the anchor has been set, otherwise @c false.
      */
    bool started() const;
    /** 
      * @return @c true if the anchor has been set and the selection is empty, otherwise @c false.
      */
    bool justStarted() const;
    /** 
      * @return @c true if the anchor is at the begin of the selection 
      */
    bool isForward() const;

  protected:
    /** Section */
    KHE::KSection Section;
    /** cursor index where the selection starts */
    int Anchor;
};


inline KSelection::KSelection() : Anchor( -1 ) {}
inline KSelection::KSelection( int Index ) : Anchor( Index )  {}
inline KSelection::~KSelection() {}

inline KSelection &KSelection::operator=( const KSelection &S )
{
  Section = S.Section;
  Anchor = S.Anchor;
  return *this;
}

inline KSelection &KSelection::operator=( const KHE::KSection &S )
{
  Section = S;
  Anchor = S.start();
  return *this;
}


inline void KSelection::setStart( int Index )
{
  Anchor = Index;
  Section.unset();
}


inline void KSelection::setEnd( int Index )
{
  // nothing selected?
  if( Index == Anchor )
    Section.unset();
  // selecting forwards?
  else if( Index > Anchor )
  {
    Section.setStart( Anchor );
    Section.setEnd( Index-1 );
  }
  // selecting backwards
  else
  {
    Section.setStart( Index );
    Section.setEnd( Anchor-1 );
  }
}

inline void KSelection::reverse()
{
   Anchor = isForward() ? Section.end()+1 : Section.start();
}

inline void KSelection::setForward( bool Forward )
{
   Anchor = Forward ? Section.start() : Section.end()+1;
}

inline const KHE::KSection &KSelection::section() const { return Section; }
inline int KSelection::anchor()              const { return Anchor; }
inline int KSelection::start()               const { return Section.start(); }
inline int KSelection::end()                 const { return Section.end(); }

inline void KSelection::cancel() { Anchor = -1; Section.unset(); }

inline bool KSelection::isValid()     const { return Section.isValid(); }
inline bool KSelection::started()     const { return Anchor != -1; }
inline bool KSelection::justStarted() const { return Anchor != -1 && Section.start() == -1; }
inline bool KSelection::isForward()   const { return Anchor == Section.start(); }

inline void KSelection::adaptToChange( int Pos, int RemovedLength, int InsertedLength )
{
  Section.adaptToChange( Pos, RemovedLength, InsertedLength );
  Anchor = isForward() ? Section.start() : Section.end()+1;
}

}

#endif
