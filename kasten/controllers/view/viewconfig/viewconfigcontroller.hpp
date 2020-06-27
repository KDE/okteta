/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2010, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VIEWCONFIGCONTROLLER_HPP
#define KASTEN_VIEWCONFIGCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

class KXMLGUIClient;
class KSelectAction;
class KToggleAction;
class QAction;

namespace Kasten {

class ByteArrayView;

class ViewConfigController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    explicit ViewConfigController(KXMLGUIClient* guiClient);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS: // action slots
    void setValueCoding(int valueCoding);
    void setCharCoding(int valueCoding);
    void setShowsNonprinting(bool on);
    void showBytesPerLineDialog();
    void showBytesPerGroupDialog();
    void setLayoutStyle(int layoutStyle);
    void setOffsetCoding(int offsetCoding);
    void toggleOffsetColumn(bool on);
    void toggleValueCharColumns(int visibleColunms);

    void onOffsetColumnVisibleChanged(bool offsetColumnVisible);
    void onOffsetCodingChanged(int offsetCoding);
    void onShowsNonprintingChanged(bool showsNonprinting);
    void onValueCodingChanged(int valueCoding);
    void onCharCodecChanged(const QString& charCodecName);
    void onLayoutStyleChanged(int layoutStyle);
    void onVisibleByteArrayCodingsChanged(int visibleByteArrayCodings);

    void setBytesPerLine(int bytesPerLine);
    void setBytesPerGroup(int bytesPerGroup);

private:
    ByteArrayView* mByteArrayView = nullptr;

    // view menu
    KSelectAction* mCodingAction;
    KSelectAction* mEncodingAction;
    KToggleAction* mShowsNonprintingAction;
    QAction* mSetBytesPerLineAction;
    QAction* mSetBytesPerGroupAction;
    KSelectAction* mResizeStyleAction;
    KToggleAction* mShowOffsetColumnAction;
    KSelectAction* mOffsetCodingAction;
    KSelectAction* mToggleColumnsAction;
};

}

#endif
