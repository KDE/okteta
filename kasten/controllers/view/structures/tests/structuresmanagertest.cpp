/*
    SPDX-FileCopyrightText: 2026 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

// sut
#include <structuresmanager.hpp>
#include <structureinstalljob.hpp>
#include <structureuninstalljob.hpp>
// KF
#include <KZip>
#include <KTar>
// Qt
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTest>

class StructuresManagerTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();

    void testInstallationRoundtrip_data();
    void testInstallationRoundtrip();
    void testOldUninstallation();

private:
    QString createTar(const QString& id,
                      const QString metadataFileName, const QByteArray& metadataContent,
                      const QString osdFileName, const QByteArray& osdFileContent);
    QString createZip(const QString& id,
                      const QString metadataFileName, const QByteArray& metadataContent,
                      const QString osdFileName, const QByteArray& osdFileContent);
    QString createDir(const QString& id,
                      const QString metadataFileName, const QByteArray& metadataContent,
                      const QString osdFileName, const QByteArray& osdFileContent);

    void deploySubdirFiles(const QString& id,
                           const QString metadataFileName, const QByteArray& metadataContent,
                           const QString osdFileName, const QByteArray& osdFileContent);
    void deployToplevelFiles(const QString metadataFileName, const QByteArray& metadataContent,
                             const QString osdFileName, const QByteArray& osdFileContent);

private:
    QTemporaryDir m_temporaryDir;
};

const QByteArray desktopFileContentTemplate = QByteArrayLiteral(
"[Desktop Entry]\n"
"Type=Service\n"
"ServiceTypes=KPluginInfo\n"
"\n"
"Icon=text/plain\n"
"Name=Char array\n"
"Comment=A test structure\n"
"\n"
"X-KDE-PluginInfo-Category=structure\n"
"X-KDE-PluginInfo-Name=%1\n"
);

const QByteArray osdFileContentTemplate = QByteArrayLiteral(
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<data>\n"
"    <array name=\"chars\" length=\"256\">\n"
"      <primitive name=\"val\" type=\"char\" />\n"
"    </array>\n"
"</data>\n"
);


QString StructuresManagerTest::createTar(const QString& id,
                                         const QString metadataFileName, const QByteArray& metadataContent,
                                         const QString osdFileName, const QByteArray& osdFileContent)
{
    const QString name = id + QLatin1String(".tgz");

    const QString path = m_temporaryDir.filePath(name);

    KTar archive(path);

    archive.open(QIODevice::WriteOnly);

    archive.writeFile(metadataFileName, metadataContent);
    archive.writeFile(osdFileName, osdFileContent);

    archive.close();

    return path;
}

QString StructuresManagerTest::createZip(const QString& id,
                                         const QString metadataFileName, const QByteArray& metadataContent,
                                         const QString osdFileName, const QByteArray& osdFileContent)
{
    const QString name = id + QLatin1String(".zip");

    const QString path = m_temporaryDir.filePath(name);

    KZip archive(path);

    archive.open(QIODevice::WriteOnly);

    archive.writeFile(metadataFileName, metadataContent);
    archive.writeFile(osdFileName, osdFileContent);

    archive.close();

    return path;
}

QString StructuresManagerTest::createDir(const QString& id,
                                         const QString metadataFileName, const QByteArray& metadataContent,
                                         const QString osdFileName, const QByteArray& osdFileContent)
{
    const QString name = id;

    QDir parentDir(m_temporaryDir.path());
    parentDir.mkdir(name);
    const QString path = parentDir.absoluteFilePath(name);;

    QFile metadataFile(m_temporaryDir.filePath(metadataFileName));
    metadataFile.open(QIODevice::WriteOnly);
    metadataFile.write(metadataContent);
    metadataFile.close();

    QFile osdFile(m_temporaryDir.filePath(osdFileName));
    osdFile.open(QIODevice::WriteOnly);
    osdFile.write(osdFileContent);
    osdFile.close();

    return path;
}

void StructuresManagerTest::deploySubdirFiles(const QString& id,
                                              const QString metadataFileName, const QByteArray& metadataContent,
                                              const QString osdFileName, const QByteArray& osdFileContent)
{
    const QString name = id;

    QDir structuresRootDir(Kasten::StructuresManager::userStructuresRootDir());
    structuresRootDir.mkpath(name);
    const QString path = structuresRootDir.absoluteFilePath(name);;

    QFile metadataFile(structuresRootDir.filePath(metadataFileName));
    metadataFile.open(QIODevice::WriteOnly);
    metadataFile.write(metadataContent);
    metadataFile.close();

    QFile osdFile(structuresRootDir.filePath(osdFileName));
    osdFile.open(QIODevice::WriteOnly);
    osdFile.write(osdFileContent);
    osdFile.close();
}

void StructuresManagerTest::deployToplevelFiles(const QString metadataFileName, const QByteArray& metadataContent,
                                                const QString osdFileName, const QByteArray& osdFileContent)
{
    QDir structuresRootDir(Kasten::StructuresManager::userStructuresRootDir());
    structuresRootDir.mkpath(QStringLiteral("."));

    QFile metadataFile(structuresRootDir.filePath(metadataFileName));
    metadataFile.open(QIODevice::WriteOnly);
    metadataFile.write(metadataContent);
    metadataFile.close();

    QFile osdFile(structuresRootDir.filePath(osdFileName));
    osdFile.open(QIODevice::WriteOnly);
    osdFile.write(osdFileContent);
    osdFile.close();
}

void StructuresManagerTest::initTestCase()
{
    // Hide any eventually installed osds
    QStandardPaths::setTestModeEnabled(true);
    qputenv("XDG_DATA_DIRS", "/oktetadummy");
}

void StructuresManagerTest::testInstallationRoundtrip_data()
{
    QTest::addColumn<QString>("id");
    QTest::addColumn<QString>("archivePath");
    QTest::addColumn<bool>("useEmptyStorage");

    {
        const QString id = QStringLiteral("tar");
        const QString metadataFileName = id + QLatin1String("/metadata.desktop");
        const QByteArray metadataContent = QByteArray(desktopFileContentTemplate).replace("%1", id.toLatin1());
        const QString osdFileName = id + QLatin1String("/main.osd");

        const QString archivePath = createTar(id, metadataFileName, metadataContent, osdFileName, osdFileContentTemplate);

        QTest::newRow("tar-empty") << id << archivePath << true;
        QTest::newRow("tar-filled") << id << archivePath << false;
    }
    {
        const QString id = QStringLiteral("oldtar");
        const QString metadataFileName = id + QLatin1Char('/') + id + QLatin1String(".desktop");
        const QByteArray metadataContent = QByteArray(desktopFileContentTemplate).replace("%1", id.toLatin1());
        const QString osdFileName = id + QLatin1Char('/') + id + QLatin1String(".osd");

        const QString archivePath = createTar(id, metadataFileName, metadataContent, osdFileName, osdFileContentTemplate);

        QTest::newRow("oldtar-empty") << id << archivePath << true;
        QTest::newRow("oldtar-filled") << id << archivePath << false;
    }
    {
        const QString id = QStringLiteral("zip");
        const QString metadataFileName = id + QLatin1String("/metadata.desktop");
        const QByteArray metadataContent = QByteArray(desktopFileContentTemplate).replace("%1", id.toLatin1());
        const QString osdFileName = id + QLatin1String("/main.osd");

        const QString archivePath = createZip(id, metadataFileName, metadataContent, osdFileName, osdFileContentTemplate);

        QTest::newRow("zip-empty") << id << archivePath << true;
        QTest::newRow("zip-filled") << id << archivePath << false;
    }
    {
        const QString id = QStringLiteral("oldzip");
        const QString metadataFileName = id + QLatin1Char('/') + id + QLatin1String(".desktop");
        const QByteArray metadataContent = QByteArray(desktopFileContentTemplate).replace("%1", id.toLatin1());
        const QString osdFileName = id + QLatin1Char('/') + id + QLatin1String(".osd");

        const QString archivePath = createZip(id, metadataFileName, metadataContent, osdFileName, osdFileContentTemplate);

        QTest::newRow("oldzip-empty") << id << archivePath << true;
        QTest::newRow("oldzip-filled") << id << archivePath << false;
    }
    {
        const QString id = QStringLiteral("dir");
        const QString metadataFileName = id + QLatin1String("/metadata.desktop");
        const QByteArray metadataContent = QByteArray(desktopFileContentTemplate).replace("%1", id.toLatin1());
        const QString osdFileName = id + QLatin1String("/main.osd");

        const QString archivePath = createDir(id, metadataFileName, metadataContent, osdFileName, osdFileContentTemplate);

        QTest::newRow("dir-empty") << id << archivePath << true;
        QTest::newRow("dir-filled") << id << archivePath << false;
    }
}

void StructuresManagerTest::testInstallationRoundtrip()
{
    QFETCH(QString, id);
    QFETCH(QString, archivePath);
    QFETCH(bool, useEmptyStorage);

    // clean-up any old files
    QVERIFY(QDir(Kasten::StructuresManager::userStructuresRootDir()).removeRecursively());

    Kasten::StructuresManager structureManager;

    const auto& structureDefs = structureManager.structureDefs();

    if (useEmptyStorage) {
        QVERIFY(structureDefs.empty());
    } else {
        // Manual install
        {
            const QString id = QStringLiteral("flat");
            const QString metadataFileName = id + QLatin1String(".desktop");
            const QByteArray metadataContent = QByteArray(desktopFileContentTemplate).replace("%1", id.toLatin1());
            const QString osdFileName = id + QLatin1String(".osd");

            deployToplevelFiles(metadataFileName, metadataContent, osdFileName, osdFileContentTemplate);
        }
        {
            const QString id = QStringLiteral("subdir");
            const QString metadataFileName = id + QLatin1String("/metadata.desktop");
            const QByteArray metadataContent = QByteArray(desktopFileContentTemplate).replace("%1", id.toLatin1());
            const QString osdFileName = id + QLatin1String("/main.osd");

            deploySubdirFiles(id, metadataFileName, metadataContent, osdFileName, osdFileContentTemplate);
        }

        structureManager.reloadPaths();

        QCOMPARE(structureDefs.size(), 2);
    }

    // Install...
    Kasten::StructureInstallJob* const installJob = structureManager.installStructureFromFile(QUrl::fromLocalFile(archivePath));
    const bool installJobSuccess = installJob->exec();
    if (!installJobSuccess) {
        qDebug() << "Install failed:" << installJob->errorString();
    }
    QVERIFY(installJobSuccess);

    if (useEmptyStorage) {
        QCOMPARE(structureDefs.size(), 1);
    } else {
        QCOMPARE(structureDefs.size(), 3);
    }

    Kasten::StructureDefinitionFile* const definition = structureManager.definition(id);

    QVERIFY(definition != nullptr);
    QVERIFY(definition->isValid());
    QCOMPARE(definition->location(), Kasten::StructureDefinitionFile::Location::SubDir);
    QCOMPARE(definition->metaData().id(), id);

    // Unnstall...
    Kasten::StructureUninstallJob* const uninstallJob = structureManager.uninstallStructure(id);
    const bool uninstallJobSuccess = uninstallJob->exec();
    if (!uninstallJobSuccess) {
        qDebug() << "Unnstall failed:" << uninstallJob->errorString();
    }
    QVERIFY(uninstallJobSuccess);

    if (useEmptyStorage) {
        QVERIFY(structureDefs.empty());
    } else {
        QCOMPARE(structureDefs.size(), 2);
    }
}

void StructuresManagerTest::testOldUninstallation()
{
    // clean-up any old files
    QVERIFY(QDir(Kasten::StructuresManager::userStructuresRootDir()).removeRecursively());

    Kasten::StructuresManager structureManager;

    const auto& structureDefs = structureManager.structureDefs();

    QVERIFY(structureDefs.empty());

    // Manual install
    const QString id = QStringLiteral("oldflat");
    const QString metadataFileName = id + QLatin1String(".desktop");
    const QByteArray metadataContent = QByteArray(desktopFileContentTemplate).replace("%1", id.toLatin1());
    const QString osdFileName = id + QLatin1String(".osd");

    deployToplevelFiles(metadataFileName, metadataContent, osdFileName, osdFileContentTemplate);

    structureManager.reloadPaths();

    QCOMPARE(structureDefs.size(), 1);

    Kasten::StructureDefinitionFile* const definition = structureManager.definition(id);

    QVERIFY(definition != nullptr);
    QVERIFY(definition->isValid());
    QCOMPARE(definition->location(), Kasten::StructureDefinitionFile::Location::ToplevelDir);
    QCOMPARE(definition->metaData().id(), id);

    // Unnstall...
    Kasten::StructureUninstallJob* const uninstallJob = structureManager.uninstallStructure(id);
    const bool uninstallJobSuccess = uninstallJob->exec();
    if (!uninstallJobSuccess) {
        qDebug() << "Unnstall failed:" << uninstallJob->errorString();
    }
    QVERIFY(uninstallJobSuccess);

    QVERIFY(structureDefs.empty());
}

QTEST_GUILESS_MAIN(StructuresManagerTest)

#include "structuresmanagertest.moc"
