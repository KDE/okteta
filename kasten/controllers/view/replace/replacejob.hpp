/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KASTEN_REPLACEJOB_HPP
#define KASTEN_REPLACEJOB_HPP

// controller
#include "replaceuserqueryable.hpp"
// libfinddialog
#include <finddirection.hpp>
// Okteta core
#include <Okteta/Address>
// Qt
#include <QObject>
#include <QByteArray>

namespace Okteta {
class AbstractByteArrayModel;
}

namespace Kasten {

class ByteArrayView;

class ReplaceJob : public QObject
{
    Q_OBJECT

public:
    /// @param userQueryAgent expected to implement If::ReplaceUserQueryable
    explicit ReplaceJob(ByteArrayView* byteArrayView, Okteta::AbstractByteArrayModel* byteArrayModel,
                        QObject* userQueryAgent,
                        QObject* parent = nullptr);
    ~ReplaceJob() override;

public:
    void setSearchData(const QByteArray& searchData);
    void setReplaceData(const QByteArray& replaceData);
    void setCaseSensitivity(Qt::CaseSensitivity caseSensitivity);
    void setDoPrompt(bool doPrompt);
    void setRange(Okteta::Address replaceRangeStartIndex, Okteta::Address replaceRangeEndIndex,
                  FindDirection direction);

public:
    void start();

Q_SIGNALS:
    void finished(bool previousFound, int noOfReplacements);

private Q_SLOTS:
    void handleSearchResult(Okteta::Address pos);
    void handleReplaceCurrentFinished(Kasten::ReplaceBehaviour replaceBehaviour);
    void handleContinueFinished(bool result);

private:
    void searchNextPosition();
    void wrapAndSearchNextPosition();
    void replaceCurrent();
    void handleEndReached();
    void finish();

private: // settings
    QByteArray m_searchData;
    QByteArray m_replaceData;
    Okteta::Address m_replaceRangeStartIndex;
    Okteta::Address m_replaceRangeEndIndex;
    Qt::CaseSensitivity m_caseSensitivity = Qt::CaseSensitive;
    bool m_doPrompt = false;

private: // status
    Okteta::Address m_currentIndex;
    Okteta::Address m_currentReplaceRangeStartIndex;
    Okteta::Address m_currentReplaceRangeEndIndex;
    int m_noOfReplacements = 0;
    FindDirection m_direction;
    bool m_previousFound : 1;
    bool m_doWrap : 1;

private:
    // expected to implement If::ReplaceUserQueryable
    QObject* m_userQueryAgent = nullptr;

private: // target
    ByteArrayView* m_byteArrayView = nullptr;
    Okteta::AbstractByteArrayModel* m_byteArrayModel = nullptr;
};

}

#endif

