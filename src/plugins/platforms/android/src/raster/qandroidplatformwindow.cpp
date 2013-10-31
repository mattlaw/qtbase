/****************************************************************************
**
** Copyright (C) 2012 BogDan Vatra <bogdan@kde.org>
** Contact: http://www.qt-project.org/legal
**
** This file is part of the plugins of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qandroidplatformwindow.h"

#include "androidjnimain.h"
#include <qpa/qwindowsysteminterface.h>

QAndroidPlatformWindow::QAndroidPlatformWindow(QWindow *window)
    : QFbWindow(window)
    , m_state(Qt::WindowNoState)
{
}

void QAndroidPlatformWindow::setGeometry(const QRect &rect)
{
    QFbWindow::setGeometry(rect);
}

void QAndroidPlatformWindow::propagateSizeHints()
{
    //shut up warning from default implementation
}

void QAndroidPlatformWindow::updateStatusBarVisibility()
{
    Qt::WindowFlags flags = window()->flags();
    bool isNonRegularWindow = flags & (Qt::Popup | Qt::Dialog | Qt::Sheet) & ~Qt::Window;
    if (!isNonRegularWindow) {
        if (m_state & Qt::WindowFullScreen)
            QtAndroid::hideStatusBar();
        else if (m_state & Qt::WindowMaximized)
            QtAndroid::showStatusBar();
    }
}

void QAndroidPlatformWindow::raise()
{
    updateStatusBarVisibility();
    QFbWindow::raise();
}

void QAndroidPlatformWindow::setWindowState(Qt::WindowState state)
{
    if (m_state == state)
        return;

    m_state = state;
    if (window()->isVisible())
        updateStatusBarVisibility();

    QFbWindow::setWindowState(state);
}

void QAndroidPlatformWindow::setVisible(bool visible)
{
    if (visible)
        updateStatusBarVisibility();

    QFbWindow::setVisible(visible);

    // The Android Activity is activated before Qt is initialized, causing the application state to
    // never be set to 'active'. We explicitly set this state when the first window becomes visible.
    if (visible)
        QtAndroid::setApplicationActive();
}
