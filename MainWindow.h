/*
Copyright (C) 2011 David Doria, daviddoria@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"

// Custom
#include "BlurComputationThread.h"

// Boost
#include "boost/multi_array.hpp"

// ITK
#include "itkCovariantVector.h"
#include "itkImage.h"

// Qt
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

class MainWindow : public QMainWindow, private Ui::MainWindow
{
Q_OBJECT
public:
  MainWindow(QWidget *parent = 0);

public slots:

  void slot_IterationComplete(const uint domainSigmaId, const uint rangeSigmaId, const QImage&);
  
  void on_actionOpenImageFile_activated();
  
  void on_btnStop_clicked();
  void on_btnGenerate_clicked();
  void on_btnSave_clicked();
  
  void StartProgressBar();
  void StopProgressBar();

protected:

  ImageType::Pointer Image;
  QGraphicsPixmapItem* ImageItem;
  QGraphicsScene* ImageScene;
  QGraphicsScene* BlurredGridScene;

  typedef boost::multi_array<QGraphicsPixmapItem*, 2> array_type;
  array_type BlurredGridItems;

  BlurComputationThreadClass ComputationThread;
};

#endif
