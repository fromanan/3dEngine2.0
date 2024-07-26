#include "Player.h"
#include "Engine/Core/AssetManager.h"
#include "Engine/Core/Scene/SceneManager.h"
#include "Engine/Physics/BulletPhysics.h"

namespace Player
{
	glm::vec3 forward;
	float horizontalAngle = 3.14f;
	float verticalAngle = 0.0f;
	float initialFoV = 45.0f;
	float maxAngle = 1.5;
	float mouseSpeed = 0.005f;
	float speed = 2000;
	float jumpforce = 9;
	std::string gunName = "ak47";
	std::string interactingWithName = "nothing";
	float interactDistance = 3;

	//states
	bool reloading = false;
	bool aiming = false;

	double reloadingTime = 0;
	double footstepTime = 0;
	double footstep_interval = 0.5;

	std::string inv[2] = { "ak47","glock" };

	void Player::Init() {
		srand(time(0));
		AssetManager::AddGameObject(GameObject("player", "Assets/Objects/capsule.obj", AssetManager::GetTexture("uvmap"), glm::vec3(0, 10, 5), false, 1, Capsule, 0.5, 1, 0.5));
		AssetManager::AddGameObject(GameObject("player_head", "Assets/Objects/capsule.obj", AssetManager::GetTexture("uvmap"), glm::vec3(0, 10, 5), false, 0, Sphere, 0.5, 0.7, 0.5));
		GameObject* player_head = AssetManager::GetGameObject("player_head");
		player_head->SetRender(false);
		btBroadphaseProxy*  proxy = player_head->GetRigidBody()->getBroadphaseHandle();
		if (proxy) {
			proxy->m_collisionFilterGroup = GROUP_PLAYER;
			proxy->m_collisionFilterMask = GROUP_STATIC | GROUP_DYNAMIC;
			// Add the constraint to the world
		}

		btRigidBody* body = AssetManager::GetGameObject("player")->GetRigidBody();
		AssetManager::GetGameObject("player")->SetRender(false);
		body->setFriction(0.0f);
		body->setRestitution(0.0f);
		body->setCcdMotionThreshold(0.05);
		body->setCcdSweptSphereRadius(0.2); // Set the radius for CCD

		body->setGravity(btVector3(0, -10 * 3.0f, 0));
		player_head->GetRigidBody()->setFriction(0.0f);
		player_head->GetRigidBody()->setRestitution(0.0f);
		player_head->GetRigidBody()->setActivationState(false);


		proxy = body->getBroadphaseHandle();
		if (proxy) {
			proxy->m_collisionFilterGroup = GROUP_PLAYER;
			proxy->m_collisionFilterMask = GROUP_STATIC | GROUP_DYNAMIC;
			// Add the constraint to the world
			
		}
		//std::cout << "loading player model" << std::endl;
		//gunName = "ak47";
	}
	void Player::Shoot() {
		if (gunName == "nothing")
			return;

		if (WeaponManager::GetGunByName(gunName)->currentammo > 0)
		{
			WeaponManager::GetGunByName(gunName)->currentammo--;
			WeaponManager::GetGunByName(gunName)->Shoot();
			verticalAngle += WeaponManager::GetGunByName(gunName)->recoil;
			horizontalAngle += (((double)rand()) / RAND_MAX) / WeaponManager::GetGunByName(gunName)->recoilY;
			btCollisionWorld::ClosestRayResultCallback hit = Camera::GetRayHit();
			if (hit.m_collisionObject != nullptr)
			{
				GameObject* gameobject = AssetManager::GetGameObject(hit.m_collisionObject->getUserIndex());
				if (gameobject != NULL)
				{
					btRigidBody* body = gameobject->GetRigidBody();
					body->applyImpulse(2 * glmToBtVector3(Camera::ComputeRay()),  body->getWorldTransform().inverse() * hit.m_hitPointWorld );
					if(body->getBroadphaseHandle()->m_collisionFilterGroup == GROUP_STATIC)
						AssetManager::AddDecal(glm::vec3(hit.m_hitPointWorld.getX(), hit.m_hitPointWorld.getY(), hit.m_hitPointWorld.getZ()), glm::vec3(hit.m_hitNormalWorld.getX(), hit.m_hitNormalWorld.getY(), hit.m_hitNormalWorld.getZ()), glm::vec3(0.025, 0.025, 0.025), AssetManager::GetTexture("bullet_hole"));
				}
			}

		}
		else {
			//click click
			AudioManager::PlaySound("dry_fire", AssetManager::GetGameObject("player")->getPosition());
		}
		WeaponManager::GetGunByName(gunName)->lastTimeShot = glfwGetTime();

	}
	
