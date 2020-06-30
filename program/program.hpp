/*
    This file is part of the Okteta program, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2007, 2011, 2014 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#ifndef OKTETAPROGRAM_HPP
#define OKTETAPROGRAM_HPP

// Qt
#include <QApplication>

namespace Kasten {

class DialogHandler;
class DocumentManager;
class ViewManager;
class MultiDocumentStrategy;
// tmp
class ByteArrayViewProfileManager;

class OktetaProgram
{
public:
    OktetaProgram(int& argc, char* argv[]);
    ~OktetaProgram();

public:
    int execute();
    void quit();

public:
    DocumentManager* documentManager();
    ViewManager* viewManager();
    MultiDocumentStrategy* documentStrategy();
    ByteArrayViewProfileManager* byteArrayViewProfileManager();

private:
    QApplication mApp;

    DocumentManager* mDocumentManager;
    ViewManager* mViewManager;
    MultiDocumentStrategy* mDocumentStrategy;

    DialogHandler* mDialogHandler;

    ByteArrayViewProfileManager* mByteArrayViewProfileManager;
};

inline DocumentManager* OktetaProgram::documentManager() { return mDocumentManager; }
inline ViewManager* OktetaProgram::viewManager()         { return mViewManager; }
inline MultiDocumentStrategy* OktetaProgram::documentStrategy() { return mDocumentStrategy; }
inline ByteArrayViewProfileManager* OktetaProgram::byteArrayViewProfileManager() { return mByteArrayViewProfileManager; }

}

#endif
