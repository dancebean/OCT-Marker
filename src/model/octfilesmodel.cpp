/*
 * Copyright (c) 2018 Kay Gawlik <kaydev@amarunet.de> <kay.gawlik@beuth-hochschule.de> <kay.gawlik@charite.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "octfilesmodel.h"

#include <manager/octdatamanager.h>

#include <QMessageBox>
#include <boost/exception/diagnostic_information.hpp>


OctFilesModel::OctFilesModel()
{
}


OctFilesModel::~OctFilesModel()
{
	for(const OctFileUnloaded* file : filelist)
		delete file;
}


QVariant OctFilesModel::data(const QModelIndex& index, int role) const
{
	if(!index.isValid())
		return QVariant();

	if(static_cast<std::size_t>(index.row()) >= filelist.size())
		return QVariant();

	if(role == Qt::DisplayRole)
		return filelist.at(static_cast<std::size_t>(index.row()))->getFilename();
	else
		return QVariant();
}


QVariant OctFilesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation == Qt::Horizontal)
		return QString("Column %1").arg(section);
	else
		return QString("Row %1").arg(section);
}

bool OctFilesModel::loadFile(QString filename)
{
	loadedFilePos = addFile(filename);
	return openFile(filename);
}



int OctFilesModel::addFile(QString filename)
{
	int count = 0;
	for(const OctFileUnloaded* file : filelist)
	{
		if(file->sameFile(filename))
		{
			return count;
		}
		++count;
	}

	beginInsertRows(QModelIndex(), loadedFilePos, loadedFilePos);
	filelist.push_back(new OctFileUnloaded(filename));
	endInsertRows();
	
	return count;
}

void OctFilesModel::loadNextFile()
{
	std::size_t filesInList = filelist.size();
	int requestFilePost = loadedFilePos + 1;
	if(requestFilePost >= 0 && static_cast<std::size_t>(requestFilePost) < filesInList)
	{
		openFile(filelist[requestFilePost]->getFilename());
		loadedFilePos = requestFilePost;
		fileIdLoaded(index(loadedFilePos));
	}
}


void OctFilesModel::loadPreviousFile()
{
	if(loadedFilePos > 0)
	{
		--loadedFilePos;
		openFile(filelist[loadedFilePos]->getFilename());
		fileIdLoaded(index(loadedFilePos));
	}
}





void OctFilesModel::slotClicked(QModelIndex index)
{
	if(!index.isValid())
		return;
	
	int row = index.row();
	if(row < 0 || static_cast<std::size_t>(row) >= filelist.size())
		return;
	

	loadedFilePos = row;
	OctFileUnloaded* file = filelist.at(static_cast<std::size_t>(row));
	openFile(file->getFilename());
}

void OctFilesModel::slotDoubleClicked(QModelIndex index)
{
	if(!index.isValid())
		return;
	
	std::size_t row = static_cast<std::size_t>(index.row());
	if(row >= filelist.size())
		return;
	
	OctFileUnloaded* file = filelist.at(row);
	
	
	qDebug("file doubleclicked: %s", file->getFilename().toStdString().c_str());
}


bool OctFilesModel::openFile(const QString& filename)
{
	try
	{
		OctDataManager::getInstance().openFile(filename);
		return true;
	}
	catch(boost::exception& e)
	{
		QMessageBox msgBox;
		msgBox.setText(QString::fromStdString(boost::diagnostic_information(e)));
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.exec();
	}
	catch(std::exception& e)
	{
		QMessageBox msgBox;
		msgBox.setText(QString::fromStdString(e.what()));
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.exec();
	}
	catch(const char* str)
	{
		QMessageBox msgBox;
		msgBox.setText(str);
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.exec();
	}
	catch(const QString& str)
	{
		QMessageBox msgBox;
		msgBox.setText(str);
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.exec();
	}
	catch(...)
	{
		QMessageBox msgBox;
		msgBox.setText(QString("Unknow error in file %1 line %2").arg(__FILE__).arg(__LINE__));
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.exec();
	}
	return false;
}


