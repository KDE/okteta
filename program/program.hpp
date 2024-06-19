/*
    This file is part of the Okteta program, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2007, 2011, 2014 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#ifndef OKTETAPROGRAM_HPP
#define OKTETAPROGRAM_HPP

// Qt
#include <QApplication>
// Std
#include <memory>

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
    OktetaProgram(const OktetaProgram&) = delete;
    ~OktetaProgram();

    OktetaProgram& operator=(const OktetaProgram&) = delete;

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

    // in (reverse) order of destruction
    std::unique_ptr<ByteArrayViewProfileManager> mByteArrayViewProfileManager;

    std::unique_ptr<DialogHandler> mDialogHandler;

    std::unique_ptr<DocumentManager> mDocumentManager;
    std::unique_ptr<ViewManager> mViewManager;
    std::unique_ptr<MultiDocumentStrategy> mDocumentStrategy;
};

inline DocumentManager* OktetaProgram::documentManager() { return mDocumentManager.get(); }
inline ViewManager* OktetaProgram::viewManager()         { return mViewManager.get(); }
inline MultiDocumentStrategy* OktetaProgram::documentStrategy() { return mDocumentStrategy.get(); }
inline ByteArrayViewProfileManager* OktetaProgram::byteArrayViewProfileManager() { return mByteArrayViewProfileManager.get(); }

}

#endif
