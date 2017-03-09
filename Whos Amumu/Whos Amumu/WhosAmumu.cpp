#pragma once
#include "PluginSDK.h"
#include "AmumuCommon.h"
#include <map>

PluginSetup("Whos Amumu?");

IMenu* MainMenu;
IMenu* ComboMenu;
IMenu* LaneClearMenu;
IMenu* JungleClearMenu;
IMenu* KillstealMenu;
IMenu* MiscMenu;
IMenu* ForceUltMenu;
IMenu* DrawingsMenu;

IMenuOption* ComboQ;
//IMenuOption* ComboSmiteQ;
IMenuOption* ComboW;
IMenuOption* ComboWMana;
IMenuOption* ComboE;
IMenuOption* ComboR;
IMenuOption* ComboRxNum;
IMenuOption* ComboREnemyHP;

IMenuOption* LaneclearW;
IMenuOption* LaneclearWMana;
IMenuOption* LaneclearWxNum;
IMenuOption* LaneclearE;
IMenuOption* LaneclearEMana;
IMenuOption* LaneclearExNum;

IMenuOption* JungleclearQ;
IMenuOption* JungleclearQMana;
IMenuOption* JungleclearW;
IMenuOption* JungleclearWMana;
IMenuOption* JungleclearE;
IMenuOption* JungleclearEMana;

IMenuOption* KillstealQ;
IMenuOption* KillstealE;
IMenuOption* KillstealR;
IMenuOption* KillstealRHC;

IMenuOption* AutoR;
IMenuOption* AutoRxNum;
std::map<int, IMenuOption*> AutoRTar;
IMenuOption* InterruptQ;
IMenuOption* AntiGapQ;
IMenuOption* WManaSave;
IMenuOption* WManaSaveM;

IMenuOption* DrawReady;
IMenuOption* DrawQ;
IMenuOption* DrawR;

ISpell2* Q;
ISpell2* W;
ISpell2* E;
ISpell2* R;

