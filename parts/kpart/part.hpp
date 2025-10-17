/*
    SPDX-FileCopyrightText: 2003, 2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETAPART_HPP
#define OKTETAPART_HPP

// Kasten
#include <Kasten/KastenCore>
// KF
#include <KParts/ReadWritePart>
// Std
#include <vector>
#include <memory>

class KJob;

namespace Kasten {
class ByteArrayViewProfileManager;
class ByteArrayView;
class ByteArrayDocument;
class SingleViewArea;
class ModelCodecViewManager;
class ModelCodecManager;
class UserMessagesHandler;
class AbstractXmlGuiController;
class AbstractXmlGuiControllerFactory;
}
class KPluginMetaData;

class OktetaPart : public KParts::ReadWritePart
{
    Q_OBJECT

    friend class OktetaBrowserExtension;

public:
    enum Modus
    {
        ReadOnly = 0,
        BrowserView = 1,
        ReadWrite = 2,
        _ModusCount,
    };
    static inline constexpr auto ModusCount = static_cast<std::size_t>(_ModusCount);

public:
    OktetaPart(QObject* parent,
               const KPluginMetaData& metaData,
               Modus modus,
               Kasten::ByteArrayViewProfileManager* viewProfileManager,
               Kasten::UserMessagesHandler* userMessagesHandler,
               Kasten::ModelCodecManager* modelCodecManager,
               Kasten::ModelCodecViewManager* modelCodecViewManager);

    ~OktetaPart() override;

public: // KParts::ReadWritePart API
    void setReadWrite(bool readWrite = true) override;

Q_SIGNALS:
    void hasSelectedDataChanged(bool hasSelectedData);

protected: // KParts::ReadWritePart API
    [[nodiscard]]
    bool saveFile() override;

protected: // KParts::ReadOnlyPart API
    [[nodiscard]]
    bool openFile() override;

private:
    void setupActions(bool browserViewWanted);
    void addController(const Kasten::AbstractXmlGuiControllerFactory& factory);

private:
    [[nodiscard]]
    Kasten::ByteArrayView* byteArrayView() const;

private Q_SLOTS:
    void onDocumentLoadJobResult(KJob* job);
    void onModified(Kasten::LocalSyncState state);

private:
    const Modus mModus;
    Kasten::ByteArrayViewProfileManager* const mViewProfileManager;

    // in (reverse) order of destruction
    std::unique_ptr<Kasten::ByteArrayDocument> mDocument;
    std::unique_ptr<Kasten::ByteArrayView> mByteArrayView;

    std::unique_ptr<Kasten::SingleViewArea> mSingleViewArea;
    std::vector<std::unique_ptr<Kasten::AbstractXmlGuiController>> mControllers;
};

inline Kasten::ByteArrayView* OktetaPart::byteArrayView() const { return mByteArrayView.get(); }

#endif
