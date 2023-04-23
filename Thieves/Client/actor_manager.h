#pragma once


class Actor;

class ActorManager final : public IBaseObjectManager
{
public:
	ActorManager() = default;
	virtual ~ActorManager() = default;

	ActorManager(const ActorManager&) = delete;
	ActorManager& operator=(const ActorManager&) = delete;

	virtual void Shutdown() override;
	virtual void Update(float delta_time) override;
	void UpdateWorldMatrix();

	void RegisterActor(const shared_ptr<Actor>& actor);

private:
	void UpdateStaticActors(float delta_time);
	void UpdateDestructibleActors(float delta_time);
	void UpdateMovableActors(float delta_time);

private:
	std::vector<shared_ptr<Actor>> m_ready_actors;
	std::vector<shared_ptr<Actor>> m_static_actors;
	std::vector<shared_ptr<Actor>> m_destructible_actors;
	std::vector<shared_ptr<Actor>> m_movable_actors;

	std::map<std::string, UINT64> m_actor_names;
};