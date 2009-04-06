/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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


// TODO: for docked widgets signal widgets if embedded or floating, if horizontal/vertical
ReplaceController::ReplaceController( KXmlGuiWindow *window )
  : mWindow( window ),
    mReplaceDialog( 0 ),
    mReplacePrompt( 0 )
{
    KActionCollection* ActionCollection = mWindow->actionCollection();

    mReplaceAction = KStandardAction::replace( this, SLOT(replace()), ActionCollection );

    mTool = new ReplaceTool();
    mTool->setUserQueryAgent( this );

    connect( mTool, SIGNAL(isApplyableChanged( bool )),
             mReplaceAction, SLOT(setEnabled( bool )) );

    connect( mTool, SIGNAL(finished( bool, int )), SLOT(onFinished( bool, int )) );
}

void ReplaceController::setTargetModel( AbstractModel* model )
{
    mTool->setTargetModel( model );
}

void ReplaceController::replace()
{
    // ensure dialog
    if( !mReplaceDialog )
        mReplaceDialog = new KReplaceDialog( mTool, mWindow );

    mReplaceDialog->show();
    mReplaceDialog->setFocus();
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
        KMessageBox::sorry( mWindow, i18nc("@info","Replace pattern not found in byte array."), messageBoxTitle );
    else
        KMessageBox::information( mWindow, replacementReport, messageBoxTitle );
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

    const int answer = KMessageBox::questionYesNo( mWindow, replacementReport+"<nl/>"+question, messageBoxTitle,
                                                   KStandardGuiItem::cont(), KStandardGuiItem::cancel() );

    const bool result = ( answer != KMessageBox::No );

    return result;
}

KDE::ReplaceBehaviour ReplaceController::queryReplaceCurrent() const
{
    if( !mReplacePrompt )
        mReplacePrompt = new KReplacePrompt( mWindow );

    mReplacePrompt->show();
    const KDE::ReplaceBehaviour answer = mReplacePrompt->query();

    if( answer == KDE::ReplaceAll || answer == KDE::CancelReplacing )
        mReplacePrompt->hide();

    return answer;
}

ReplaceController::~ReplaceController()
{
    delete mReplaceDialog;
    delete mReplacePrompt;
    delete mTool;
}
