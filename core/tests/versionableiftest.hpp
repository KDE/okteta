/*
    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef VERSIONABLEIFTEST_HPP
#define VERSIONABLEIFTEST_HPP

// Qt
#include <QObject>
// Std
#include <memory>

class QSignalSpy;

namespace Okteta {

class Versionable;

class VersionableIfTest : public QObject
{
    Q_OBJECT

public:
    ~VersionableIfTest() override;

protected:
    VersionableIfTest();

protected: // our API
    virtual std::unique_ptr<QObject> createVersionable() = 0;
    virtual void deleteVersionable(std::unique_ptr<QObject>&& versionable) = 0;

private:
    void checkRevertedToVersionIndex(int versionIndex);
    void checkHeadVersionDescriptionChanged(const QString& versionDescription);
    void checkHeadVersionChanged(int newHeadVersionIndex);
    void clearSignalSpys();

private Q_SLOTS: // test functions
    void init();
    void cleanup();

    void testBegin();

private: // used in all tests
    /** the object to test */
    std::unique_ptr<QObject> mObject;
    Versionable* mVersionableControl = nullptr;

    std::unique_ptr<QSignalSpy> mRevertedToVersionIndexSpy;
    std::unique_ptr<QSignalSpy> mHeadVersionDescriptionChangedSpy;
    std::unique_ptr<QSignalSpy> mHeadVersionChangedSpy;
};

}

#endif
