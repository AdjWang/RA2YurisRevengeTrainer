
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
	static void NoLoad_Assemble();
	void NoLoad();
	void FreeNoLoad();
	static void NoPower_Assemble();
	void NoPower();
	void FreeNoPower();
	static void PutAsWill_Assemble();
	void PutAsWill();
	void FreePutAsWill();
	static void PutWaterAble_Assemble();
	void PutWaterAble();
	void FreePutWaterAble();
	static void AutoRepair_Assemble();
	void AutoRepair();
	void FreeAutoRepair();
	static void AutoRepairStruc_Assemble();
	void AutoRepairStruc();
	void FreeAutoRepairStruc();
	static void RevengeYuri_Assemble();
	void RevengeYuri();
	void FreeRevengeYuri();
	static void PermanentYuri_Assemble();
	void PermanentYuri();
	void FreePermanentYuri();
	static void MineAttack_Assemble();
	void MineAttack();
	void FreeMineAttack();
	static void MineBuildIn_Assemble();
	void MineBuildIn();
	void FreeMineBuildIn();
	static void MineUnEngineer_Assemble();
	void MineUnEngineer();
	void FreeMineUnEngineer();
	static void BuildImme_Assemble();
	void BuildImme();
	void FreeBuildImme();

	static void Unbeatable_Assemble();
	static void SecKill_Assemble();
	static void UnbeatableSecKill_Assemble();
	void UnbeatableSecKill_Pre();
	void Unbeatable();
	void SecKill();
	void UnbeatableSecKill();
	void FreeUnbeatableSecKill();
	static void AntiChrono_Assemble();
	void AntiChrono();
	void FreeAntiChrono();
	static void AntiChronoDisbuild_Assemble();
	void AntiChronoDisbuild();
	void FreeAntiChronoDisbuild();
	void AntiChronoDisable();
	void FreeAntiChronoDisable();

	static void UnRepair_Assemble();
	void UnRepair();
	void FreeUnRepair();

	void UnRefreshCon();
	void FreeUnRefreshCon();
	static void AllBuild_Assemble();
	void AllBuild();
	void FreeAllBuild();
	static void AllCarBoat_Assemble();
	void AllCarBoat();
	void FreeAllCarBoat();
	static void AllSoldier_Assemble();
	void AllSoldier();
	void FreeAllSoldier();
	static void AllPlane_Assemble();
	void AllPlane();
	void FreeAllPlane();

	static void FastAttack_Assemble();
	void FastAttack();
	void FreeFastAttack();
	static void FastTurnBattery_Assemble();
	void FastTurnBattery();
	void FreeFastTurnBattery();
	static void FastTurnRound_Assemble();
	void FastTurnRound();
	void FreeFastTurnRound();
	static void FastReload_Assemble();
	void FastReload();
	void FreeFastReload();
	static void FullAmmunition_Assemble();
	void FullAmmunition();
	void FreeFullAmmunition();

	static void AllRangeAttack_Assemble();
	void AllRangeAttack();
	void FreeAllRangeAttack();
	static void AllRangeAlert_Assemble();
	void AllRangeAlert();
	void FreeAllRangeAlert();

	static void InstantChronoMove_Assemble();
	void InstantChronoMove();
	void FreeInstantChronoMove();
	static void InstantChronoAttack_Assemble();
	void InstantChronoAttack();
	void FreeInstantChronoAttack();

	static void AntiSpy_Assemble();
	void AntiSpy();
	void FreeAntiSpy();

	static void DisableGAGAP_Assemble();
	void DisableGAGAP();
	void FreeDisableGAGAP();

	static void DisableAll_Assemble();
	void DisableAll();
	void FreeDisableAll();

	void SpeedSet();
	void FreeSpeedSet();

	static void EnableSoldAll_Cursor_Assemble();
	void EnableSoldAll_Cursor();
	void FreeEnableSoldAll_Cursor();
	void EnableSoldAll_Belong();
	void FreeEnableSoldAll_Belong();
	void EnableSoldAll_Builder();
	void FreeEnableSoldAll_Builder();

	static void SoldierFlashMove_Assemble();
	void SoldierFlashMove();
	void FreeSoldierFlashMove();

	static void EliteBuild_Aircraft_Assemble();
	void EliteBuild_Aircraft();
	void FreeEliteBuild_Aircraft();
	void EliteBuild_Vehicle();		//车船与飞机的汇编代码相同
	void FreeEliteBuild_Vehicle();
	static void EliteBuild_Soldier_Assemble();
	void EliteBuild_Soldier();
	void FreeEliteBuild_Soldier();
};

