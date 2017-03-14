#pragma once
#include "PluginSDK.h"

bool BasicCheck(IMenuOption* menu, ISpell2* spell, IUnit* target, float range)
{
	if (menu->Enabled() && spell->IsReady() && target != nullptr && GEntityList->Player()->IsValidTarget(target, range))
	{
		return true;
	}
	else
	{
		return false;
	}
}
int GetEnemiesInRange(float range)
{
	int enemies = 0;
	for (auto enemy : GEntityList->GetAllHeros(false, true))
	{
		if (enemy != nullptr && GEntityList->Player()->IsValidTarget(enemy, range))
			enemies++;
	}
	return enemies;
}
int GetEnemiesInRangeUnit(IUnit* target, float range)
{
	auto enemiesInRange = 0;

	for (auto enemy : GEntityList->GetAllHeros(false, true))
	{
		if (enemy != nullptr && GEntityList->Player()->IsValidTarget(enemy, range) && enemy->GetNetworkId() != target->GetNetworkId())
		{
			auto distance = (target->GetPosition() - enemy->GetPosition()).Length();
			if (distance < range)
			{
				enemiesInRange++;
			}
		}
	}
	return enemiesInRange;
}
inline bool FoundMinionsNeutral(float range)
{
	for (auto Minions : GEntityList->GetAllMinions(false, false, true))
	{
		if (GEntityList->Player()->IsValidTarget(Minions, range))
			return true;
	}

	return false;
}

inline bool FoundMinions(float range)
{
	for (auto Minions : GEntityList->GetAllMinions(false, true, false))
	{
		if (GEntityList->Player()->IsValidTarget(Minions, range))
			return true;
	}

	return false;
}
int GetMinionsInRange(float range)
{
	int minions = 0;
	for (auto minion : GEntityList->GetAllMinions(false, true, false))
	{
		if (minion != nullptr && GEntityList->Player()->IsValidTarget(minion, range))
			minions++;
	}
	return minions;
}
static int CountEnemiesInAttackRange(IPluginSDK* sdk, float extraRange = 0.f, float multiplier = 1.f)
{
	static auto entityList = sdk->GetEntityList();
	static auto player = entityList->Player();

	auto count = 0;

	for (auto unit : entityList->GetAllHeros(false, true))
	{
		auto range = (player->GetRealAutoAttackRange(unit) + extraRange) * multiplier;
		if (player->IsValidTarget(unit, range))
		{
			count++;
		}
	}

	return count;
}
inline float GetDistance(IUnit* source, IUnit* target)
{
	auto x1 = source->GetPosition().x;
	auto x2 = target->GetPosition().x;
	auto y1 = source->GetPosition().y;
	auto y2 = target->GetPosition().y;
	auto z1 = source->GetPosition().z;
	auto z2 = target->GetPosition().z;
	return static_cast<float>(sqrt(pow((x2 - x1), 2.0) + pow((y2 - y1), 2.0) + pow((z2 - z1), 2.0)));
}