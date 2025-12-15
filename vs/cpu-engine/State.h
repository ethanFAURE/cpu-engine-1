#pragma once

struct cpu_state_base
{
	virtual void OnEnter(void* user, int from) = 0;
	virtual void OnExecute(void* user) = 0;
	virtual void OnExit(void* user, int to) = 0;
};

template <typename T>
struct cpu_state : public cpu_state_base
{
	void OnEnter(void* user, int from) override { onEnter(*(T*)user, from); }
	void OnExecute(void* user) override { onExecute(*(T*)user); }
	void OnExit(void* user, int to) override { onExit(*(T*)user, to); }

	using EnterFn = void(*)(T& instance, int from);
	using ExecuteFn = void(*)(T& instance);
	using ExitFn = void(*)(T& instance, int to);

	EnterFn onEnter = nullptr;
	ExecuteFn onExecute = nullptr;
	ExitFn onExit = nullptr;
};

class cpu_fsm_base
{
public:
	cpu_fsm_base()
	{
		dead = false;
		index = -1;
		sortedIndex = -1;

		m_pReceiver = nullptr;
		m_current = -1;
		m_pending = -1;
		m_time = 0.0f;
	}
	virtual ~cpu_fsm_base()
	{
		for ( auto it=m_states.begin() ; it!=m_states.end() ; ++it )
			delete *it;
	}

	void ToState(int to)
	{
		m_pending = to;
	}

	int GetState() const
	{
		return m_current;
	}

	float GetTime() const
	{
		return m_time;
	}

	void Update(float dt)
	{
		m_time += dt;

		if ( m_pending!=m_current )
		{
			int from = m_current;
			int to = m_pending;

			if ( m_current!=-1 )
				m_states[m_current]->OnExit(m_pReceiver, to);

			m_current = to;

			if ( m_current!=-1 )
			{
				m_time = 0.0f;
				m_states[m_current]->OnEnter(m_pReceiver, from);
			}
		}

		if ( m_current!=-1 )
			m_states[m_current]->OnExecute(m_pReceiver);
	}

public:
	bool dead;
	int index;
	int sortedIndex;

protected:
	void* m_pReceiver;
	std::vector<cpu_state_base*> m_states;
	int m_current;
	int m_pending;
	float m_time;
};

template <typename T>
class cpu_fsm : public cpu_fsm_base
{
public:
	cpu_fsm(T* pReceiver)
	{
		m_pReceiver = pReceiver;
	}

	template <typename S>
	void Add()
	{
		static_assert(std::is_base_of_v<cpu_state<T>, S>, "S is not derived from cpu_state<T>");
		S* pState = new S;
		pState->id = (int)m_states.size();
		pState->onEnter = &S::OnEnter;
		pState->onExecute = &S::OnExecute;
		pState->onExit = &S::OnExit;
		m_states.push_back(pState);
	}
};