void Menu()
{
	MainMenu = GPluginSDK->AddMenu("Whos Amumu?");
	ComboMenu = MainMenu->AddMenu("Combo");
	LaneClearMenu = MainMenu->AddMenu("Laneclear");
	JungleClearMenu = MainMenu->AddMenu("Jungleclear");
	KillstealMenu = MainMenu->AddMenu("Killsteal");
	MiscMenu = MainMenu->AddMenu("Miscs");
	DrawingsMenu = MainMenu->AddMenu("Drawings");

	ComboQ = ComboMenu->CheckBox("Use Q", true);
	//ComboSmiteQ = ComboMenu->CheckBox("-> Smite Collision", true);
	ComboW = ComboMenu->CheckBox("Use W", true);
	ComboWMana = ComboMenu->AddFloat("-> If Mp >=", 0, 100, 20);
	ComboE = ComboMenu->CheckBox("Use E", true);
	ComboR = ComboMenu->CheckBox("Use R", true);
	ComboRxNum = ComboMenu->AddFloat("-> If Enemy Hit >=", 1, 5, 2);
	ComboREnemyHP = ComboMenu->AddFloat("-> Or Target Hp <=", 0, 100, 60);

	LaneclearW = LaneClearMenu->CheckBox("Use W", true);
	LaneclearWMana = LaneClearMenu->AddFloat("-> If Mp >=", 0, 100, 50);
	LaneclearWxNum = LaneClearMenu->AddFloat("-> If Minions In Range >=", 1, 10, 3);
	LaneclearE = LaneClearMenu->CheckBox("Use E", true);
	LaneclearEMana = LaneClearMenu->AddFloat("-> If Mp >=", 0, 100, 50);
	LaneclearExNum = LaneClearMenu->AddFloat("-> If Minions In Range >=", 1, 10, 3);

	JungleclearQ = JungleClearMenu->CheckBox("Use Q", true);
	JungleclearQMana = JungleClearMenu->AddFloat("-> If Mp >=", 0, 100, 50);
	JungleclearW = JungleClearMenu->CheckBox("Use W", true);
	JungleclearWMana = JungleClearMenu->AddFloat("-> If Mp >=", 0, 100, 20);
	JungleclearE = JungleClearMenu->CheckBox("Use E", true);
	JungleclearEMana = JungleClearMenu->AddFloat("-> If Mp >=", 0, 100, 20);

	KillstealQ = KillstealMenu->CheckBox("Use Q", true);
	KillstealE = KillstealMenu->CheckBox("Use E", true);
	KillstealR = KillstealMenu->CheckBox("Use R", true);
	KillstealRHC = KillstealMenu->AddFloat("-> If My HP <=", 0, 100, 10);


	AutoR = MiscMenu->CheckBox("Auto R", true);
	AutoRxNum = MiscMenu->AddFloat("-> If Enemy Hit >=", 1, 5, 4);
	ForceUltMenu = MiscMenu->AddMenu("Force Ult");
	for (auto enemy : GEntityList->GetAllHeros(false, true))
	{
		std::string szMenuName = "Force R on " + std::string(enemy->ChampionName());
		AutoRTar[enemy->GetNetworkId()] = ForceUltMenu->CheckBox(szMenuName.c_str(), false);
	}
	InterruptQ = MiscMenu->CheckBox("Use Q Interrupt", true);
	AntiGapQ = MiscMenu->CheckBox("Use Q Anti-Gapcloser", true);
	WManaSave = MiscMenu->CheckBox("Auto Stop W", true);
	WManaSaveM = MiscMenu->AddFloat("-> If Mp <=", 0, 100, 20);

	DrawReady = DrawingsMenu->CheckBox("Draw Ready Spells", true);
	DrawQ = DrawingsMenu->CheckBox("Draw Q", true);
	DrawR = DrawingsMenu->CheckBox("Draw R", true);
}
void LoadSpells()
{
	Q = GPluginSDK->CreateSpell2(kSlotQ, kLineCast, true, false, static_cast<eCollisionFlags>(kCollidesWithMinions | kCollidesWithYasuoWall));
	W = GPluginSDK->CreateSpell2(kSlotW, kCircleCast, false, true, kCollidesWithNothing);
	E = GPluginSDK->CreateSpell2(kSlotE, kCircleCast, false, true, kCollidesWithNothing);
	R = GPluginSDK->CreateSpell2(kSlotR, kCircleCast, false, true, kCollidesWithNothing);
	Q->SetOverrideRange(1100);
	Q->SetOverrideDelay(0.25f);
	Q->SetOverrideRadius(90);
	Q->SetOverrideSpeed(2000);
	W->SetOverrideRange(300);
	E->SetOverrideRange(350);
	E->SetOverrideDelay(0.5f);
	R->SetOverrideRange(550);
	R->SetOverrideDelay(0.25F);
}
void Combo()
{
	auto player = GEntityList->Player();
	auto TQ = GTargetSelector->FindTarget(QuickestKill, SpellDamage, Q->Range());
	auto TW = GTargetSelector->FindTarget(QuickestKill, SpellDamage, W->Range());
	auto TE = GTargetSelector->FindTarget(QuickestKill, SpellDamage, E->Range());
	auto TR = GTargetSelector->FindTarget(QuickestKill, SpellDamage, R->Range());

	if (ComboQ->Enabled() && Q->IsReady())
	{
		if (R->IsReady() && ComboRxNum->GetFloat() > 0)
		{
			int maxTargetsHit = 0;
			IUnit* unitMostTargetsHit = nullptr;
			
			auto pred = new AdvPredictionOutput();
			for (auto enemy : GEntityList->GetAllHeros(false, true))
			{
				if (Q->RunPrediction(enemy, false, static_cast<eCollisionFlags>(kCollidesWithMinions | kCollidesWithYasuoWall), pred))
				{
					if (player->IsValidTarget(enemy, Q->Range()) && enemy->IsValidTarget() && pred->HitChance >= kHitChanceHigh)
					{
						int targetsHit = GetEnemiesInRangeUnit(enemy, R->Range());
						if (targetsHit > maxTargetsHit || unitMostTargetsHit != nullptr && targetsHit >= maxTargetsHit && enemy->IsValidTarget())
						{
							maxTargetsHit = targetsHit;
							unitMostTargetsHit = enemy;
						}
					}
				}
			}

			if (maxTargetsHit >= ComboRxNum->GetFloat())
			{
				Q->CastOnTarget(unitMostTargetsHit);
			}		
		}

		if(TQ != nullptr)
		{
			Q->CastOnTarget(TQ, kHitChanceHigh);
		}
	}

	if (!player->HasBuff("AuraofDespair"))
	{
		if (BasicCheck(ComboW, W, TW, W->Range()) && player->ManaPercent() >= ComboWMana->GetFloat())
		{
			W->CastOnPlayer();
		}
	}

	if (BasicCheck(ComboE, E, TE, E->Range()))
	{
		E->CastOnPlayer();
	}

	if (ComboR->Enabled() && R->IsReady() && GetEnemiesInRange(R->Range()) >= ComboRxNum->GetFloat() ||
		BasicCheck(ComboR, R, TR, R->Range()) && TR->HealthPercent() <= ComboREnemyHP->GetFloat())
	{
		R->CastOnPlayer();
	}
}
void Laneclear()
{
	auto player = GEntityList->Player();

	if (!FoundMinionsNeutral(600) && FoundMinions(600))
	{
		for (auto minion : GEntityList->GetAllMinions(false, true, false))
		{
			if (!player->HasBuff("AuraofDespair"))
			{
				if (BasicCheck(LaneclearW, W, minion, W->Range()) && !minion->IsDead() 
					&& player->ManaPercent() >= LaneclearWMana->GetFloat() && GetMinionsInRange(W->Range()) >= LaneclearWxNum->GetFloat())
				{
					W->CastOnPlayer();
					return;
				}
			}

			if (BasicCheck(LaneclearE, E, minion, E->Range()) && !minion->IsDead()
				&& player->ManaPercent() >= LaneclearEMana->GetFloat() && GetMinionsInRange(E->Range()) >= LaneclearExNum->GetFloat())
			{
				E->CastOnPlayer();
				return;
			}
		}
	}
}
void Jungleclear()
{
	auto player = GEntityList->Player();

	if (FoundMinionsNeutral(600) && !FoundMinions(600))
	{
		for (auto monster : GEntityList->GetAllMinions(false, false, true))
		{
			if (BasicCheck(JungleclearQ, Q, monster, Q->Range()) && !monster->IsDead()
				&& player->ManaPercent() >= JungleclearQMana->GetFloat() && GetDistance(player, monster) > GOrbwalking->GetAutoAttackRange(player))
			{
				Q->CastOnTarget(monster, kHitChanceMedium);
				return;
			}

			if (!player->HasBuff("AuraofDespair"))
			{
				if (BasicCheck(JungleclearW, W, monster, W->Range()) && !monster->IsDead()
					&& player->ManaPercent() >= JungleclearWMana->GetFloat())
				{
					W->CastOnPlayer();
					return;
				}
			}

			if (BasicCheck(JungleclearE, E, monster, E->Range()) && !monster->IsDead()
				&& player->ManaPercent() >= JungleclearEMana->GetFloat())
			{
				E->CastOnPlayer();
				return;
			}
		}
	}
}
void Killsteal()
{
	for (auto target : GEntityList->GetAllHeros(false, true))
	{
		auto player = GEntityList->Player();

		if (target != nullptr) continue;
		{
			if (BasicCheck(KillstealQ, Q, target, Q->Range()) 
				&& GHealthPrediction->GetKSDamage(target, kSlotQ, Q->GetDelay(), false) > target->GetHealth())
			{
				Q->CastOnTarget(target, kHitChanceHigh);
				return;
			}

			if (BasicCheck(KillstealE, E, target, E->Range())
				&& GHealthPrediction->GetKSDamage(target, kSlotE, E->GetDelay(), false) > target->GetHealth())
			{
				E->CastOnPlayer();
				return;
			}

			if (BasicCheck(KillstealR, R, target, R->Range()) && player->HealthPercent() <= KillstealRHC->GetFloat()
				&& GHealthPrediction->GetKSDamage(target, kSlotR, R->GetDelay(), false) > target->GetHealth())
			{
				R->CastOnPlayer();
				return;
			}
		}
	}
}
void Interrupt(InterruptibleSpell const& Args)
{
	auto player = GEntityList->Player();

	if (InterruptQ->Enabled() && Q->IsReady()
		&& player->IsValidTarget(Args.Target, Q->Range())
		&& Args.Target->GetTeam() != player->GetTeam()
		&& Args.Target != player)
	{
		Q->CastOnUnit(Args.Target);
	}
}
void AntiGapcloser(GapCloserSpell const& Args)
{
	auto player = GEntityList->Player();

	if (AntiGapQ->Enabled() && Q->IsReady()
		&& player->IsValidTarget(Args.Sender, Q->Range())
		&& Args.Sender->GetTeam() != player->GetTeam()
		&& Args.Sender != player)
	{
		Q->CastOnUnit(Args.Sender);
	}
}
void AutoUlt()
{
	auto TAR = GTargetSelector->FindTarget(QuickestKill, SpellDamage, R->Range());

	if (AutoR->Enabled() && R->IsReady() && TAR != nullptr && GetEnemiesInRange(R->Range()) >= AutoRxNum->GetFloat() ||
		R->IsReady() && TAR != nullptr && AutoRTar[TAR->GetNetworkId()]->Enabled())
	{
		R->CastOnPlayer();
	}
}
void WMana()
{
	auto player = GEntityList->Player();

	if (WManaSave->Enabled() && player->HasBuff("AuraofDespair") && player->ManaPercent() <= WManaSaveM->GetFloat())
	{
		W->CastOnPlayer();
	}
}
/*void SmiteQ()
{
	if (ComboSmiteQ->Enabled())
	{
		int Count = 0;

		auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());

		if (!CheckTarget(target)) return;

		if (GEntityList->Player()->IsValidTarget(target, Q->Range()))
		{
			auto startPos = GEntityList->Player()->GetPosition();
			auto endPos = target->GetPosition();

			auto cRect = Geometry::Rectangle(startPos.To2D(), endPos.To2D(), Q->Radius());

			for (auto minion : GEntityList->GetAllMinions(false, true, false))
			{
				if (GetDistance(GEntityList->Player(), minion) < 550)
				{
					if (cRect.IsInside(minion))
					{
						Count++;
					}

					if (Count > 0 && Count <= 1)
					{
						auto damage = GDamage->GetSummonerSpellDamage(GEntityList->Player(), minion, kSummonerSpellSmite);

						if (GEntityList->Player()->IsFacing(target) && CountMinions(minion->GetPosition(), 50) < 2 && damage > minion->GetHealth())
						{
							Smite->CastOnUnit(minion);
						}
					}
				}
			}
		}
	}
}*/
void Drawings()
{
	if (DrawReady->Enabled())
	{
		if (Q->IsReady() && DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), Q->Range()); }
		if (R->IsReady() && DrawR->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), R->Range()); }
	}
	else
	{
		if (Q->IsReady() && DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), Q->Range()); }
		if (R->IsReady() && DrawR->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), R->Range()); }
	}
}
PLUGIN_EVENT(void) OnRender()
{
	Drawings();
}
PLUGIN_EVENT(void) OnGameUpdate()
{
	Killsteal();
	AutoUlt();
	WMana();

	if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
	{
		Combo();
	}

	if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
	{
		Laneclear();
		Jungleclear();
	}
}
PLUGIN_EVENT(void) OnInterrupt(InterruptibleSpell const& Args)
{
	Interrupt(Args);
}
PLUGIN_EVENT(void) OnGapcloser(GapCloserSpell const& Args)
{
	AntiGapcloser(Args);
}
PLUGIN_API void OnLoad(IPluginSDK* PluginSDK)
{
	PluginSDKSetup(PluginSDK);
	Menu();
	LoadSpells();

	GEventManager->AddEventHandler(kEventOnRender, OnRender);
	GEventManager->AddEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->AddEventHandler(kEventOnInterruptible, OnInterrupt);
	GEventManager->AddEventHandler(kEventOnGapCloser, OnGapcloser);
	GGame->PrintChat("Whos Amumu? Loaded");
}
PLUGIN_API void OnUnload()
{
	MainMenu->Remove();

	GEventManager->RemoveEventHandler(kEventOnRender, OnRender);
	GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->RemoveEventHandler(kEventOnInterruptible, OnInterrupt);
	GEventManager->RemoveEventHandler(kEventOnGapCloser, OnGapcloser);
}