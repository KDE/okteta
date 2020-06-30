/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SEARCHTOOL_HPP
#define KASTEN_SEARCHTOOL_HPP

// libfinddialog
#include <finddirection.hpp>
// Kasten core
#include <Kasten/AbstractTool>
// Okteta core
#include <Okteta/Address>

namespace Okteta {
class AbstractByteArrayModel;
}

namespace Kasten {

namespace If {
class SearchUserQueryable;
}
class ByteArrayView;

class SearchTool : public AbstractTool
{
    Q_OBJECT

public:
    SearchTool();
    ~SearchTool() override;

public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    QString title() const override;

    void setTargetModel(AbstractModel* model) override;

public: // status
    QByteArray searchData() const;
//     Qt::CaseSensitivity caseSensitivity() const;
    bool hasSelectedData() const;
    QString charCodingName() const;

    bool isApplyable() const; // candidate for AbstractTool API

public: // actions
    void search(FindDirection direction, bool fromCursor, bool inSelection);

public:
    void setUserQueryAgent(If::SearchUserQueryable* userQueryAgent);

public Q_SLOTS: // settings
    void setSearchData(const QByteArray& searchData);
    void setCaseSensitivity(Qt::CaseSensitivity caseSensitivity);

Q_SIGNALS:
    void isApplyableChanged(bool isApplyable);    // candidate for AbstractTool API
    void charCodecChanged(const QString& codecName);

    void dataNotFound();

private:
    void doSearch(FindDirection direction);

private: // settings
    QByteArray mSearchData;
    Qt::CaseSensitivity mCaseSensitivity{Qt::CaseSensitive};

private: // status
    bool mPreviousFound : 1;
    Okteta::Address mSearchFirstIndex;
    Okteta::Address mSearchLastIndex;

private:
    If::SearchUserQueryable* mUserQueryAgent = nullptr;

private: // target
    ByteArrayView* mByteArrayView = nullptr;
    Okteta::AbstractByteArrayModel* mByteArrayModel = nullptr;
};

inline QByteArray SearchTool::searchData() const { return mSearchData; }
// inline Qt::CaseSensitivity SearchTool::caseSensitivity()       const { return mCaseSensitivity; }

}

#endif
