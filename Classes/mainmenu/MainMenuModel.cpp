#include "MainMenuModel.h"

MainMenuModel::MainMenuModel(void)
{
	this->_modelDelegate = NULL;
}

MainMenuModel::~MainMenuModel(void)
{

}

void MainMenuModel::setDelegate(MainMenuModelDelegate *pDelegate)
{
	this->_modelDelegate = pDelegate;
}