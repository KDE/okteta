/***************************************************************************
                          byteseditapp.h  -  description
                             -------------------
    begin                : Tue Jul 29 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef _BYTESEDITAPP_H_
#define _BYTESEDITAPP_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// qt specific
#include <qwidget.h>
// kde specific
#include <kparts/part.h>
#include <kapplication.h>
#include <kmainwindow.h>

class QPainter;
namespace KHE {
class BytesEditInterface;
//class HexColumnInterface;
//class TextColumnInterface;
class ClipboardInterface;
class ZoomInterface;
};

class KToggleAction;
class KRadioAction;

/**
 * This class serves as the main window for BytesEditApp.  It handles the
 * menus, toolbars, and status bars.
 *
 * @short Main window class
 * @author $AUTHOR <$EMAIL>
 * @version $APP_VERSION
 */
class BytesEditApp : public KMainWindow
{
    Q_OBJECT
  public:
    BytesEditApp();
    virtual ~BytesEditApp();

private slots:
    void setReadOnly();

    void slotSetCoding();
    void slotSetShowUnprintable();
    void slotSetResizeStyle();

    void slotResetData();
    void slotResetData2();
    void slotChangeData();

    void changeStatusbar(const QString& text);
    void changeCaption(const QString& text);

  private:
    void setupActions();
    void setStartData();

  private:
    char* Buffer;
    QWidget* BytesEditWidget;
    KHE::BytesEditInterface *BytesEdit;

  private:
    KHE::ClipboardInterface *Clipboard;
    KHE::ZoomInterface *Zoom;
    // edit menu
    KAction *CutAction;
    KAction *CopyAction;
    KToggleAction *ReadOnlyAction;
    // view menu
    KRadioAction *HexCodingAction;
    KRadioAction *DecCodingAction;
    KRadioAction *OctCodingAction;
    KRadioAction *BinCodingAction;
    KToggleAction *ShowUnprintableAction;
    // settings menu
    KRadioAction *NoResizeAction;
    KRadioAction *LockGroupsAction;
    KRadioAction *FullSizeUsageAction;

};

#endif // _BYTESEDITAPP_H_
