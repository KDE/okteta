/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2006-2010,2012 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef VIEWCONFIGCONTROLLER_H
#define VIEWCONFIGCONTROLLER_H

// lib
#include "oktetakastencontrollers_export.h"
// Kasten gui
#include <abstractxmlguicontroller.h>

class KXMLGUIClient;
class KSelectAction;
class KToggleAction;
class KAction;


namespace Kasten2
{

class ByteArrayView;


class OKTETAKASTENCONTROLLERS_EXPORT ViewConfigController : public AbstractXmlGuiController
{
  Q_OBJECT

  public:
    explicit ViewConfigController( KXMLGUIClient* guiClient );

  public: // AbstractXmlGuiController API
    virtual void setTargetModel( AbstractModel* model );

  private Q_SLOTS: // action slots
    void setValueCoding( int valueCoding );
    void setCharCoding( int valueCoding );
    void setShowsNonprinting( bool on );
    void setBytesPerLine();
    void setBytesPerGroup();
    void setLayoutStyle( int layoutStyle );
    void toggleOffsetColumn( bool on );
    void toggleValueCharColumns( int visibleColunms );

    void onOffsetColumnVisibleChanged( bool offsetColumnVisible );
    void onShowsNonprintingChanged( bool showsNonprinting );
    void onValueCodingChanged( int valueCoding );
    void onCharCodecChanged( const QString& charCodecName );
    void onLayoutStyleChanged( int layoutStyle );
    void onVisibleByteArrayCodingsChanged( int visibleByteArrayCodings );

  private:
    ByteArrayView* mByteArrayView;

    // view menu
    KSelectAction* mCodingAction;
    KSelectAction* mEncodingAction;
    KToggleAction* mShowsNonprintingAction;
    KAction* mSetBytesPerLineAction;
    KAction* mSetBytesPerGroupAction;
    KSelectAction* mResizeStyleAction;
    KToggleAction* mShowOffsetColumnAction;
    KSelectAction* mToggleColumnsAction;
};

}

#endif
