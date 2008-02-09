/***************************************************************************
                          versiontablemodel.h  -  description
                             -------------------
    begin                : Sat Feb 9 2008
    copyright            : 2008 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef VERSIONTABLEMODEL_H
#define VERSIONTABLEMODEL_H


// Qt
#include <QtCore/QAbstractTableModel>

class KDocumentVersionData;
namespace KDE { namespace If {
class Versionable;
}}
class KAbstractDocument;


class VersionTableModel : public QAbstractTableModel
{
  Q_OBJECT

  public:
    enum ColumnIds
    {
        CurrentColumnId = 0,
        IdColumnId = 1,
        ChangeDescriptionColumnId = 2,
        NoOfColumnIds = 3 // TODO: what pattern is usually used to mark number of ids?
    };

  public:
    VersionTableModel( KAbstractDocument *document, KDE::If::Versionable *versionControl, QObject *parent = 0 );
    virtual ~VersionTableModel();

  public: // QAbstractTableModel API
    virtual int rowCount( const QModelIndex &parent ) const;
    virtual int columnCount( const QModelIndex &parent ) const;
    virtual QVariant data( const QModelIndex &index, int role ) const;
    virtual QVariant headerData( int section, Qt::Orientation orientation, int role ) const;

  public:
    void setDocument( KAbstractDocument *document, KDE::If::Versionable *versionControl );

  protected Q_SLOTS:
    void onRevertedToVersionIndex( int versionIndex );
    void onHeadVersionChanged( int newHeadVersionIndex );
    void onHeadVersionDataChanged( const KDocumentVersionData &newVersionData );

  protected:
    KAbstractDocument *mDocument;
    KDE::If::Versionable *mVersionControl;
    /// holds the current version index
    int mVersionIndex;
};

#endif
