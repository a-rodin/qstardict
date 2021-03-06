**QStarDict**, which stands for [quasi-star](https://en.wikipedia.org/wiki/Quasi-star) dictionary, is a free (as [in freedom](https://www.gnu.org/philosophy/free-sw.html)) dictionary application. It is aimed primarily, but not exclusively, at those who learn foreign languages.

[![Build Status](https://travis-ci.org/a-rodin/qstardict.svg?branch=master)](https://travis-ci.org/a-rodin/qstardict)
## Features
### Fuzzy search, cross-links, and history navigation

QStarDict not only finds words as soon as they are typed, but also shows _fuzzy matches_ for them. It helps to identify similar words or correct typos. All articles are automatically _cross-linked_, so that click on a word in any article opens the corresponding article if it exists. With _history navigation_ buttons it is easy to move back and forth between articles.

<p align="center">
<img src="https://raw.githubusercontent.com/wiki/a-rodin/qstardict/images/qstardict-main.gif" width="742" height="533" alt="Main Window">
</p>

### Add words to Anki
QStarDict has integration with [Anki](https://apps.ankiweb.net), which allows to create cards from dictionary articles.

<p align="center">
<img src="https://raw.githubusercontent.com/wiki/a-rodin/qstardict/images/qstardict-anki.gif" width="803" height="533" alt="QStarDict and Anki">
</p>

### Look up words from any application

If QStarDict is running under X server, it is possible to look up works from other applications using a popup window which is shown for selected words in any application.

### Hide to notification area

To keep QStarDict always running with closed main window, it is possible to hide it to the notification area.

<p align="center">
<img alt="system tray" src="https://raw.githubusercontent.com/wiki/a-rodin/qstardict/images/qstardict-tray.png" width="143" height="31">
</p>

### Extend with plugins
If you want to add new dictionary format or custom toolbar action to QStarDict, it can be done without modifying the core by [writing a plugin](https://github.com/a-rodin/qstardict/wiki/How-to-write-a-QStarDict-plugin).

### Installation ###
QStarDict is available in Ubuntu, Debian and other distros repositories. 

If you want to compile it yourself install Qt5 and GLib2 development files and run in the project's directory

    qmake INSTALL_PREFIX=<your install prefix, /usr by default>
    make
    [sudo] make install
    
More information about installation:
* [GNU/Linux](https://github.com/a-rodin/qstardict/blob/master/INSTALL)
* [macOS](https://github.com/a-rodin/qstardict/blob/master/README.MACOS)
* [Windows](https://github.com/a-rodin/qstardict/blob/master/README.WINDOWS)
