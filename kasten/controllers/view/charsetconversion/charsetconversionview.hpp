/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CHARSETCONVERSIONVIEW_HPP
#define KASTEN_CHARSETCONVERSIONVIEW_HPP

// Okteta core
#include <Okteta/Byte>
// Qt
#include <QWidget>

namespace Okteta {
class ByteArrayComboBox;
}
class KComboBox;
class QPushButton;
class QCheckBox;
template <class Key, class T> class QMap;

namespace Kasten {

class CharsetConversionTool;

class CharsetConversionView : public QWidget
{
    Q_OBJECT

public:
    explicit CharsetConversionView(CharsetConversionTool* tool, QWidget* parent = nullptr);
    ~CharsetConversionView() override;

public:
    CharsetConversionTool* tool() const;

private Q_SLOTS: // gui
    void onDefaultByteEditChanged(const QByteArray& byteArray);
    void onConvertButtonClicked();

private Q_SLOTS: // tool
    void onApplyableChanged(bool isApplyable);
    void onConversionDone(bool success, int convertedBytesCount,
                          const QMap<Okteta::Byte, int>& failedPerByteCount);

private:
    CharsetConversionTool* mTool;

    KComboBox* mDirectionComboBox;
    KComboBox* mOtherCharSetComboBox;
    QCheckBox* mSubstituteMissingCharCheckBox;
    Okteta::ByteArrayComboBox* mSubstituteByteEdit;
    QPushButton* mConvertButton;
};

inline CharsetConversionTool* CharsetConversionView::tool() const { return mTool; }

}

#endif
