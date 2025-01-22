/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef ABSTRACTBYTEARRAYVIEWIFTEST_HPP
#define ABSTRACTBYTEARRAYVIEWIFTEST_HPP

// Qt
#include <QObject>

class QSignalSpy;

namespace Okteta {

class AbstractByteArrayModel;
class AbstractByteArrayView;

class AbstractByteArrayViewIfTest : public QObject
{
    Q_OBJECT

protected:
    AbstractByteArrayViewIfTest();

protected: // our API
    virtual AbstractByteArrayView* createByteArrayView() = 0;
    virtual void deleteByteArrayView(AbstractByteArrayView* byteArrayView) = 0;

private:
    void checkZoomScaleChanged(double zoomScaleExpected);
    void clearSignalSpys();

private Q_SLOTS: // test functions
    void init();
    void cleanup();

    void testNormalZoomScale();
    void testZoomLevels();

private: // used in all tests
    /** pointer to the view to test */
    AbstractByteArrayView* m_byteArrayView = nullptr;

    AbstractByteArrayModel* m_byteArrayModel = nullptr;

    QSignalSpy* m_zoomScaleSpy = nullptr;
};

inline AbstractByteArrayViewIfTest::AbstractByteArrayViewIfTest() = default;

}

#endif
