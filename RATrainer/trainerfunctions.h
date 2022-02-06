
#pragma once
#include "trainerbase.h"


class TrainerFunctions : public TrainerBase
{
public:
	//读取寄存器
	BOOL rSpeed(DWORD* pSpeed); //读取速度
	BOOL rMymoney(DWORD* pMoney);
	BOOL rBuildMaxm(DWORD* pMaxm);
	//写入寄存器
	BOOL wSpeed(DWORD Speed); //写入速度
	BOOL wMymoney(DWORD money);
	BOOL wBuildMaxm(DWORD maxm);

	BOOL QuickBuild();
	BOOL RadarOn();
	BOOL SuperOn();
	void WinImme();
	BOOL TobeGhost();
	//创建线程
	static void Map_Assemble();
	void AllMap();
	static void NuclearBomb_Assemble();
	void GetaNuclearBomb();
	static void UpChose_Assemble();
	void UpChose();
	static void SpeedUpChose_Assemble();
	void SpeedUpChose();
	static void MineChose_Assemble();
	void MineChose();
	static void DeleteThis_Assemble();
	void DeleteThis();
	//代码注入
	bool NoLoad(int command);
	void NoPower();		// 开发中，暂停使用
	bool PutAsWill(int command);
	bool AutoRepair(int command);


	bool RevengeYuri(int command);
	bool MineAttack(int command);
	bool MineBuildIn(int command);
	bool MineUnEngineer(int command);
	bool BuildImme(int command);

	bool Unbeatable(int command);
	bool AntiChrono(int command);
	bool AntiChronoDisable(int command);

	bool UnRepair(int command);

	bool InvadeMode(int command);

	bool UnlockTech(int command);

	bool FastAttack(int command);
	bool FastTurnBattery(int command);
	bool FastTurnRound(int command);
	bool FastReload(int command);
	bool FullAmmunition(int command);

	bool AllRangeAttack(int command);
	bool AllRangeAlert(int command);

	bool InstantChronoMove(int command);
	bool InstantChronoAttack(int command);

	bool AntiSpy(int command);

	bool DisableGAGAP(int command);

	bool DisableAll(int command);

	bool SpeedSet(int command);

	bool EnableSoldAll_Cursor(int command);
	bool EnableSoldAll_Belong(int command);
	bool EnableSoldAll_Builder(int command);

	bool SoldierFlashMove(int command);

	bool AllElite(int command);

};
