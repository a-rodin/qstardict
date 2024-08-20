/*****************************************************************************
 * main.cpp - QStarDict, a StarDict clone written with using Qt              *
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

#include "trainer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QVector<WordForTraining> wordsList;
    wordsList.push_back(WordForTraining("hello", "привет"));
    wordsList.push_back(WordForTraining("verisimilitude", "правдоподобие, похожесть на правду"));
    wordsList.push_back(WordForTraining("hi", "привет"));

    QStarDict::Trainer trainer;
    trainer.setWords(wordsList);
    trainer.start();
    trainer.show();

//    QStarDict::ScatteredLettersStage scatteredLettersStage;
//    scatteredLettersStage.setWords(wordsList);
//    scatteredLettersStage.startStage();
//    scatteredLettersStage.show();
//
//    QStarDict::TypeInStage typeInStage;
//    typeInStage.setWords(wordsList);
//    typeInStage.startStage();
//    typeInStage.show();
    
    return app.exec();
}
