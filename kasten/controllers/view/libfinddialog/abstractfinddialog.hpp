/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTFINDDIALOG_HPP
#define KASTEN_ABSTRACTFINDDIALOG_HPP

// lib
#include "finddirection.hpp"
// Qt
#include <QDialog>
#include <QByteArray>
#include <QString>

namespace Okteta {
class ByteArrayComboBox;
}

class QGroupBox;
class QCheckBox;
class QPushButton;
class QVBoxLayout;

namespace Kasten {

class AbstractFindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AbstractFindDialog(QWidget* parent = nullptr);
    ~AbstractFindDialog() override;

public: // set
    void setDirection(FindDirection Direction);
    void setInSelection(bool InSelection);
    void setInSelectionEnabled(bool inSelectionEnabled);

public: // get
    QByteArray data() const;
    bool fromCursor() const;
    bool inSelection() const;
    Qt::CaseSensitivity caseSensitivity() const;
    FindDirection direction() const;

public Q_SLOTS:
    void setCharCodec(const QString& codecName);

protected: // QWidget API
    void showEvent(QShowEvent* e) override;

protected:
    void setFindButton(const QString& buttonText, const QString& buttonIconName,
                       const QString& buttonToolTip, const QString& buttonWhatsThis);
    void setFindButtonEnabled(bool enabled);
    void setupFindBox();
    void setupOperationBox(QGroupBox* operationBox = nullptr);
    void setupCheckBoxes(QCheckBox* optionCheckBox = nullptr);

protected: // API to be implemented
    virtual void onFindButtonClicked();
    virtual void rememberCurrentSettings();

private Q_SLOTS:
    void onSearchDataChanged(const QByteArray& ata);
    void onSearchDataFormatChanged(int Format);
    void onSelectedToggled(bool checked);
    void forwardFindButtonClicked();

private:
    QVBoxLayout* MainWidgetLayout;
    Okteta::ByteArrayComboBox* SearchDataEdit;
    QCheckBox* BackwardsCheckBox;
    QCheckBox* AtCursorCheckBox;
    QCheckBox* SelectedCheckBox;
    QCheckBox* WholeWordsCheckBox;
    QCheckBox* CaseSensitiveCheckBox;
    QPushButton* FindButton;
};

}

#endif
