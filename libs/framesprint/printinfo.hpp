/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef FRAMESPRINT_PRINTINFO_HPP
#define FRAMESPRINT_PRINTINFO_HPP

// Qt
#include <QUrl>

class PrintInfo
{
public:
    PrintInfo() = default;
    PrintInfo(const PrintInfo&) = delete;

    ~PrintInfo() = default;

    PrintInfo& operator=(const PrintInfo&) = delete;

public:
    QUrl url() const;
    int noOfPages() const;

public:
    void setUrl(const QUrl& url);
    void setNoOfPages(int noOfPages);

private:
    QUrl mUrl;
    int mNoOfPages;
};

inline QUrl PrintInfo::url()      const { return mUrl; }
inline int PrintInfo::noOfPages() const { return mNoOfPages; }
inline void PrintInfo::setUrl(const QUrl& url) { mUrl = url; }
inline void PrintInfo::setNoOfPages(int noOfPages) { mNoOfPages = noOfPages; }

#endif
