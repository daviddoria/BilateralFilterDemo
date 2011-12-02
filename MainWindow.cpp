/*
Copyright (C) 2010 David Doria, daviddoria@gmail.com

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

#include "MainWindow.h"

// Custom
#include "Helpers.h"

// STL
#include <iostream>

// Qt
#include <QDoubleValidator>
#include <QFileDialog>
#include <QImage>

// ITK
#include "itkBilateralImageFilter.h"
#include "itkImageFileReader.h"

MainWindow::MainWindow(QWidget *parent)
{
  // Setup the GUI and connect all of the signals and slots
  setupUi(this);

  QDoubleValidator* validator = new QDoubleValidator(0.0d, 100.0d, 2, this);
  this->txtRangeSigmaMin->setValidator(validator);
  this->txtRangeSigmaMax->setValidator(validator);
  this->txtDomainSigmaMin->setValidator(validator);
  this->txtDomainSigmaMax->setValidator(validator);

  this->ImageScene = new QGraphicsScene;
  this->graphicsViewImage->setScene(this->ImageScene);

  this->BlurredGridScene = new QGraphicsScene;
  this->graphicsViewBlurredGrid->setScene(this->BlurredGridScene);

  connect(&this->ComputationThread, SIGNAL(IterationCompleteSignal(const uint,const uint,const QImage&)),
          this, SLOT(slot_IterationComplete(const uint,const uint,const QImage&)));
  
  connect(&this->ComputationThread, SIGNAL(StartProgressBarSignal()), this, SLOT(StartProgressBar()));
  connect(&this->ComputationThread, SIGNAL(StopProgressBarSignal()), this, SLOT(StopProgressBar()));

  this->progressBar->setMinimum(0);
  this->progressBar->setMaximum(0);
  this->progressBar->hide();
}

void MainWindow::on_actionOpenImageFile_activated()
{
  QString fileName = QFileDialog::getOpenFileName(this,
                    "OpenFile", ".", "All Files (*.*)");

  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(fileName.toStdString());
  reader->Update();

  this->Image = reader->GetOutput();

  QImage qimage = Helpers::GetQImageRGB<ImageType>(this->Image);
  
  this->ImageItem = this->ImageScene->addPixmap(QPixmap::fromImage(qimage));
  this->graphicsViewImage->fitInView(this->ImageItem, Qt::KeepAspectRatio);
}

void MainWindow::on_btnStop_clicked()
{
  
}

void MainWindow::on_btnGenerate_clicked()
{

  unsigned int dimRangeSigma = this->spinRangeSigma->value();
  unsigned int dimDomainSigma = this->spinDomainSigma->value();
  BlurredGridItems.resize(boost::extents[dimRangeSigma][dimDomainSigma]);

  std::vector<float> domainSigmas;
  float domainSigmaStep = (txtDomainSigmaMax->text().toFloat() - txtDomainSigmaMin->text().toFloat())/static_cast<float>(dimDomainSigma - 1);
  for(unsigned int i = 0; i < dimDomainSigma; ++i)
    {
    domainSigmas.push_back(txtDomainSigmaMin->text().toFloat() + i * domainSigmaStep);
    }

  std::vector<float> rangeSigmas;
  float rangeSigmaStep = (txtRangeSigmaMax->text().toFloat() - txtRangeSigmaMin->text().toFloat())/static_cast<float>(dimRangeSigma - 1);
  for(unsigned int i = 0; i < dimRangeSigma; ++i)
    {
    rangeSigmas.push_back(txtRangeSigmaMin->text().toFloat() + i * rangeSigmaStep);
    }

  ComputationThread.Image = this->Image;
  ComputationThread.RangeSigmas = rangeSigmas;
  ComputationThread.DomainSigmas = domainSigmas;
  ComputationThread.start();
  
}

void MainWindow::on_btnSave_clicked()
{
  QString fileName = QFileDialog::getSaveFileName(this, "Save Scene", ".", "Image (*.*)");

  QPixmap sceneImage(this->BlurredGridScene->sceneRect().width(), this->BlurredGridScene->sceneRect().height());
  QColor white(255,255,255);
  sceneImage.fill(white);

  QPainter painter(&sceneImage);
  painter.setRenderHint(QPainter::Antialiasing);
  this->BlurredGridScene->render(&painter);
  sceneImage.save(fileName);
}

void MainWindow::slot_IterationComplete(const unsigned int domainSigmaId, const unsigned int rangeSigmaId, const QImage& qimage)
{
  // Setup label
  std::stringstream ss;
  ss << "Domain sigma: " << ComputationThread.DomainSigmas[domainSigmaId] << " Range sigma: " << ComputationThread.RangeSigmas[rangeSigmaId];

  // Create label
  QGraphicsSimpleTextItem* simpleTextItem = this->BlurredGridScene->addSimpleText(ss.str().c_str());
  unsigned int labelHeight = simpleTextItem->boundingRect().height();
    
  unsigned int width = this->Image->GetLargestPossibleRegion().GetSize()[0];
  unsigned int height = this->Image->GetLargestPossibleRegion().GetSize()[1];

  this->BlurredGridItems[domainSigmaId][rangeSigmaId] = this->BlurredGridScene->addPixmap(QPixmap::fromImage(qimage));
  this->BlurredGridItems[domainSigmaId][rangeSigmaId]->setPos(domainSigmaId * (width + 3), rangeSigmaId * (height + labelHeight + 6));

  simpleTextItem->setPos(domainSigmaId * (width + 3),
                         this->BlurredGridItems[domainSigmaId][rangeSigmaId]->pos().y() + this->BlurredGridItems[domainSigmaId][rangeSigmaId]->boundingRect().height());
    
  this->graphicsViewBlurredGrid->fitInView(this->BlurredGridItems[domainSigmaId][rangeSigmaId], Qt::KeepAspectRatio);
  std::cout << "slot_IterationComplete finshed." << std::endl;

}


void MainWindow::StartProgressBar()
{
  this->progressBar->show();
}

void MainWindow::StopProgressBar()
{
  this->progressBar->hide();
}
