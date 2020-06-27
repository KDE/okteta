/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef VERSIONABLEIFTEST_HPP
#define VERSIONABLEIFTEST_HPP

// Qt
#include <QObject>

class QSignalSpy;

namespace Okteta {

class Versionable;

class VersionableIfTest : public QObject
{
    Q_OBJECT

protected:
    VersionableIfTest();

protected: // our API
    virtual QObject* createVersionable() = 0;
    virtual void deleteVersionable(QObject* versionable) = 0;

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
    /** pointer to the class to test */
    QObject* mObject = nullptr;
    Versionable* mVersionableControl = nullptr;

    QSignalSpy* mRevertedToVersionIndexSpy = nullptr;
    QSignalSpy* mHeadVersionDescriptionChangedSpy = nullptr;
    QSignalSpy* mHeadVersionChangedSpy = nullptr;
};

inline VersionableIfTest::VersionableIfTest() = default;

}

#endif
