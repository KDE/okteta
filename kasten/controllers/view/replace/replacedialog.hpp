/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_REPLACEDIALOG_HPP
#define KASTEN_REPLACEDIALOG_HPP

// lib
#include <abstractfinddialog.hpp>

namespace Kasten {

class ReplaceTool;

class ReplaceDialog : public AbstractFindDialog
{
    Q_OBJECT

public:
    explicit ReplaceDialog(ReplaceTool* tool, QWidget* parent = nullptr);
    ~ReplaceDialog() override;

public: // set
//     void setDirection( KReplaceDirection Direction );
    void setCharCodec(const QString& codecName);

public: // get
    QByteArray replaceData() const;
    bool prompt() const;

protected: // AbstractFindDialog API
    void onFindButtonClicked() override;
    void rememberCurrentSettings() override;

protected: // QWidget API
    void showEvent(QShowEvent* showEvent) override;

private:
    Okteta::ByteArrayComboBox* ReplaceDataEdit;
    QCheckBox* PromptCheckBox;

    ReplaceTool* mTool;
};

}

#endif
