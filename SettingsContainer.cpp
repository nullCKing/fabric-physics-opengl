#include "SettingsContainer.h"
SettingsContainer::SettingsContainer()
{
	settingsToggler = false;
}

SettingsContainer::~SettingsContainer()
{

}

void SettingsContainer::toggleSettings(bool _toggle)
{
	if (_toggle)
		settingsToggler = true;
	else
		settingsToggler = false;
}