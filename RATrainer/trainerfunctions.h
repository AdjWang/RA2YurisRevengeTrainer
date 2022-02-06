
#pragma once
#include "trainerbase.h"


class TrainerFunctions : public TrainerBase
{
public:
	//��ȡ�Ĵ���
	BOOL rSpeed(DWORD* pSpeed); //��ȡ�ٶ�
	BOOL rMymoney(DWORD* pMoney);
	BOOL rBuildMaxm(DWORD* pMaxm);
	//д��Ĵ���
	BOOL wSpeed(DWORD Speed); //д���ٶ�
	BOOL wMymoney(DWORD money);
	BOOL wBuildMaxm(DWORD maxm);

	BOOL QuickBuild();
	BOOL RadarOn();
	BOOL SuperOn();
	void WinImme();
	BOOL TobeGhost();
	//�����߳�
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
	//����ע��
	bool NoLoad(int command);
	void NoPower();		// �����У���ͣʹ��
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
