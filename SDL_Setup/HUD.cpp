#include "HUD.h"
using namespace std;

HUD::HUD()
{
	this->advanced = false;
}

void HUD::toggleAdvanced()
{
	if(this->advanced)
		this->advanced = false;
	else
		this->advanced = true;
}