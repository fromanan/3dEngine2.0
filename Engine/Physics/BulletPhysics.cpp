#include "BulletPhysics.h"
#include "Engine/Core/AssetManager.h"


/*
Collider::Collider() {

}
Collider::~Collider() {

}


Collider::Collider(GameObject* gameobject, std::string Name, float Mass) {
	std::vector<glm::vec3> vertices = gameobject->getIndexedVerticies();

	glm::vec4 startvert = glm::vec4(vertices[0].x, vertices[0].y, vertices[0].z, 1) * (glm::rotate(glm::mat4(1), -gameobject->getRotation().y, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), gameobject->getRotation().x, glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1), gameobject->getRotation().z, glm::vec3(0, 0, 1))) * glm::scale(glm::mat4(1), gameobject->getScale());
	float minx = startvert.x;
	float maxx = startvert.x;
	float miny = startvert.y;
	float maxy = startvert.y;
	float minz = startvert.z;
	float maxz = startvert.z;

	for (int i = 0; i < vertices.size() - 1; i++)
	{
		glm::vec4 tempVec(vertices[i].x, vertices[i].y, vertices[i].z, 1);
		tempVec = tempVec * (glm::rotate(glm::mat4(1), gameobject->getRotation().y, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), gameobject->getRotation().x, glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1), gameobject->getRotation().z, glm::vec3(0, 0, 1))) * glm::scale(glm::mat4(1), gameobject->getScale());
		if (tempVec.x < minx)
			minx = tempVec.x;
		if (tempVec.x > maxx)
			maxx = tempVec.x;

		if (tempVec.y < miny)
			miny = tempVec.y;
		if (tempVec.y > maxy)
			maxy = tempVec.y;

		if (tempVec.z < minz)
			minz = tempVec.z;
		if (tempVec.z > maxx)
			maxz = tempVec.z;


	}

	//1.05 is for padding because the camera can somtimes clip into the object
	float width = (maxx - minx) * 1.0;
	float height  = (maxy - miny) * 1.0;
	float depth  = (maxz - minz) * 1.0;


	glm::vec3 position = glm::vec3(gameobject->getPosition().x + (minx + maxx) / 2, gameobject->getPosition().y + (miny + maxy) / 2, gameobject->getPosition().z + (minz + maxz) / 2);

	this->name = Name;

	collider = new btBoxShape(btVector3(btScalar(width / 2), btScalar(height / 2), btScalar(depth / 2)));
	PhysicsManagerBullet::AddColliderShape(collider);
	bool isDynamic = (Mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		collider->calculateLocalInertia(btScalar(Mass), localInertia);

	transform.setIdentity();
	transform.setOrigin(btVector3(position.x, position.y, position.z));

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(btScalar(Mass), myMotionState, collider, localInertia);
	body = new btRigidBody(rbInfo);

	//add the body to the dynamics world
	PhysicsManagerBullet::AddRigidBody(body);
}


//width is radius if Sphere
Collider::Collider(glm::vec3 position, float width, float height, float depth,float mass, std::string Name, ColliderShape shape) {
	switch (shape) {
	case Box:
		collider = new btBoxShape(btVector3(btScalar(width / 2), btScalar(height / 2), btScalar(depth / 2)));

		break;
	case Sphere:
		collider = new btSphereShape(width);
		break;
	case Cylinder:
		collider = new btCylinderShape(btVector3(btScalar(width / 2), btScalar(height / 2), btScalar(depth / 2)));
		break;
	default:
		collider = new btBoxShape(btVector3(btScalar(width / 2), btScalar(height / 2), btScalar(depth / 2)));
	}
	collider = new btBoxShape(btVector3(btScalar(width/2), btScalar(height/2), btScalar(depth/2)));
	PhysicsManagerBullet::AddColliderShape(collider);
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		collider->calculateLocalInertia(btScalar(mass), localInertia);

	transform.setIdentity();
	transform.setOrigin(btVector3(position.x, position.y, position.z));

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(btScalar(mass), myMotionState, collider, localInertia);
	body = new btRigidBody(rbInfo);

	//add the body to the dynamics world
	PhysicsManagerBullet::AddRigidBody(body);
	name = Name;
}

std::string Collider::GetName() {
	return name;
}
std::string Collider::GetTag() {
	return tag;
}
void Collider::SetTag(std::string Tag) {
	tag = Tag;
}

void Collider::SetPosition(glm::vec3 position) {
	transform.setOrigin(btVector3(position.x, position.y, position.z));
}
glm::vec3 Collider::GetPosition() {
	return glm::vec3(body->getWorldTransform().getOrigin().getX(), body->getWorldTransform().getOrigin().getY(), body->getWorldTransform().getOrigin().getZ());
}
glm::vec3 Collider::GetRotation() {
	return glm::eulerAngles(glm::quat(body->getWorldTransform().getRotation().w(), body->getWorldTransform().getRotation().x(), body->getWorldTransform().getRotation().y(), body->getWorldTransform().getRotation().z()));
}

void Collider::SetDepth() {
	
}
void Collider::SetHeight() {

}
void Collider::SetWidth() {

}
void Collider::SetDimensions(float width, float height, float depth) {

}
//void Collider::Regenerate(GameObject* gameobject) {

//}


btRigidBody* Collider::GetRigidBody() {
	return body;
}
btCollisionShape* Collider::GetCollisionShape() {
	return collider;
}
*/





namespace PhysicsManagerBullet {
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	//std::vector<Collider> colliders;


	btDiscreteDynamicsWorld* dynamicsWorld;
	btSequentialImpulseConstraintSolver* solver;
	btBroadphaseInterface* overlappingPairCache;
	btCollisionDispatcher* dispatcher;
	btDefaultCollisionConfiguration* collisionConfiguration;
	/*
	void AddCollider(Collider collider) {
		colliders.push_back(collider);
	}
	Collider* GetCollider(std::string Name) {
		for (int i = 0; i < colliders.size(); i++) {
			if (colliders[i].GetName() == Name)
				return &colliders[i];
		}
		return NULL;
	}
	void RemoveCollider(std::string name) {
		for (int i = 0; i < colliders.size(); i++) {
			if (colliders[i].GetName() == name)
				colliders.erase(colliders.begin() + i);
		}
	}
	*/

	void AddColliderShape(btCollisionShape* collider) {
		collisionShapes.push_back(collider);
	}
	void AddRigidBody(btRigidBody* body) {
		dynamicsWorld->addRigidBody(body);
	}
	btDiscreteDynamicsWorld* GetDynamicWorld() {
		return dynamicsWorld;
	}


	void Init() {

		///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
		collisionConfiguration = new btDefaultCollisionConfiguration();

		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		dispatcher = new btCollisionDispatcher(collisionConfiguration);

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		overlappingPairCache = new btDbvtBroadphase();

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		solver = new btSequentialImpulseConstraintSolver;

		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

		dynamicsWorld->setGravity(btVector3(0, -10, 0));



	}
	void Update(float deltaTime) {
		dynamicsWorld->stepSimulation(deltaTime,30);

		//print positions of all objects
		for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
		{
			btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			btTransform trans;
			if (body && body->getMotionState())
			{
				body->getMotionState()->getWorldTransform(trans);
			}
			else
			{
				trans = obj->getWorldTransform();
			}

		}

		dynamicsWorld->debugDrawWorld();
	}
}
