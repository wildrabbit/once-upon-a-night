#ifndef RenderComponentH_H
#define RenderComponentH_H

#include "../../Component/Component.h"
namespace OUAN
{
	class RenderComponent: public Component
	{
	public:
		RenderComponent(const std::string& type="");
		~RenderComponent();

		virtual void update(long elapsedTime);

		//Event handlers
		//void onDoSomething(EventData,emitter, ...);
		//void onDoSomethingElse(...);
	};

	class TRenderComponentParameters: public TComponentParameters
	{
	public:
		TRenderComponentParameters();
		~TRenderComponentParameters();
	};
}

#endif