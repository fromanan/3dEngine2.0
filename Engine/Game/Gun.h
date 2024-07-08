#pragma once

enum GunType {
	Semi,
	Automatic,
	ShotGun
};

struct Gun {
	int ammo;
	int firerate;
	int currentammo;
	int reloadtime;
	int damage;
	GunType type;
};