	bool Player::OnGround() {
		GameObject* player = AssetManager::GetGameObject("player");
		glm::vec3 out_end = player->getPosition() + glm::vec3(0,-0.5,0);

		btCollisionWorld::ClosestRayResultCallback RayCallback(
			btVector3(player->getPosition().x, player->getPosition().y, player->getPosition().z),
			btVector3(out_end.x, out_end.y, out_end.z)
		);
		PhysicsManagerBullet::GetDynamicWorld()->rayTest(
			btVector3(player->getPosition().x, player->getPosition().y, player->getPosition().z),
			btVector3(out_end.x, out_end.y, out_end.z),
			RayCallback
		);
		if(RayCallback.hasHit())
			return true;
		return false;
	}

	void Player::Update(float deltaTime) {
		GameObject* player =  AssetManager::GetGameObject("player");
		bool IsGrounded = OnGround();
		if (IsGrounded){
			player->GetRigidBody()->setAngularVelocity(btVector3(0, 0, 0));
		}
			
		btQuaternion quat;
		quat.setEuler(0, player->getRotation().y, 0);
		player->GetRigidBody()->getWorldTransform().setRotation(quat);

		interactingWithName = "Nothing";

		if (verticalAngle <= maxAngle && verticalAngle >= -maxAngle)
			verticalAngle += mouseSpeed * float(768 / 2 - Input::GetMouseY());
		
		else if (verticalAngle > maxAngle)
			verticalAngle = maxAngle;
		
		else if (verticalAngle < -maxAngle)
			verticalAngle = -maxAngle;
		
		forward = glm::vec3(
			sin(horizontalAngle + 3.14f),
			0,
			cos(horizontalAngle + 3.14f)
		);
		// Right vector
		glm::vec3 right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);
		// Move forward
		btVector3 movement = btVector3(0,0, 0);
		if (IsGrounded)
		{
			if (Input::KeyDown('w')) {
				movement += glmToBtVector3(-forward);
			}
			// Move backward
			if (Input::KeyDown('s')) {
				movement += glmToBtVector3(forward);
			}
			// Strafe right
			if (Input::KeyDown('d')) {
				movement += glmToBtVector3(right);
			}
			// Strafe left
			if (Input::KeyDown('a')) {
				movement += glmToBtVector3(-right);
			}
			if (Input::KeyDown(' ')) {
				movement.setY(1 * jumpforce);
			}
			//deltatime was making it jittery will fix later
			movement.setX(movement.x() * speed * 0.003);
			movement.setZ(movement.z() * speed * 0.003);

			player->GetRigidBody()->setLinearVelocity(movement);
			//player->GetRigidBody()->applyCentralForce(movement * 50);
		}
		
		if (Input::KeyPressed('e')) {
			btCollisionWorld::ClosestRayResultCallback hit = Camera::GetRayHit();
			if (hit.m_collisionObject != nullptr)
			{
				GameObject* gameobject = AssetManager::GetGameObject(hit.m_collisionObject->getUserIndex());
				if (gameobject != NULL && glm::distance(gameobject->getPosition(), getPosition()) <= interactDistance)
				{
					interactingWithName = gameobject->GetName();
				}
			}
		}
		if (Input::KeyPressed('r') && !reloading && !aiming) {
			reloading = true;
			reloadingTime = glfwGetTime();
		}

		if (Input::RightMouseDown() && !reloading) {
			aiming = true;
		}
		else 
			aiming = false;
		
