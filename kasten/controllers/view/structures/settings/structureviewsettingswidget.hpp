/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRUCTUREVIEWSETTINGSWIDGET_HPP
#define KASTEN_STRUCTUREVIEWSETTINGSWIDGET_HPP

#include <ui_structureviewsettingswidget.h>
#include <QWidget>

class KConfigSkeletonItem;

class StructureViewSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StructureViewSettingsWidget();
    ~StructureViewSettingsWidget() override;

private:
    void setupBasesCombo(QComboBox* box, KConfigSkeletonItem* configItem,
                         int currentValue, const char* slot);
    void handleMapping(int index, QComboBox* box, QSpinBox* spin);

private Q_SLOTS:
    void setCharDisplay(int index);
    void setSignedDisplay(int index);
    void setUnsignedDisplay(int index);

private:
    Ui_StructureViewSettingsWidget ui;
};

#endif
