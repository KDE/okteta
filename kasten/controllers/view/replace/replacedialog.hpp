/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2007, 2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_REPLACEDIALOG_HPP
#define KASTEN_REPLACEDIALOG_HPP

// lib
#include <abstractfinddialog.hpp>
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayComboBox>

namespace Kasten {

class ReplaceTool;

class ReplaceDialog : public AbstractFindDialog
{
    Q_OBJECT

private:
    static inline constexpr char ConfigGroupId[] = "ReplaceTool";

    static inline constexpr char FromCursorConfigKey[] = "FromCursor";
    static inline constexpr char DirectionConfigKey[] = "Direction";
    static inline constexpr char InSelectionConfigKey[] = "InSelection";
    static inline constexpr char ReplaceDataCodingConfigKey[] = "ReplaceDataCoding";
    static inline constexpr char SearchDataCodingConfigKey[] = "SearchDataCoding";

    static inline constexpr bool DefaultFromCursor = false;
    static inline constexpr Kasten::FindDirection DefaultDirection = Kasten::FindForward;
    static inline constexpr bool DefaultInSelection = false;
    static inline constexpr Okteta::ByteArrayComboBox::Coding DefaultReplaceDataCoding = Okteta::ByteArrayComboBox::HexadecimalCoding;
    static inline constexpr Okteta::ByteArrayComboBox::Coding DefaultSearchDataCoding = Okteta::ByteArrayComboBox::HexadecimalCoding;

public:
    explicit ReplaceDialog(ReplaceTool* tool, QWidget* parent = nullptr);
    ~ReplaceDialog() override;

public: // set
//     void setDirection( KReplaceDirection Direction );
    void setCharCodec(const QString& codecName);

public: // get
    [[nodiscard]]
    QByteArray replaceData() const;
    [[nodiscard]]
    bool prompt() const;

protected: // AbstractFindDialog API
    void onFindButtonClicked() override;
    void rememberCurrentSettings() override;

protected: // QWidget API
    void showEvent(QShowEvent* showEvent) override;

private:
    void setReplaceDataCoding(Okteta::ByteArrayComboBox::Coding replaceDataCoding);
    [[nodiscard]]
    Okteta::ByteArrayComboBox::Coding replaceDataCoding() const;

private:
    Okteta::ByteArrayComboBox* ReplaceDataEdit;
    QCheckBox* PromptCheckBox;

    ReplaceTool* const mTool;
};

}

#endif
