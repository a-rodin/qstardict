/*****************************************************************************
 * dictwidget.cpp - QStarDict, a StarDict clone written with using Qt        *
 * Copyright (C) 2007 Alexander Rodin                                        *
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
#include "speaker.h"

namespace
{
class DictWidgetToolbar: public QToolBar
{
	public:
		DictWidgetToolbar(QWidget *parent = 0)
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

DictWidget::DictWidget(QWidget *parent, Qt::WindowFlags f)
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

	m_toolBar->addAction(QIcon(":/pics/speaker.png"), tr("Speak &word"),
			this, &DictWidget::speak);

	QAction *actionSearch = m_toolBar->addAction(QIcon(":/pics/system-search.png"), tr("Search"),
            this, &DictWidget::handleSearch);
	actionSearch->setCheckable(true);
	actionSearch->setShortcut(QKeySequence::Find);

    QAction *actionAddWord = m_toolBar->addAction(QIcon(":/pics/word-add.png"), tr("&Add word for studying"),
            this, &DictWidget::addWord);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	layout->addWidget(m_toolBar);
	layout->addWidget(m_translationView);
	layout->addWidget(m_search);
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
    auto translatedWord = m_translationView->source().toString(QUrl::RemoveScheme);
    auto cursor = m_translationView->textCursor();
    QString translation;
    if (cursor.hasSelection()) {
        translation = cursor.selection().toPlainText();
    } else {
        QMessageBox::warning(
            this,
            tr("Adding a word for studying"),
            tr("Please select a part of the article with the translation that " \
               "you want to add for studying and try again."));
        return;
    }

    qDebug("word: %s", translatedWord.toUtf8().data());
    qDebug("translation: %s", translation.toUtf8().data());
}

}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

