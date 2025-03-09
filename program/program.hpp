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
class UserMessagesHandler;
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
    OktetaProgram(OktetaProgram&&) = delete;
    ~OktetaProgram();

    OktetaProgram& operator=(const OktetaProgram&) = delete;
    OktetaProgram& operator=(OktetaProgram&&) = delete;

public:
    [[nodiscard]]
    int execute();
    void quit();

public:
    [[nodiscard]]
    UserMessagesHandler* userMessagesHandler() const;
    [[nodiscard]]
    DocumentManager* documentManager() const;
    [[nodiscard]]
    ViewManager* viewManager() const;
    [[nodiscard]]
    MultiDocumentStrategy* documentStrategy() const;
    [[nodiscard]]
    ByteArrayViewProfileManager* byteArrayViewProfileManager() const;

private:
    QApplication mApp;

    // in (reverse) order of destruction
    std::unique_ptr<ByteArrayViewProfileManager> mByteArrayViewProfileManager;

    std::unique_ptr<DialogHandler> mDialogHandler;
    std::unique_ptr<UserMessagesHandler> m_userMessagesHandler;

    std::unique_ptr<DocumentManager> mDocumentManager;
    std::unique_ptr<ViewManager> mViewManager;
    std::unique_ptr<MultiDocumentStrategy> mDocumentStrategy;
};

inline UserMessagesHandler* OktetaProgram::userMessagesHandler() const { return m_userMessagesHandler.get(); }
inline DocumentManager* OktetaProgram::documentManager() const { return mDocumentManager.get(); }
inline ViewManager* OktetaProgram::viewManager() const { return mViewManager.get(); }
inline MultiDocumentStrategy* OktetaProgram::documentStrategy() const { return mDocumentStrategy.get(); }
inline ByteArrayViewProfileManager* OktetaProgram::byteArrayViewProfileManager() const { return mByteArrayViewProfileManager.get(); }

}

#endif
