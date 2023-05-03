#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>

const unsigned int MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent
{
	protected:
		static int nextId;
};


template <typename T>
class Component: public IComponent
{
	static int GetId()
	{
		static auto id = nextId++;
		return id;
	}
};


class Entity
{
	private:
		int id;

public :
	Entity(int id) : id(id) {};
	Entity(const Entity& entity) = default;
	int GetId() const;

	Entity& operator =(const Entity& other) = default;
	bool operator == (const Entity& other) const {return id == other.id;}
	bool operator != (const Entity& other) const { return id != other.id;}
	bool operator > (const Entity& other) const { return id > other.id; }
	bool operator < (const Entity& other) const { return id < other.id; }
};


class System
{
	private:

		Signature componentSignature;

		std::vector<Entity> entities;

	public:

		System() = default;
		virtual ~System() = default;

		void AddEntityToSystem(Entity entity);
		void RemoveEntityFromSystem(Entity entity);
		const std::vector<Entity>& GetSystemEntities() const;
		const Signature& GetComponentSignature() const;

		template <typename TComponent> void RequireComponent();
};


class IPool
{
	public:
		virtual ~IPool() {}
};


template <typename T>
class Pool: IPool
{
	private:
		std::vector<T> data;

	public:
		Pool(int size = 100)
		{
			data.resize(size);
		}

		virtual ~Pool() = default;
	
		bool isEmpty() const
		{
			return data.empty();
		}

		int GetSize() const
		{
			return data.size();
		}

		void Resize(int n)
		{
			data.resize(n);
		}

		void Clear()
		{
			data.clear();
		}

		void Add(T object)
		{
			data.push_back(object);
		}

		void Set(int index, T object)
		{
			data[index] = object;
		}

		T& Get(int index)
		{
			return static_cast<T&>(data[index]);
		}

		T& operator [] (unsigned int index)
		{
			return data[index];
		}
};



class Registry
{

	private:
		int numEntities = 0;
		
		// Vector of component pools
		// Vector index = component type id
		// Pool index = entity id
		std::vector<IPool*> componentPools;

		// Vector of component signatures
		// Vector index = entity id
		std::vector<Signature> entityComponentSignatures;


		std::unordered_map<std::type_index, System*> systems;

		//Set of entities that are flagged to be added or removed in th next registry Update()

		std::set<Entity> entitiesToBeAdded;
		std::set<Entity> entitiesToBeKilled;


	public:

		Registry() = default;

		Entity CreateEntity();

		void Update();

		void KillEntity(Entity entity);

		void AddEntityToSystem(Entity entity);

		template <typename T, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
		//RemoveComponent(Entity entity)
		//HasComponent(Entity entity)

		//AddSystem()
		//RemoveSystem()
		//HasSystem()
		//GetSystem()

};


template <typename TComponent>
void System::RequireComponent()
{
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}



template <typename TComponent, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs&& ...args)
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	//If componentId is greater than the size of componentPooks, resize the vector
	if (componentId >= componentPools.size())
	{
		componentPools.resize(componentId + 1, nullptr);
	}

	//if we still dont have a pool for that component type
	if (!componentPools[componentId])
	{
		Pool<TComponent>* new ComponentPool = new Pool<TComponent>();
		componentPools[componentId] = newComponentPool;
	}

	Pool<TComponent>* componentPool = ComponentPools[componentId];

	if (entityId >= componentPool->GetSize())
	{
		componentPool->Resize(numEntities);
	}

	//Create a new Component object o the type T, and forward the various prameters to the constructor
	TComponent newComponent(std::forward<TArgs>(args)...);

	//Add the new component to the component pool list, using the entity Id as index
	componentPool->Set(entityId, newComponent);

	//Change the component signature of the entity and set the componentId to one 
	entityComponentSignatures[entityId].set(componentId);
}

#endif
