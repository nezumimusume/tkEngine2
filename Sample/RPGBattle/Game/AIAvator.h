#pragma once
class IAICommand;
using IAICommandUPtr = std::unique_ptr<IAICommand>;

class AIAvator : public IGameObject
{
public:
	enum EnAnimation {
		enAnimation_Idle,
		enAnimation_Attack,
		enAnimation_Damage,
		enAnimation_Dead,
		enAnimation_Num,
	};
	AIAvator();
	~AIAvator();
	//AIのコマンドを生成。
	virtual void GenerateAICommandList(std::deque<IAICommandUPtr>& commands) = 0;
	void SetParameter(const wchar_t* name, int _hp, int _mp, int _atk, int _def) {
		m_name = name;
		hp = _hp;
		mp = _mp;
		atk = _atk;
		def = _def;
		maxHp = _hp;
		baseDef = def;
		baseAtk = atk;
	}
	int GetHP() const
	{
		return hp;
	}
	int GetMP() const
	{
		return mp;
	}
	void UseMP(int useMp)
	{
		mp -= useMp;
	}
	void AddHP(int add)
	{
		hp += add;
		hp = std::min<int>(hp, maxHp);
	}
	void AddDamage(int damage)
	{
		hp -= damage;
		hp = std::max<int>(0, hp);
	}
	void AddHate() {
		hate += 10;
	}
	void ApplySukuruto()
	{
		sukurutoBuffCount = 3;
		def = baseDef * 2;
	}
	void ApplyBaikiruto()
	{
		baikirutoBuffCount = 3;
		atk = baseAtk * 2;
	}
	void ApplyTenshiNoInori()
	{
		isTenshiNoInori = true;
	}
	void UpdateStatus() 
	{
		if (hp == 0) {
			//死亡したらヘイトは0になる。
			hate = 0;
			sukurutoBuffCount = 0;
			baikirutoBuffCount = 0;
			def = baseDef;
			atk = baseAtk;
		}
		else {
			//自然とヘイトは上がっていく。
			hate += 2;
			if (sukurutoBuffCount == 0) {
				//守備力を戻す。
				def = baseDef;
			}
			sukurutoBuffCount = std::max<int>(sukurutoBuffCount - 1, 0);
			if (baikirutoBuffCount == 0) {
				//攻撃力を戻す。
				atk = baseAtk;
			}
			baikirutoBuffCount = std::max<int>(baikirutoBuffCount - 1, 0);
		}
	}
	void HateToZero()
	{
		hate = 0;
	}
	virtual void PlayAnimation(int animNo, float interpolateTime)
	{
		(void)animNo;
		(void)interpolateTime;
	}
	virtual bool IsPlayAnimation()
	{
		return false;
	}
	virtual int GetCurrentAnimationNo() const
	{
		return -1;
	}
	const wchar_t* GetName() const
	{
		return m_name.c_str();
	}
	//ヘイトを取得。
	int GetHate() const
	{
		return hate;
	}
	int GetAtk() const
	{
		return atk;
	}
	int GetDef() const
	{
		return def;
	}
	std::wstring m_name;
	int hp;
	int mp;
	int atk;
	int def;
	int hate = 0;
	int maxHp;
	int baseDef = 0;
	int baseAtk = 0;
	int sukurutoBuffCount = 0;
	int baikirutoBuffCount = 0;
	bool isTenshiNoInori = false;
};

