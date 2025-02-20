#pragma once

class CoreManager
{
public:
	CoreManager();
	virtual ~CoreManager();

	void Init();
	void Update(int core);
	void DeadUpdate();
	void End();

	//�R�A�v�Z
	int Core(int core) { return m_core += core; }
	//�R�A����
	int GetCore() { return m_core; }
	//�R�A���g���ꍇ
	int SetCore(int core) { return m_core = core; }

public:
	//���x���グ�ɕK�v�ȃR�A��
	int NeedCore(int level);

private:
	int m_core;     //�v���C���[�����肵���R�A��S�ē����
};

