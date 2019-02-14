/*
    This file is part of the Okteta KPart module, made within the KDE community.

    Copyright 2003,2007,2009,2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
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
// KF5
#include <KLocalizedString>

OktetaPartFactory::OktetaPartFactory()
    : mAboutData(QStringLiteral("oktetapart"),
                 i18n("OktetaPart"),
                 QStringLiteral("0.13.60"),
                 i18n("Embedded hex editor"),
                 KAboutLicense::GPL_V2,
                 i18n("2003-2014 Friedrich W. H. Kossebau"))
{
    mAboutData.addAuthor(i18n("Friedrich W. H. Kossebau"), i18n("Author"), QStringLiteral("kossebau@kde.org"));

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
    Q_UNUSED(args)
    Q_UNUSED(keyword);

    const QByteArray className(iface);
    const OktetaPart::Modus modus =
        (className == "KParts::ReadOnlyPart") ? OktetaPart::Modus::ReadOnly :
        (className == "Browser/View") ?         OktetaPart::Modus::BrowserView :
        /* else */                              OktetaPart::Modus::ReadWrite;

    OktetaPart* part = new OktetaPart(parent, mAboutData, modus, mByteArrayViewProfileManager, mModelCodecManager, mModelCodecViewManager);

    return part;
}
