/***************************************************************************
                          kvalueeditor.h  -  description
                             -------------------
    begin                : Sa Dez 4 2004
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


#ifndef KHE_KVALUEEDITOR_H
#define KHE_KVALUEEDITOR_H


// lib specific
#include "keditor.h"

namespace KHE
{

class KValueColumn;


class KValueEditor: public KEditor
{
  protected:
    enum KValueEditAction
    { EnterValue, IncValue, DecValue, ValueAppend, ValueEdit, LeaveValue, CancelValue, ValueBackspace };

  public:
    KValueEditor( KValueColumn *VC, KBufferCursor *BC, KHexEdit *HE, KController *P );
    virtual ~KValueEditor();

  public: // KEditor API
    virtual bool handleKeyPress( QKeyEvent *KeyEvent );

  public:
    void reset();

  public:
    bool isInEditMode() const;

  protected:
    /** executes keyboard Action \a Action. This is normally called by a key event handler. */
    void doValueEditAction( KValueEditAction Action, int Input = -1 );

  public://protected:
    KValueColumn *ValueColumn;
    /** flag whether we are in editing mode */
    bool InEditMode:1;
    /** flag whether byte edit mode was reached by inserting */
    bool EditModeByInsert:1;
    /** */
    unsigned char EditValue;
    /** stores the old byte value */
    unsigned char OldValue;
    /** buffer with the  */
    QString ByteBuffer;
};

inline KValueEditor::~KValueEditor() {}

inline bool KValueEditor::isInEditMode() const { return InEditMode; }
inline void KValueEditor::reset() { InEditMode = false; }

}

#endif
