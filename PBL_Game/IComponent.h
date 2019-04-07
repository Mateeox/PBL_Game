#include "GameObject.h"
class IComponent {
	public:
		virtual void Start();
		virtual void Update();

	protected: 
		GameObject* owner;
};