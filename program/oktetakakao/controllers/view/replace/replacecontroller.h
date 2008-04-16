/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2006-2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef REPLACECONTROLLER_H
#define REPLACECONTROLLER_H


// lib
#include <kfinddirection.h>
// Kakao gui
#include <kviewcontroller.h>

class KXmlGuiWindow;
class KAction;
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
    explicit ReplaceController( KXmlGuiWindow *MW );
    virtual ~ReplaceController();

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
    KHEUI::KByteArrayView *ViewWidget;
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
