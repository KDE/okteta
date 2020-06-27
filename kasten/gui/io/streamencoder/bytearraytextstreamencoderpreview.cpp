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
{
    mWidget = new QTextEdit(); // TODO: use Kate for syntax highlighting
    mWidget->setReadOnly(true);
    mWidget->setLineWrapMode(QTextEdit::NoWrap);
    mWidget->setToolTip(i18n("The preview uses maximal the first 100 bytes."));

    // TODO: find a signal/event emitted when fixedfont changes
//     connect( KGlobalSettings::self(), &KGlobalSettings::kdisplayFontChanged,
//              this, &ByteArrayTextStreamEncoderPreview::setFixedFontByGlobalSettings );
    setFixedFontByGlobalSettings();

    connect(mEncoder, &AbstractByteArrayStreamEncoder::settingsChanged, this, &ByteArrayTextStreamEncoderPreview::update);
}

ByteArrayTextStreamEncoderPreview::~ByteArrayTextStreamEncoderPreview()
{
    delete mWidget;
}

QWidget* ByteArrayTextStreamEncoderPreview::widget() const { return mWidget; }

void ByteArrayTextStreamEncoderPreview::setData(AbstractModel* model, const AbstractModelSelection* selection)
{
    mModel = model;
    mSelection = selection;

    update();
}

void ByteArrayTextStreamEncoderPreview::update()
{
    if (mModel) {
        mWidget->setText(mEncoder->previewData(mModel, mSelection));
    }
}

void ByteArrayTextStreamEncoderPreview::setFixedFontByGlobalSettings()
{
    mWidget->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
}

}
