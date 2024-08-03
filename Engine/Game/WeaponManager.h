#pragma once
#include "Gun.h"

namespace WeaponManager
{
    void Init();
    Gun* GetGunByName(const std::string& name);
}
