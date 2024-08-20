/*****************************************************************************
 * scatteredletters.cpp - QStarDict, a free dictionary application           *
 * Copyright (C) 2024 Alexander Rodin                                        *
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

#include "scatteredletters.h"

#include <algorithm>
#include <random>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QPushButton>

namespace QStarDict
{

ScatteredLetters::ScatteredLetters(QWidget *parent)
    : QWidget(parent)
{
    m_topLayout = 0;
    m_bottomLayout = 0;
    m_layout = 0;
}

ScatteredLetters::~ScatteredLetters()
{
    deleteLayoutsAndButtons();
}

void ScatteredLetters::deleteLayoutsAndButtons()
{
    delete m_topLayout;
    delete m_bottomLayout;
    delete m_layout;
    for (QVector<QPushButton*>::const_iterator i = m_letterButtons.begin(); i != m_letterButtons.end(); i++)
        delete *i;
    m_letterButtons.clear();
    for (QVector<QPushButton*>::const_iterator i = m_typedInLetterButtons.begin();
            i != m_typedInLetterButtons.end(); i++)
        delete *i;
    m_typedInLetterButtons.clear();
}

void ScatteredLetters::setWord(const QString &word)
{
    deleteLayoutsAndButtons();

    m_word = word;
    m_errorsCount = 0;
    m_completed = false;

    m_topLayout = new QHBoxLayout();
    for (int i = 0; i < word.size(); i++)
    {
        QPushButton *letterButton = new QPushButton(word[i], this);
        letterButton->setMaximumWidth(letterButton->height());
        m_topLayout->addWidget(letterButton);

        QSizePolicy sizePolicy = letterButton->sizePolicy();
        sizePolicy.setRetainSizeWhenHidden(true);
        letterButton->setSizePolicy(sizePolicy);

        letterButton->setVisible(false);

        m_typedInLetterButtons.push_back(letterButton);
    }

    QVector<QChar> wordLetters(word.size());
    std::copy(word.begin(), word.end(), wordLetters.begin());

    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::shuffle(wordLetters.begin(), wordLetters.end(), generator);

    m_bottomLayout = new QHBoxLayout();
    for (int i = 0; i < wordLetters.size(); i++)
    {
        QPushButton *letterButton = new QPushButton(wordLetters[i], this);
        letterButton->setMaximumWidth(letterButton->height());
        m_bottomLayout->addWidget(letterButton);

        QSizePolicy sizePolicy = letterButton->sizePolicy();
        sizePolicy.setRetainSizeWhenHidden(true);
        letterButton->setSizePolicy(sizePolicy);

        connect(letterButton, &QPushButton::pressed, [=]() {
            QChar letter = letterButton->text().front();
            for (QVector<QPushButton*>::const_iterator i = m_typedInLetterButtons.begin();
                    i != m_typedInLetterButtons.end(); i++)
            {
                if (! (*i)->isVisible())
                {
                    if ((*i)->text().front() == letter)
                    {
                        (*i)->setVisible(true);
                        letterButton->setVisible(false);
                        if (i + 1 == m_typedInLetterButtons.end())
                        {
                            m_completed = true;
                            emit completed(m_errorsCount > 1);
                        }
                    }
                    else
                    {
                        m_errorsCount++;
                        emit error(m_errorsCount);
                    }
                    break;
                }
            }
        });

        m_letterButtons.push_back(letterButton);
    }

    m_layout = new QVBoxLayout(this);
    m_layout->addLayout(m_topLayout);
    m_layout->addLayout(m_bottomLayout);
    setLayout(m_layout);
}

void ScatteredLetters::keyReleaseEvent(QKeyEvent *event)
{
    for (QVector<QPushButton*>::const_iterator i = m_letterButtons.begin(); i != m_letterButtons.end(); i++)
    {
        if ((*i)->isVisible() && (*i)->text() == event->text())
        {
            (*i)->animateClick();
            break;
        }
    }
}

void ScatteredLetters::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return)
    {
        if (! m_completed)
        {
            surrender();
        }
        else
        {
            emit enterPressed();
        }
    }
}

void ScatteredLetters::surrender()
{
    if (m_completed)
        return;

    m_errorsCount += 2;
    for (QVector<QPushButton*>::const_iterator i = m_letterButtons.begin(); i != m_letterButtons.end(); i++)
    {
        (*i)->setVisible(false);
    }
    for (QVector<QPushButton*>::const_iterator i = m_typedInLetterButtons.begin();
            i != m_typedInLetterButtons.end(); i++)
    {
        (*i)->setVisible(true);
    }
    emit error(m_errorsCount);
    m_completed = true;
    emit completed(m_errorsCount);
}

}