		if (gunName != "nothing"){
			if (glfwGetTime() - reloadingTime > WeaponManager::GetGunByName(gunName)->reloadtime && reloading)
			{
				reloading = false;
				WeaponManager::GetGunByName(gunName)->currentammo = WeaponManager::GetGunByName(gunName)->ammo;
				WeaponManager::GetGunByName(gunName)->down = 1;
			}
			
			//get ray details
			if (Input::LeftMousePressed() && WeaponManager::GetGunByName(gunName)->type == Semi && glfwGetTime() - WeaponManager::GetGunByName(gunName)->lastTimeShot > 60.0f / WeaponManager::GetGunByName(gunName)->firerate && !reloading) {
				Shoot();
			}
			else if (Input::LeftMouseDown() && WeaponManager::GetGunByName(gunName)->type == Auto && glfwGetTime() - WeaponManager::GetGunByName(gunName)->lastTimeShot > 60.0f / WeaponManager::GetGunByName(gunName)->firerate && !reloading) {
				Shoot();
			}
			if (Input::KeyPressed('q') && !reloading) {
				//SceneManager::GetCurrentScene()->AddGunPickUp(gunName, gunName + "_pickup", getPosition() + Camera::GetDirection() * 1.5f);
				AssetManager::GetGameObject(gunName)->SetRender(false);
				gunName = "nothing";
			}
			
		}
		if (Input::KeyPressed('1')) {
			SelectWeapon(inv[0]);
		}

		if (Input::KeyPressed('2')) {
			SelectWeapon(inv[1]);
		}
		horizontalAngle += mouseSpeed * float(1024 / 2 - Input::GetMouseX());

		GameObject* head = AssetManager::GetGameObject("player_head");
		player->setRotation(glm::vec3(0, horizontalAngle, 0));
		head->setRotation(glm::vec3(-verticalAngle, horizontalAngle, 0));
		head->setPosition(player->getPosition() + glm::vec3(0, 1.5, 0));

		Camera::SetHorizontalAngle(horizontalAngle);
		Camera::SetVerticalAngle(verticalAngle);
		Camera::SetPosition(head->getPosition());
		

		if(gunName != "nothing")
			WeaponManager::GetGunByName(gunName)->Update(deltaTime, reloading, aiming);

		if ((Input::KeyDown('w') || Input::KeyDown('a') || Input::KeyDown('s') || Input::KeyDown('d')) && footstepTime + footstep_interval < glfwGetTime() ) {
			AudioManager::PlaySound("foot_step" + std::to_string((rand() % 4) + 1), AssetManager::GetGameObject("player")->getPosition());
			footstepTime = glfwGetTime();
		}
		//AssetManager::GetGameObject("player")->GetRigidBody()->setLinearVelocity(btVector3(0.0f, AssetManager::GetGameObject("player")->GetRigidBody()->getLinearVelocity().y(), 0.0f));
	}
	glm::vec3 Player::getPosition() {
		return AssetManager::GetGameObject("player")->getPosition();
	}
	glm::vec3 Player::getForward() {
		return forward;
	}
	void Player::setPosition(glm::vec3 pos) {
		AssetManager::GetGameObject("player")->setPosition(pos);
		Camera::SetPosition(AssetManager::GetGameObject("player")->getPosition());
	}
	std::string Player::GetInteractingWithName() {
		return interactingWithName;
	}
	std::string Player::getCurrentGun() {
		return gunName;
	}
	bool Player::SelectWeapon(std::string weaponName) {
		if (reloading)
			return false;
		if(gunName != "nothing")
			AssetManager::GetGameObject(WeaponManager::GetGunByName(gunName)->gunModel)->SetRender(false);
		gunName = weaponName;
		AssetManager::GetGameObject(WeaponManager::GetGunByName(gunName)->gunModel)->SetRender(true);
		return true;
	}
	void Player::SwitchWeapons(int index) {
		AssetManager::GetGameObject(inv[index])->SetRender(false);
		gunName = inv[index];
	}


}