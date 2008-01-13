/***************************************************************************
                          filterview.h  -  description
                             -------------------
    begin                : Fri Jan 11 2008
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


#ifndef FILTERVIEW_H
#define FILTERVIEW_H


// Qt
#include <QtGui/QWidget>

class FilterTool;
class AbstractByteArrayFilterParameterSet;
class KPushButton;
class KComboBox;
class QStackedWidget;
class QString;

class FilterView : public QWidget
{
  Q_OBJECT

  public:
    explicit FilterView( FilterTool *tool, QWidget *parent = 0 );
    virtual ~FilterView();

  public: // set
    void setCharCode( const QString &codeName );
    void addParameterEdit( const QString &name, QWidget *parameterEdit );

  public: // get 
    int filterId() const;
    void getParameterSet( AbstractByteArrayFilterParameterSet *parameterSet ) const;

  private Q_SLOTS:
    void onFilterClicked();
    void onOperationChange( int index );
    void onViewChanged( bool hasView );
    void onValidityChanged( bool isValid );

  private:
    FilterTool *mTool;
    KComboBox *mOperationComboBox;
    QStackedWidget *mParameterSetEditStack;
    KPushButton *mFilterButton;
};

#endif
