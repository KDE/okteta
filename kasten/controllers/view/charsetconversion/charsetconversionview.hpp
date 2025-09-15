/*
    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CHARSETCONVERSIONVIEW_HPP
#define KASTEN_CHARSETCONVERSIONVIEW_HPP

// Okteta core
#include <Okteta/Byte>
// Qt
#include <QWidget>
// Std
#include <map>

namespace Okteta {
class ByteArrayComboBox;
}
class KComboBox;
class QCheckBox;
class QAction;

namespace Kasten {

class AbstractUserMessagesHandler;
class CharsetConversionTool;

class CharsetConversionView : public QWidget
{
    Q_OBJECT

public:
    explicit CharsetConversionView(CharsetConversionTool* tool,
                                   AbstractUserMessagesHandler* userMessagesHandler,
                                   QWidget* parent = nullptr);
    ~CharsetConversionView() override;

public:
    [[nodiscard]]
    CharsetConversionTool* tool() const;

private Q_SLOTS: // gui
    void onDefaultByteEditChanged(const QByteArray& byteArray);
    void onDefaultByteEditCodingChanged(int coding);
    void onConvertButtonClicked();

private Q_SLOTS: // tool
    void onApplyableChanged(bool isApplyable);
    void onConversionDone(bool success, int convertedBytesCount,
                          const std::map<Okteta::Byte, int>* failedPerByteCount);

private:
    CharsetConversionTool* const mTool;

    AbstractUserMessagesHandler* const m_userMessagesHandler;

    KComboBox* mDirectionComboBox;
    KComboBox* mOtherCharSetComboBox;
    QCheckBox* mSubstituteMissingCharCheckBox;
    Okteta::ByteArrayComboBox* mSubstituteByteEdit;
    QAction* mConvertAction;
};

inline CharsetConversionTool* CharsetConversionView::tool() const { return mTool; }

}

#endif
