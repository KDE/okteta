/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SEARCHDIALOG_HPP
#define KASTEN_SEARCHDIALOG_HPP

// lib
#include <abstractfinddialog.hpp>

namespace Kasten {

class SearchTool;

class SearchDialog : public AbstractFindDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(SearchTool* tool, QWidget* parent = nullptr);
    ~SearchDialog() override;

protected: // AbstractFindDialog API
    void onFindButtonClicked() override;

protected: // QWidget API
    void showEvent(QShowEvent* showEvent) override;

private:
    SearchTool* mTool;
};

}

#endif
