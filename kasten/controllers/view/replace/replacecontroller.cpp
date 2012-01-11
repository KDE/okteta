/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2006-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "replacecontroller.h"

// controller
#include "kreplacedialog.h"
#include "kreplaceprompt.h"
#include "replacetool.h"
// KDE
#include <KXmlGuiWindow>
#include <KLocale>
#include <KAction>
#include <KActionCollection>
#include <KStandardAction>
#include <KMessageBox>
// Qt
#include <QtGui/QApplication>


namespace Kasten2
{

// TODO: for docked widgets signal widgets if embedded or floating, if horizontal/vertical
ReplaceController::ReplaceController( KXMLGUIClient* guiClient, QWidget* parentWidget )
  : mParentWidget( parentWidget ),
    mReplaceDialog( 0 ),
    mReplacePrompt( 0 )
{
    KActionCollection* ActionCollection = guiClient->actionCollection();

    mReplaceAction = KStandardAction::replace( this, SLOT(replace()), ActionCollection );

    mTool = new ReplaceTool();
    mTool->setUserQueryAgent( this );

    connect( mTool, SIGNAL(isApplyableChanged(bool)),
             mReplaceAction, SLOT(setEnabled(bool)) );

    connect( mTool, SIGNAL(finished(bool,int)), SLOT(onFinished(bool,int)) );

    mReplaceAction->setEnabled( false );
}

void ReplaceController::setTargetModel( AbstractModel* model )
{
    mTool->setTargetModel( model );
}

void ReplaceController::replace()
{
    // ensure dialog
    if( !mReplaceDialog )
        mReplaceDialog = new KReplaceDialog( mTool, mParentWidget );

    mReplaceDialog->show();
}


void ReplaceController::onFinished( bool previousFound, int noOfReplacements )
{
    if( mReplacePrompt )
        mReplacePrompt->hide();

    const QString messageBoxTitle = i18nc( "@title:window", "Replace" );
    const QString replacementReport = (noOfReplacements==0) ?
            i18nc( "@info", "No replacements made.") :
            i18ncp( "@info", "1 replacement made.", "%1 replacements made.", noOfReplacements );

    if( ! previousFound )
        KMessageBox::sorry( mParentWidget, i18nc("@info","Replace pattern not found in byte array."), messageBoxTitle );
    else
        KMessageBox::information( mParentWidget, replacementReport, messageBoxTitle );
}

bool ReplaceController::queryContinue( KFindDirection direction, int noOfReplacements ) const
{
    const QString messageBoxTitle = i18nc( "@title:window", "Replace" );
    const QString replacementReport = (noOfReplacements==0) ?
            i18nc( "@info", "No replacements made.") :
            i18ncp( "@info", "1 replacement made.", "%1 replacements made.", noOfReplacements );
    const QString question = ( direction == FindForward ) ?
        i18nc( "@info", "End of byte array reached.<nl/>Continue from the beginning?" ) :
        i18nc( "@info", "Beginning of byte array reached.<nl/>Continue from the end?" );

    const QString message = replacementReport + QLatin1String("<br /><br />") + question;
    const int answer = KMessageBox::questionYesNo( mParentWidget, message, messageBoxTitle,
                                                   KStandardGuiItem::cont(), KStandardGuiItem::cancel() );

    const bool result = ( answer != KMessageBox::No );

    return result;
}

ReplaceBehaviour ReplaceController::queryReplaceCurrent() const
{
    if( !mReplacePrompt )
        mReplacePrompt = new KReplacePrompt( mParentWidget );

    mReplacePrompt->show();
    const ReplaceBehaviour answer = mReplacePrompt->query();

    if( answer == ReplaceAll || answer == CancelReplacing )
        mReplacePrompt->hide();

    return answer;
}

ReplaceController::~ReplaceController()
{
    delete mReplaceDialog;
    delete mReplacePrompt;
    delete mTool;
}

}
