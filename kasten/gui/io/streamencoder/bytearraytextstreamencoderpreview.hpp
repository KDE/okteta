/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYTEXTSTREAMENCODERPREVIEW_HPP
#define KASTEN_BYTEARRAYTEXTSTREAMENCODERPREVIEW_HPP

// lib
#include "oktetakastengui_export.hpp"
// Kasten ui
#include <Kasten/AbstractSelectionView>

class QTextEdit;

namespace Kasten {

class AbstractByteArrayStreamEncoder;
class AbstractModelSelection;
class AbstractModel;

class ByteArrayTextStreamEncoderPreview : public AbstractSelectionView
{
    Q_OBJECT

public:
    explicit ByteArrayTextStreamEncoderPreview(AbstractByteArrayStreamEncoder* encoder);
    ~ByteArrayTextStreamEncoderPreview() override;

public: // AbstractSelectionView API
    QWidget* widget() const override;
    void setData(AbstractModel* model, const AbstractModelSelection* selection) override;

public Q_SLOTS:
    void update();

private Q_SLOTS:
    void setFixedFontByGlobalSettings();

private:
    AbstractByteArrayStreamEncoder* const mEncoder;

    AbstractModel* mModel = nullptr;
    const AbstractModelSelection* mSelection = nullptr;

    QTextEdit* mWidget;
};

}

#endif
