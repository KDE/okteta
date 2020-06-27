/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "documentinfoview.hpp"

//
#include "documentinfotool.hpp"
// KF
#include <KLocalizedString>
#include <KIconLoader>
#include <KSeparator>
#include <KSqueezedTextLabel>
#include <KIO/Global>
// Qt
#include <QFont>
#include <QLabel>
#include <QLayout>
#include <QGridLayout>
#include <QLocale>
#include <QMimeType>

namespace Kasten {

DocumentInfoView::DocumentInfoView(DocumentInfoTool* tool, QWidget* parent)
    : QWidget(parent)
    , mTool(tool)
{
    auto* baseLayout = new QVBoxLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);

    // icon
    mIconLabel = new QLabel(this);
//     int bsize = 66 + 2 * mIconLabel->style()->pixelMetric( QStyle::PM_ButtonMargin );
//     mIconLabel->setFixedSize(bsize, bsize);
    mIconLabel->setFixedHeight(KIconLoader::SizeEnormous);
    mIconLabel->setMinimumWidth(KIconLoader::SizeEnormous);
    mIconLabel->setAlignment(Qt::AlignHCenter);
    baseLayout->addWidget(mIconLabel);

    // file label
    mDocumentTitleLabel = new QLabel(this);
    QFont font = mDocumentTitleLabel->font();
    font.setBold(true);
    mDocumentTitleLabel->setFont(font);
    mDocumentTitleLabel->setAlignment(Qt::AlignHCenter);
    mDocumentTitleLabel->setWordWrap(true);
    mDocumentTitleLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    baseLayout->addWidget(mDocumentTitleLabel);

    // separator
    KSeparator* separator = new KSeparator(Qt::Horizontal, this);
    baseLayout->addWidget(separator);

    // property grid
    auto* propertyGrid = new QGridLayout(); // unknown rows
    propertyGrid->setColumnStretch(0, 0);
    propertyGrid->setColumnStretch(1, 1);

    int currentPropertyRow = 0;

    // type property
    QLabel* label = new QLabel(i18n("Type:"), this);
    propertyGrid->addWidget(label, currentPropertyRow, 0, Qt::AlignRight);

    mMimeTypeLabel = new QLabel(QString(), this);
    propertyGrid->addWidget(mMimeTypeLabel, currentPropertyRow++, 1);

    // location property
    label = new QLabel(i18n("Location:"), this);
    propertyGrid->addWidget(label, currentPropertyRow, 0, Qt::AlignRight);

    mLocationLabel = new KSqueezedTextLabel(this);
    // force the layout direction to be always LTR
    mLocationLabel->setLayoutDirection(Qt::LeftToRight);
    // but if we are in RTL mode, align the text to the right
    // otherwise the text is on the wrong side of the dialog
    if (layoutDirection() == Qt::RightToLeft) {
        mLocationLabel->setAlignment(Qt::AlignRight);
    }
    // TODO: for some reason if building with enable_final flag the compiler sees
    // an ambiguous conversion without the explicit Qt::TextInteractionFlags(...)
    mLocationLabel->setTextInteractionFlags(Qt::TextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard));
    propertyGrid->addWidget(mLocationLabel, currentPropertyRow++, 1);

    // size property
    label = new QLabel(i18n("Size:"), this);
    propertyGrid->addWidget(label, currentPropertyRow, 0, Qt::AlignRight);

    mSizeLabel = new QLabel(this);
    propertyGrid->addWidget(mSizeLabel, currentPropertyRow++, 1);

#if 0
    label = new QLabel(i18n("Created/Loaded:"), this);   // TODO: make adjustable depending on document
    propertyGrid->addWidget(label, currentPropertyRow, 0, Qt::AlignRight);
    currentPropertyRow++;

    label = new QLabel(i18n("Last modified:"), this);
    propertyGrid->addWidget(label, currentPropertyRow, 0, Qt::AlignRight);
    currentPropertyRow++;

    label = new QLabel(i18n("Last synchronized:"), this);
    propertyGrid->addWidget(label, currentPropertyRow, 0, Qt::AlignRight);
    currentPropertyRow++;
// last accessed from remote

    KDateTime dt;// = item.time(KFileItem::CreationTime);
    if (!dt.isNull()) {
        label = new QLabel(i18n("Created:"), this);
        propertyGrid->addWidget(label, currentPropertyRow, 0, Qt::AlignRight);

        label = new QLabel(KLocale::global()->formatDateTime(dt), this);
        propertyGrid->addWidget(label, currentPropertyRow++, 2);
    }
#endif

    baseLayout->addLayout(propertyGrid);
    baseLayout->addStretch(10);

    connect(mTool, &DocumentInfoTool::documentTitleChanged,
            this, &DocumentInfoView::onDocumentTitleChanged);
    connect(mTool, &DocumentInfoTool::documentMimeTypeChanged,
            this, &DocumentInfoView::onMimeTypeChanged);
    connect(mTool, &DocumentInfoTool::locationChanged,
            this, &DocumentInfoView::onLocationChanged);
    connect(mTool, &DocumentInfoTool::documentSizeChanged,
            this, &DocumentInfoView::onDocumentSizeChanged);
    onDocumentTitleChanged(mTool->documentTitle());
    onMimeTypeChanged(mTool->mimeType());
    onLocationChanged(mTool->location());
    onDocumentSizeChanged(mTool->documentSize());
}

DocumentInfoView::~DocumentInfoView() = default;

void DocumentInfoView::onDocumentTitleChanged(const QString& documentTitle)
{
    mDocumentTitleLabel->setText(documentTitle);
}

void DocumentInfoView::onMimeTypeChanged(const QMimeType& mimeType)
{
    QString mimeTypeComment;
    QPixmap mimeTypeIcon;

    if (!mimeType.isValid()) {
        mimeTypeComment = QStringLiteral("-");
//         mimeTypeIcon = ?
    } else {
        mimeTypeComment = mimeType.comment();
        mimeTypeIcon = KIconLoader::global()->loadIcon(mimeType.iconName(), KIconLoader::Desktop, KIconLoader::SizeEnormous);
    }

    mIconLabel->setPixmap(mimeTypeIcon);
    mMimeTypeLabel->setText(mimeTypeComment);
}

void DocumentInfoView::onLocationChanged(const QString& location)
{
    const QString entry = location.isEmpty() ?
                          i18nc("There is no storage location assigned to yet.", "[None]") :
                          location;
    mLocationLabel->setText(entry);
}

void DocumentInfoView::onDocumentSizeChanged(int newSize)
{
    const QString size = (newSize != -1) ?
                         KIO::convertSize(newSize) + QLatin1String(" (") + QLocale().toString(newSize) + QLatin1Char(')') :
                         QStringLiteral("-");
    mSizeLabel->setText(size);
}

}
