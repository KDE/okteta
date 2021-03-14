/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CLIPBOARDCONTROLLER_HPP
#define KASTEN_CLIPBOARDCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

class KXMLGUIClient;
class QAction;

namespace Kasten {

namespace If {
class DataSelectable;
class SelectedDataWriteable;
class SelectedDataCutable;
}

class ClipboardController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    explicit ClipboardController(KXMLGUIClient* guiClient);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS: // action slots
    void cut();
    void copy();
    void paste();

private Q_SLOTS:
    void onHasSelectedDataChanged(bool hasSelectedData);
    void onCanCutSelectedDataChanged(bool canCutSelectedData);
    void onReadOnlyChanged(bool isReadOnly);
    void onClipboardDataChanged();

private:
    AbstractModel* mModel = nullptr;
    If::DataSelectable* mSelectionControl = nullptr;
    If::SelectedDataWriteable* mMimeDataControl = nullptr;
    If::SelectedDataCutable* mCutControl = nullptr;

    QAction* mCutAction;
    QAction* mCopyAction;
    QAction* mPasteAction;
};

}

#endif
