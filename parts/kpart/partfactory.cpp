/*
    SPDX-FileCopyrightText: 2003, 2007, 2009, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "partfactory.hpp"
#include "partfactory-config.hpp"

// part
#include "part.hpp"
// Okteta Kasten
#include <Kasten/Okteta/ByteArrayViewProfileManager>
#include <Kasten/Okteta/ByteArrayStreamEncoderConfigEditorFactoryFactory>
#include <Kasten/Okteta/ByteArrayDataGeneratorConfigEditorFactoryFactory>
#include <Kasten/Okteta/AbstractByteArrayStreamEncoder>
#include <Kasten/Okteta/ByteArrayStreamEncoderFactory>
#include <Kasten/Okteta/ByteArrayDataGeneratorFactory>
// Kasten
#include <Kasten/AbstractModelDataGenerator>
#include <Kasten/AbstractModelDataGeneratorConfigEditorFactory>
#include <Kasten/AbstractModelStreamEncoderConfigEditorFactory>
#include <Kasten/ModelCodecViewManager>
#include <Kasten/ModelCodecManager>
#include <Kasten/UserMessagesHandler>
// KF
#include <KPluginMetaData>
// Std
#include <utility>

OktetaPartFactory::OktetaPartFactory()
    : mByteArrayViewProfileManager(std::make_unique<Kasten::ByteArrayViewProfileManager>())
    , m_userMessagesHandler(std::make_unique<Kasten::UserMessagesHandler>())
    , mModelCodecViewManager(std::make_unique<Kasten::ModelCodecViewManager>())
    , mModelCodecManager(std::make_unique<Kasten::ModelCodecManager>())
{

    auto streamEncoderList = Kasten::ByteArrayStreamEncoderFactory::createStreamEncoders();

    auto dataGeneratorList = Kasten::ByteArrayDataGeneratorFactory::createDataGenerators();

    auto encoderConfigEditorFactoryList =
        Kasten::ByteArrayStreamEncoderConfigEditorFactoryFactory::createFactorys();

    auto generatorConfigEditorFactoryList =
        Kasten::ByteArrayDataGeneratorConfigEditorFactoryFactory::createFactorys();

    mModelCodecManager->setStreamEncoders(std::move(streamEncoderList));
    mModelCodecManager->setDataGenerators(std::move(dataGeneratorList));

    mModelCodecViewManager->setStreamEncoderConfigEditorFactories(std::move(encoderConfigEditorFactoryList));
    mModelCodecViewManager->setDataGeneratorConfigEditorFactories(std::move(generatorConfigEditorFactoryList));
}

OktetaPartFactory::~OktetaPartFactory() = default;

QObject* OktetaPartFactory::create(const char* iface,
                                   QWidget* parentWidget,
                                   QObject* parent,
                                   const QVariantList& args,
                                   const QString& keyword)
{
    Q_UNUSED(parentWidget)
    Q_UNUSED(keyword)

    const OktetaPart::Modus modus =
        (args.contains(QStringLiteral("Browser/View")) ||
         (strcmp(iface, "Browser/View") == 0)) ?       OktetaPart::Modus::BrowserView :
        (strcmp(iface, "KParts::ReadOnlyPart") == 0) ? OktetaPart::Modus::ReadOnly :
        /* else */                                     OktetaPart::Modus::ReadWrite;

    auto* part = new OktetaPart(parent, metaData(), modus,
                                mByteArrayViewProfileManager.get(),
                                m_userMessagesHandler.get(),
                                mModelCodecManager.get(), mModelCodecViewManager.get());

    return part;
}

#include "moc_partfactory.cpp"
