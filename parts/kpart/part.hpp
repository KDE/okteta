/*
    This file is part of the Okteta KPart module, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETAPART_HPP
#define OKTETAPART_HPP

// Kasten
#include <Kasten/KastenCore>
// KF
#include <kparts_version.h>
#include <KParts/ReadWritePart>
// Qt
#include <QVector>

namespace Kasten {
class ByteArrayViewProfileManager;
class ByteArrayView;
class ByteArrayDocument;
class SingleViewArea;
class ModelCodecViewManager;
class ModelCodecManager;
class AbstractXmlGuiController;
class AbstractXmlGuiControllerFactory;
class AbstractDocument;
}
#if KPARTS_VERSION >= QT_VERSION_CHECK(5, 77, 0)
class KPluginMetaData;
#else
class KAboutData;
#endif
class QVBoxLayout;

class OktetaPart : public KParts::ReadWritePart
{
    Q_OBJECT

    friend class OktetaBrowserExtension;

public:
    enum Modus
    {
        ReadOnly = 0,
        BrowserView = 1,
        ReadWrite = 2
    };

public:
    OktetaPart(QObject* parent,
#if KPARTS_VERSION >= QT_VERSION_CHECK(5, 77, 0)
               const KPluginMetaData& metaData,
#else
               const KAboutData& componentData,
#endif
               Modus modus,
               Kasten::ByteArrayViewProfileManager* viewProfileManager,
               Kasten::ModelCodecManager* modelCodecManager,
               Kasten::ModelCodecViewManager* modelCodecViewManager);

    ~OktetaPart() override;

public: // KParts::ReadWritePart API
    void setReadWrite(bool readWrite = true) override;

Q_SIGNALS:
    void hasSelectedDataChanged(bool hasSelectedData);

protected: // KParts::ReadWritePart API
    bool saveFile() override;

protected: // KParts::ReadOnlyPart API
    bool openFile() override;

private:
    void setupActions(bool browserViewWanted);
    void addController(const Kasten::AbstractXmlGuiControllerFactory& factory);

private:
    Kasten::ByteArrayView* byteArrayView() const;

private Q_SLOTS:
    void onDocumentLoaded(Kasten::AbstractDocument* document);
    void onModified(Kasten::LocalSyncState state);

private:
    const Modus mModus;
    QVBoxLayout* mLayout;

    Kasten::ByteArrayDocument* mDocument;
    Kasten::ByteArrayView* mByteArrayView;

    Kasten::SingleViewArea* mSingleViewArea;
    QVector<Kasten::AbstractXmlGuiController*> mControllers;

    Kasten::ByteArrayViewProfileManager* mViewProfileManager;
};

inline Kasten::ByteArrayView* OktetaPart::byteArrayView() const { return mByteArrayView; }

#endif
