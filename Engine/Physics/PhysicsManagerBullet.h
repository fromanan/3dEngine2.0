#pragma once

namespace PhysicsManagerBullet
{
	void Init();
	void Update(float deltaTime);
	void AddColliderShape(btCollisionShape* collider);
	void AddRigidBody(btRigidBody* body);
	btDiscreteDynamicsWorld* GetDynamicWorld();
}
