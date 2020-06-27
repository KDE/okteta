/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CHECKSUMVIEW_HPP
#define KASTEN_CHECKSUMVIEW_HPP

// Kasten gui
#include <Kasten/AbstractToolWidget>

class AbstractByteArrayChecksumParameterSet;

class KComboBox;
class QPushButton;
class QLineEdit;

class QStackedWidget;

namespace Kasten {

class ChecksumTool;

class ChecksumView : public AbstractToolWidget
{
    Q_OBJECT

public:
    explicit ChecksumView(ChecksumTool* tool, QWidget* parent = nullptr);
    ~ChecksumView() override;

public:
    ChecksumTool* tool() const;

private:
    void getParameterSet(AbstractByteArrayChecksumParameterSet* parameterSet) const;

private:
    void addAlgorithms();

private Q_SLOTS: // gui
//     void setDirty( bool dirty );
    void onOperationChange(int index);
    void onCalculateClicked();

private Q_SLOTS: // tool
    void onChecksumUptodateChanged(bool checksumUptodate);
    void onApplyableChanged(bool isApplyable);
    void onValidityChanged(bool isValid);

private:
    ChecksumTool* mTool;

    KComboBox* mAlgorithmComboBox;
    QStackedWidget* mParameterSetEditStack;
    QLineEdit* mChecksumLabel;
    QPushButton* mCalculateButton;
};

inline ChecksumTool* ChecksumView::tool() const { return mTool; }

}

#endif
