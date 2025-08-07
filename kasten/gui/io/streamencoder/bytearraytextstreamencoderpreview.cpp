/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraytextstreamencoderpreview.hpp"

// lib
#include "abstractbytearraystreamencoder.hpp"
// KF
#include <KLocalizedString>
// Qt
#include <QFontDatabase>
#include <QTextEdit>

namespace Kasten {

ByteArrayTextStreamEncoderPreview::ByteArrayTextStreamEncoderPreview(AbstractByteArrayStreamEncoder* encoder)
    : mEncoder(encoder)
    , mWidget(std::make_unique<QTextEdit>()) // TODO: use KSyntaxHighlighting for syntax highlighting
{
    mWidget->setReadOnly(true);
    mWidget->setLineWrapMode(QTextEdit::NoWrap);
    mWidget->setToolTip(i18nc("@info:tooltip", "The preview uses maximally the first 100 bytes."));

    // TODO: find a signal/event emitted when fixedfont changes
//     connect( KGlobalSettings::self(), &KGlobalSettings::kdisplayFontChanged,
//              this, &ByteArrayTextStreamEncoderPreview::setFixedFontByGlobalSettings );
    setFixedFontByGlobalSettings();

    connect(mEncoder, &AbstractByteArrayStreamEncoder::settingsChanged, this, &ByteArrayTextStreamEncoderPreview::update);
}

ByteArrayTextStreamEncoderPreview::~ByteArrayTextStreamEncoderPreview() = default;

QWidget* ByteArrayTextStreamEncoderPreview::widget() const { return mWidget.get(); }

void ByteArrayTextStreamEncoderPreview::setData(AbstractModel* model, const AbstractModelSelection* selection)
{
    mModel = model;
    mSelection = selection;

    update();
}

void ByteArrayTextStreamEncoderPreview::update()
{
    if (mModel) {
        const QByteArray data = mEncoder->previewData(mModel, mSelection);
        // TODO: assuming text streams for now
        // encoders use QTextStream with default local codecs, so use fromLocal8Bit here
        const QString text = QString::fromLocal8Bit(data);
        mWidget->setText(text);
    }
}

void ByteArrayTextStreamEncoderPreview::setFixedFontByGlobalSettings()
{
    mWidget->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
}

}

#include "moc_bytearraytextstreamencoderpreview.cpp"
