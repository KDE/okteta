/*
    This file is part of the Okteta KPart module, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2007, 2009, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "partfactory.hpp"

// part
#include "part.hpp"
// Okteta Kasten
#include <Kasten/Okteta/ByteArrayViewProfileManager>
#include <Kasten/Okteta/ByteArrayStreamEncoderConfigEditorFactoryFactory>
#include <Kasten/Okteta/ByteArrayDataGeneratorConfigEditorFactoryFactory>
#include <Kasten/Okteta/ByteArrayStreamEncoderFactory>
#include <Kasten/Okteta/ByteArrayDataGeneratorFactory>
// Kasten
#include <Kasten/ModelCodecViewManager>
#include <Kasten/ModelCodecManager>
// KF
#include <KPluginMetaData>
#include <KLocalizedString>

OktetaPartFactory::OktetaPartFactory()
{
    mByteArrayViewProfileManager = new Kasten::ByteArrayViewProfileManager();

    mModelCodecViewManager = new Kasten::ModelCodecViewManager();
    mModelCodecManager = new Kasten::ModelCodecManager();

    const QVector<Kasten::AbstractModelStreamEncoder*> encoderList =
        Kasten::ByteArrayStreamEncoderFactory::createStreamEncoders();

    const QVector<Kasten::AbstractModelDataGenerator*> generatorList =
        Kasten::ByteArrayDataGeneratorFactory::createDataGenerators();

    const QVector<Kasten::AbstractModelStreamEncoderConfigEditorFactory*> encoderConfigEditorFactoryList =
        Kasten::ByteArrayStreamEncoderConfigEditorFactoryFactory::createFactorys();

    const QVector<Kasten::AbstractModelDataGeneratorConfigEditorFactory*> generatorConfigEditorFactoryList =
        Kasten::ByteArrayDataGeneratorConfigEditorFactoryFactory::createFactorys();

    mModelCodecManager->setEncoders( encoderList );
    mModelCodecManager->setGenerators( generatorList );

    mModelCodecViewManager->setEncoderConfigEditorFactories(encoderConfigEditorFactoryList);
    mModelCodecViewManager->setGeneratorConfigEditorFactories(generatorConfigEditorFactoryList);
}

OktetaPartFactory::~OktetaPartFactory()
{
    delete mByteArrayViewProfileManager;
    delete mModelCodecViewManager;
    delete mModelCodecManager;
}

QObject* OktetaPartFactory::create(const char* iface,
                                   QWidget* parentWidget,
                                   QObject* parent,
                                   const QVariantList& args,
                                   const QString& keyword)
{
    Q_UNUSED(parentWidget)
    Q_UNUSED(keyword);

    const OktetaPart::Modus modus =
        (args.contains(QStringLiteral("Browser/View")) ||
         (strcmp(iface, "Browser/View") == 0)) ?       OktetaPart::Modus::BrowserView :
        (strcmp(iface, "KParts::ReadOnlyPart") == 0) ? OktetaPart::Modus::ReadOnly :
        /* else */                                     OktetaPart::Modus::ReadWrite;

    auto* part = new OktetaPart(parent, metaData(), modus, mByteArrayViewProfileManager, mModelCodecManager, mModelCodecViewManager);

    return part;
}
