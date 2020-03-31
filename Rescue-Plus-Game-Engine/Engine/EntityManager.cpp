#include "EntityManager.h"
#include "Time.h"

// Releases the entities in the Entity Manager
void EntityManager::Release()
{
	for (auto i = 0; i < entities.size(); i++)
	{
		if (entities[i]) { delete entities[i]; }
	}
}

//Adds an entity to the Entity Manager with a unique ID.
void EntityManager::AddEntity(GameObject* e)
{
	//Check the iterator of the entity
	if (std::find(entities.begin(), entities.end(), e) != entities.end())
	{
		printf("Cannot add entity %s because it is already in entity manager", e->GetName().c_str());
		return;
	}

	//Add to the list
	entities.push_back(e);
}

//Gets an entity from the Entity Manager with a certain name.
GameObject* EntityManager::GetEntity(std::string id)
{
	for (auto i = 0; i < entities.size(); i++)
	{
		if (entities[i]->GetName() == id) {
			return entities[i];
		}
	}

	return nullptr;
}

// Remove an entity by its object
void EntityManager::RemoveEntityFromList(GameObject* entity, bool release)
{
	GameObject* org = entity;
	std::vector<GameObject*>::iterator it = std::find(entities.begin(), entities.end(), entity);

	//Erase entity
	entities.erase(it);

	//Delete instance if user wants to
	if (release)
		delete org;

	return;
}

// Remove an entity by its name
void EntityManager::RemoveEntity(std::string name, bool deleteEntity)
{
	for (auto i = 0; i < entities.size(); i++)
	{
		if (entities[i]->GetName() == name)
		{
			entities[i]->SetEnabled(false);
			remove_entities.push_back(EntityRemoval{ entities[i], deleteEntity });
			return;
		}
	}

	printf("Entity of name %s does not exist in EntityManager. Cannot remove\n", name.c_str());
}

// Remove an entity by its object
void EntityManager::RemoveEntity(GameObject* entity, bool deleteEntity)
{
	//Get the iterator of the entity
	std::vector<GameObject*>::iterator it = std::find(entities.begin(), entities.end(), entity);
	if (it == entities.end())
	{
		printf("Cannot remove entity %s because it is not in entity manager\n", entity->GetName().c_str());
		return;
	}

	entity->SetEnabled(false);
	remove_entities.push_back(EntityRemoval{entity, deleteEntity});
	return;
}

// Run FixedUpdate() for all entities in the manager
void EntityManager::FixedUpdate()
{
	//Update entities
	for (size_t i = 0; i < entities.size(); i++)
	{
		if (entities[i] && entities[i]->GetEnabled())
		{
			entities[i]->FixedUpdate();
		}
	}

	//Remove entities
	for (size_t i = 0; i < remove_entities.size(); i++)
	{
		RemoveEntityFromList(remove_entities[i].e, remove_entities[i].release);
	}
	remove_entities.clear();
}

// Run Update() for all entities in the manager
void EntityManager::Update()
{
	//Update entities
	for (size_t i = 0; i < entities.size(); i++)
	{
		if (entities[i] && entities[i]->GetEnabled())
		{
			entities[i]->Update();
		}
	}

	//Remove entities
	for (size_t i = 0; i < remove_entities.size(); i++)
	{
		RemoveEntityFromList(remove_entities[i].e, remove_entities[i].release);
	}
	remove_entities.clear();
}