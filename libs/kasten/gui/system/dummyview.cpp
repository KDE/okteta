/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "dummyview.hpp"

// Kasten core
#include <Kasten/AbstractDocument>
// Qt
#include <QLabel>

namespace Kasten {

DummyView::DummyView(AbstractDocument* document)
    : AbstractView(document)
    , mLabel(new QLabel())
    , mDocument(document)
{
    mLabel->setText(mDocument->title());

    connect(document, &Kasten::AbstractDocument::titleChanged,
            this, &DummyView::onTitleChange);
}

DummyView::~DummyView()
{
    delete mLabel;
}

bool DummyView::hasFocus()               const { return mLabel->hasFocus(); } // TODO: make sure, label really holds focus after setFocus()
QWidget* DummyView::widget()             const { return mLabel; }
QString DummyView::title()               const { return mDocument->title(); }

void DummyView::setFocus()
{
    mLabel->setFocus();
}

void DummyView::onTitleChange(const QString& newTitle)
{
    mLabel->setText(newTitle);
}

}
