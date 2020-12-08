/*
    This file is part of the Okteta KPart module, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETAPARTFACTORY_HPP
#define OKTETAPARTFACTORY_HPP

// KF
#include <kparts_version.h>
#include <KPluginFactory>
#if KPARTS_VERSION < QT_VERSION_CHECK(5, 77, 0)
#include <KAboutData>
#endif

namespace Kasten {
class ByteArrayViewProfileManager;
class ModelCodecViewManager;
class ModelCodecManager;
}

class OktetaPartFactory : public KPluginFactory
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID KPluginFactory_iid FILE "oktetapart.json")

    Q_INTERFACES(
        KPluginFactory
    )

public:
    OktetaPartFactory();

    ~OktetaPartFactory() override;

public: // KPluginFactory API
    QObject* create(const char* iface,
                    QWidget* parentWidget,
                    QObject* parent,
                    const QVariantList& args,
                    const QString& keyword) override;

private:
#if KPARTS_VERSION < QT_VERSION_CHECK(5, 77, 0)
    KAboutData mAboutData;
#endif
    Kasten::ByteArrayViewProfileManager* mByteArrayViewProfileManager;
    Kasten::ModelCodecViewManager* mModelCodecViewManager;
    Kasten::ModelCodecManager* mModelCodecManager;
};

#endif
