# OpenGL Engine 2.0

### Notice
This branch has the old custom physics engine I coded, The physics engine was very simple 3D physics, The new current branch is the BulletPhysics branch which uses the bullet physics engine for physics. I may 
Come back to this later and code my own physics but when I have a better understanding on physics engines.

### 3D Engine 2.0 2024/July/16
Lots of changes this week, Rendering pipe line had an upgrade with allowing textures to now having a normal map for increased graphics. An Audio system has also been added using FMOD allowing .wav files to be loaded and played with full 3D sound. Guns have been implemented with an ak47 and a pistol, pickup able weapons have also been added. Doors were also added and a system 
for scripts to be able to see what the player is interacting with was also added. A very basic animation system has also been added using keyframes that record transformations of an object.

![screenshot](/github/screenshots/gunsnmore.png)

### 3D Engine 2.0 2024/July/08
Rendering pipeline had a few changes most notably transparent textures and decals were added. Raycasting was also added, the camerea casts a ray giving you the intersection point at any AABB in the screenshot below I use this for bullet holes. GameObjects also
now have a parent-child hierarchy and there model matrix gets updated correctly. Soon I will have to redo the model system to incoperate assimp for animations.

![screenshot](/github/screenshots/Decals.png)



### 3D Engine 2.0 2024/July/04
This week I have implemented phyiscs, my implementation includes rigidbodies and axis alined boxes for collision, GameObjects can be used to produce a collider with the same demensions as the model. A Map editor is also in the works making it easier for 
me to edit the scenes, you can also save gameobjects in the scene to json files and load them through the Asset Manager. The Player was changed to work with the new physics system and the movement has been changed to add inertia making the movement more "realistic".


### 3D Engine 2.0 2024/June/30
This is a full rewrite of my first 3D OpenGL engine, the first engine succumbed to poorly written spaghetti code from when I was first learning OpenGl. This Engine has been more thoroughly thought out and will hopefully
allow me to make a simple FPS shooter game. So far the Engine has basic Rendering and Text Rendering, Basic Lighting, Scene Managment, Input Managment, Asset Managment. The next step for the engine is basic Physics and Audio which. 

![screenshot](/github/screenshots/3Dengine.png)
