/***************************************************************************
                          replacecontroller.h  -  description
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


#ifndef KREPLACETOOLET_H
#define KREPLACETOOLET_H


// lib
#include <kfinddirection.h>
// kakao
#include <kviewcontroller.h>

class KXmlGuiWindow;
class KAction;
class KViewManager;
namespace KHEUI {
class KByteArrayView;
}
namespace KHECore {
class KAbstractByteArrayModel;
}
class KReplaceDialog;
class KReplacePrompt;


class ReplaceController : public KViewController
{
  Q_OBJECT

  public:
    ReplaceController( KXmlGuiWindow *MW );
    ~ReplaceController();

  public: // KViewController API
    virtual void setView( KAbstractView *View );

  protected:
    void findNext();
    void replaceCurrent();

  protected Q_SLOTS: // action slots
    void replace();

  private Q_SLOTS:
    void onDialogOkClicked();
    void onPromptAllClicked();
    void onPromptSkipClicked();
    void onPromptReplaceClicked();

  protected:
    KXmlGuiWindow *MainWindow;
    KHEUI::KByteArrayView *HexEdit;
    KHECore::KAbstractByteArrayModel *ByteArray;

    KAction *ReplaceAction;

    QByteArray SearchData;
    QByteArray ReplaceData;
    bool PreviousFound:1;
    bool DoWrap:1;
    bool IgnoreCase:1;
    bool Prompt:1;
    KFindDirection Direction;
    int ReplaceFirstIndex;
    int ReplaceLastIndex;
    int CurrentIndex;
    int NoOfReplacements;

    KReplaceDialog *ReplaceDialog;
    KReplacePrompt *ReplacePrompt;
};

#endif
