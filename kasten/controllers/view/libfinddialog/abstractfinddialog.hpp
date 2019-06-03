/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2006-2007,2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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
