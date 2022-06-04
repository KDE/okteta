/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef ABOUTSTRUCTUREDIALOG_HPP
#define ABOUTSTRUCTUREDIALOG_HPP

// tool
#include "../structuremetadata.hpp"
// Qt
#include <QDialog>

class AboutStructureDialog : public QDialog
{
    Q_OBJECT

public:
    AboutStructureDialog(const StructureMetaData& structureMetaData, QWidget* parent);

private:
    void showLicenseDialog();
    void emailAuthor();

private:
    const StructureMetaData m_structureMetaData;
};

#endif
