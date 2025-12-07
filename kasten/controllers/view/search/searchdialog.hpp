/*
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
    static const QString ConfigGroupId;

    static constexpr char FromCursorConfigKey[] = "FromCursor";
    static constexpr char DirectionConfigKey[] = "Direction";
    static constexpr char InSelectionConfigKey[] = "InSelection";
    static constexpr char SearchDataCodingConfigKey[] = "SearchDataCoding";

    static constexpr bool DefaultFromCursor = false;
    static constexpr Kasten::FindDirection DefaultDirection = Kasten::FindForward;
    static constexpr bool DefaultInSelection = false;
    static constexpr Okteta::ByteArrayComboBox::Coding DefaultSearchDataCoding = Okteta::ByteArrayComboBox::HexadecimalCoding;

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
