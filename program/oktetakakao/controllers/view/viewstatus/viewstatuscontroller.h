/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef VIEWSTATUSCONTROLLER_H
#define VIEWSTATUSCONTROLLER_H

// Kakao gui
#include <abstractxmlguicontroller.h>
// Okteta gui
#include <koffsetformat.h>

namespace KHEUI {
class KByteArrayView;
}
class KStatusBar;
class QLabel;
class QString;


class ViewStatusController : public AbstractXmlGuiController
{
  Q_OBJECT

  public:
    explicit ViewStatusController( KStatusBar* statusBar );

  public: // AbstractXmlGuiController API
    virtual void setTargetModel( AbstractModel* model );

  protected Q_SLOTS:
    void onCursorPositionChanged( int offset );
    void onOverwriteModeChanged( bool isOverwrite );
    void onValueCodingChanged( int coding );
    void onCharCodecChanged( const QString& charCodecName );
    void onReadOnlyChanged( bool isReadOnly );

  protected:
    KHEUI::KByteArrayView* mByteArrayView;

    QLabel* mOffsetLabel;
    QLabel* mOverwriteModeLabel;
    QLabel* mValueCodingLabel;
    QLabel* mCharCodingLabel;
    QLabel* mReadOnlyLabel;

    KHEUI::KOffsetFormat::print mPrintFunction;
    int mStartOffset;
};

#endif
