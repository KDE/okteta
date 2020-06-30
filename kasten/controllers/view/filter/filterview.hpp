/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_FILTERVIEW_HPP
#define KASTEN_FILTERVIEW_HPP

// Kasten gui
#include <Kasten/AbstractToolWidget>

class AbstractByteArrayFilterParameterSet;

class QPushButton;
class KComboBox;

class QStackedWidget;
class QString;

namespace Kasten {

class FilterTool;

class FilterView : public AbstractToolWidget
{
    Q_OBJECT

public:
    explicit FilterView(FilterTool* tool, QWidget* parent = nullptr);
    ~FilterView() override;

public:
    FilterTool* tool() const;

private: // get
    void getParameterSet(AbstractByteArrayFilterParameterSet* parameterSet) const;

private: // separation
    void addFilters();

private Q_SLOTS:
    void onFilterClicked();
    void onOperationChange(int index);
    void onHasWriteableChanged(bool hasWriteable);
    void onCharCodecChanged(const QString& charCodecName);
    void onValidityChanged(bool isValid);

private:
    FilterTool* mTool;
    KComboBox* mOperationComboBox;
    QStackedWidget* mParameterSetEditStack;
    QPushButton* mFilterButton;
};

inline FilterTool* FilterView::tool() const { return mTool; }

}

#endif
