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
// Std
#include <memory>

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
    const std::unique_ptr<Kasten::ByteArrayViewProfileManager> mByteArrayViewProfileManager;
    const std::unique_ptr<Kasten::ModelCodecViewManager> mModelCodecViewManager;
    const std::unique_ptr<Kasten::ModelCodecManager> mModelCodecManager;
};

#endif
