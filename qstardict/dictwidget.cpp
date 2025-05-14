/*****************************************************************************
 * dictwidget.cpp - QStarDict, a dictionary application for learning         *
 *                  languages                                                *
 * Copyright (C) 2007-2025 Alexander Rodin                                   *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 2 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License along   *
 * with this program; if not, write to the Free Software Foundation, Inc.,   *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.               *
 *****************************************************************************/

#include "dictwidget.h"

#include <QScrollBar>
#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QTextDocumentFragment>
#include <QTextStream>
#include <QMessageBox>
#include <QMouseEvent>
#include <QTextDocument>
#include "application.h"
#include "dictbrowser.h"
#include "dictbrowsersearch.h"
#include "ipa.h"
#include "mainwindow.h"
#include "speaker.h"
#include "vocabulary.h"

namespace
{
class DictWidgetToolbar: public QToolBar
{
	public:
		DictWidgetToolbar(QWidget *parent = nullptr)
			: QToolBar(parent)
		{ }

	protected:
		virtual void mouseDoubleClickEvent(QMouseEvent *event)
		{
			if (! actionAt(event->pos()))
				QToolBar::mouseDoubleClickEvent(event);
		}
};
}

namespace QStarDict
{

DictWidget::DictWidget(QWidget *parent, Qt::WindowFlags f, bool openMainWindowAction)
	: QFrame(parent, f)
{
	m_translationView = new DictBrowser(this);
	setFrameStyle(m_translationView->frameStyle());
	m_translationView->setFrameStyle(QFrame::NoFrame);
	m_translationView->verticalScrollBar()->setCursor(Qt::ArrowCursor);
	m_translationView->horizontalScrollBar()->setCursor(Qt::ArrowCursor);
	m_translationView->setOpenExternalLinks(true);
	connect(m_translationView, &DictBrowser::sourceChanged,
            this, &DictWidget::on_translationView_sourceChanged);

	m_search = new DictBrowserSearch(this);
	connect(m_search, &DictBrowserSearch::search, m_translationView, &DictBrowser::search);
	connect(m_translationView, &DictBrowser::searchResult, m_search, &DictBrowserSearch::searchResult);
	m_search->hide();

    // Primary toolbar
	m_toolBar = new DictWidgetToolbar(this);
	m_toolBar->setMouseTracking(true);

	QAction *actionBackward = m_toolBar->addAction(QIcon(":/pics/go-previous.png"), tr("Go to &previous translation"),
			m_translationView, &DictBrowser::backward);
	actionBackward->setDisabled(true);
	connect(m_translationView, &DictBrowser::backwardAvailable, actionBackward, &QAction::setEnabled);

	QAction *actionForward = m_toolBar->addAction(QIcon(":/pics/go-next.png"), tr("Go to &next translation"),
			m_translationView, &DictBrowser::forward);
	actionForward->setDisabled(true);
	connect(m_translationView, &DictBrowser::forwardAvailable, actionForward, &QAction::setEnabled);

	m_toolBar->addAction(QIcon(":/pics/speaker.png"), tr("Pronounce the &word"),
			this, &DictWidget::speak);

	QAction *actionSearch = m_toolBar->addAction(QIcon(":/pics/system-search.png"), tr("Search"),
            this, &DictWidget::handleSearch);
	actionSearch->setCheckable(true);
	actionSearch->setShortcut(QKeySequence::Find);

    m_toolBar->addAction(QIcon(":/pics/word-add.png"), tr("&Add the word for studying"),
            this, &DictWidget::addWord);

    m_toolBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Right toolbar
    DictWidgetToolbar *rightToolBar = new DictWidgetToolbar(this);
    rightToolBar->setMouseTracking(true);
    if (openMainWindowAction)
    {
        rightToolBar->addAction(QIcon(":pics/view-fullscreen.png"),
            tr("Open in the main window"), this, &DictWidget::openMainWindow);
    }

    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_toolBar, 0, 0);
    layout->addWidget(rightToolBar, 0, 1);
    layout->addWidget(m_translationView, 1, 0, 1, 2);
    layout->addWidget(m_search, 2, 0, 1, 2);
    setLayout(layout);
}

void DictWidget::translate(const QString &str)
{
	m_translationView->setSource(QUrl("qstardict:" + str));
}

void DictWidget::on_translationView_sourceChanged(const QUrl &name)
{
	emit wordTranslated(name.toString(QUrl::RemoveScheme));
}

void DictWidget::speak()
{
	Application::instance()->speaker()->speak(translatedWord());
}

void DictWidget::handleSearch()
{
	m_search->setVisible(!m_search->isVisible());
}

void DictWidget::setDefaultStyleSheet(const QString &css)
{
	m_translationView->document()->setDefaultStyleSheet(css);
	m_translationView->reload();
}

void DictWidget::addWord()
{
    QTextCursor cursor = m_translationView->textCursor();
    QString translation;
    if (cursor.hasSelection())
        translation = cursor.selection().toPlainText().trimmed();
    else {
        m_translationView->showToast(
            tr("Please select a part of the article with the translation that " \
               "you want to add for studying and try again."));
        return;
    }

    auto html = m_translationView->loadedArticleHtml();

    static QRegularExpression titleRegExp("<font class=\"title\">([^<]*)</font>");
    auto word = titleRegExp.match(html).captured(1);
    static QRegularExpression transcriptionRegExp("<font class=\"transcription\">([^<]*)</font>");
    auto transcription = transcriptionRegExp.match(html).captured(1);

    Application::instance()->vocabulary()->addWord(word, translation, transcription);
    m_translationView->showToast(
            tr("The word \"%1\" is added for studying.").arg(word) + "<br>\n" +
            tr("Transcription: <b>%1</b>.").arg(transcription) + "<br>\n" +
            tr("Translation: %1.").arg(translation));
}

void DictWidget::openMainWindow()
{
    MainWindow *mainWindow = Application::instance()->mainWindow();
    mainWindow->showTranslation(translatedWord(), true);
    mainWindow->setVisible(false);
    mainWindow->setVisible(true);
}

}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

