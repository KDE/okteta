/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SEARCHDIALOG_HPP
#define KASTEN_SEARCHDIALOG_HPP

// lib
#include <abstractfinddialog.hpp>
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayComboBox>

namespace Kasten {

class SearchTool;

class SearchDialog : public AbstractFindDialog
{
    Q_OBJECT

private:
    static inline constexpr char ConfigGroupId[] = "SearchTool";

    static inline constexpr char FromCursorConfigKey[] = "FromCursor";
    static inline constexpr char DirectionConfigKey[] = "Direction";
    static inline constexpr char InSelectionConfigKey[] = "InSelection";
    static inline constexpr char SearchDataCodingConfigKey[] = "SearchDataCoding";

    static inline constexpr bool DefaultFromCursor = false;
    static inline constexpr Kasten::FindDirection DefaultDirection = Kasten::FindForward;
    static inline constexpr bool DefaultInSelection = false;
    static inline constexpr Okteta::ByteArrayComboBox::Coding DefaultSearchDataCoding = Okteta::ByteArrayComboBox::HexadecimalCoding;

public:
    explicit SearchDialog(SearchTool* tool, QWidget* parent = nullptr);
    ~SearchDialog() override;

protected: // AbstractFindDialog API
    void onFindButtonClicked() override;
    void rememberCurrentSettings() override;

protected: // QWidget API
    void showEvent(QShowEvent* showEvent) override;

private:
    SearchTool* const mTool;
};

}

#endif
