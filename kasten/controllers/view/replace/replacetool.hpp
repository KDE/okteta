/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_REPLACETOOL_HPP
#define KASTEN_REPLACETOOL_HPP

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

class ByteArrayView;
class ReplaceJob;

// TODO: is queryAgent needed, or should the tool better be a state machine? same with search tool
class ReplaceTool : public AbstractTool
{
    Q_OBJECT

public:
    ReplaceTool();
    ~ReplaceTool() override;

public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    QString title() const override;

    void setTargetModel(AbstractModel* model) override;

public: // status
    QByteArray searchData() const;
    QByteArray replaceData() const;
//     Qt::CaseSensitivity caseSensitivity() const;
    bool hasSelectedData() const;
    QString charCodingName() const;

    bool isApplyable() const; // candidate for AbstractTool API

public: // actions
    void replace(FindDirection direction, bool fromCursor, bool inSelection);

public:
    /// @param userQueryAgent expected to implement If::ReplaceUserQueryable
    void setUserQueryAgent(QObject* userQueryAgent);

public Q_SLOTS: // settings
    void setSearchData(const QByteArray& searchData);
    void setReplaceData(const QByteArray& replaceData);
    void setCaseSensitivity(Qt::CaseSensitivity caseSensitivity);
    void setDoPrompt(bool doPrompt);

Q_SIGNALS:
    void isApplyableChanged(bool isApplyable);    // candidate for AbstractTool API
    void finished(bool previousFound, int noOfReplacements);

private:
    void doReplace(FindDirection direction, Okteta::Address startIndex);

private Q_SLOTS:
    void onReadOnlyChanged(bool isReadOnly);
    void onJobFinished(bool previousFound, int noOfReplacements);

private: // settings
    QByteArray mSearchData;
    QByteArray mReplaceData;
    Qt::CaseSensitivity mCaseSensitivity = Qt::CaseSensitive;
    bool mDoPrompt : 1;

private: // status
    ReplaceJob* mReplaceJob = nullptr;

private:
    // expected to implement If::ReplaceUserQueryable
    QObject* mUserQueryAgent = nullptr;

private: // target
    ByteArrayView* mByteArrayView = nullptr;
    Okteta::AbstractByteArrayModel* mByteArrayModel = nullptr;
};

inline QByteArray ReplaceTool::searchData() const { return mSearchData; }
inline QByteArray ReplaceTool::replaceData() const { return mReplaceData; }
// inline Qt::CaseSensitivity ReplaceTool::caseSensitivity() const { return mCaseSensitivity; }

}

#endif
