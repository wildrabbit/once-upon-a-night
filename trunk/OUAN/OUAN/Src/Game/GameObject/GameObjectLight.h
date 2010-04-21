#ifndef GameObjectLightH_H
#define GameObjectLightH_H

#include "GameObject.h"
#include "../../Graphics/RenderComponent/RenderComponentInitial.h"
#include "../../Graphics/RenderComponent/RenderComponentPositional.h"
#include "../../Graphics/RenderComponent/RenderComponentLight.h"
#include "../../Logic/LogicComponent/LogicComponent.h"
namespace OUAN
{
	/// Models a light source object
	class GameObjectLight : public GameObject, public boost::enable_shared_from_this<GameObjectLight>
	{
	private:
		/// Holds the information related to visual rendering
		RenderComponentLightPtr mRenderComponentLight;
		/// Holds information related to the object's position in space
		RenderComponentInitialPtr mRenderComponentInitial;
		RenderComponentPositionalPtr mRenderComponentPositional;
		/// Logic component: it'll represent the 'brains' of the game object
		/// containing information on its current state, its life and health(if applicable),
		/// or the world(s) the object belongs to
		LogicComponentPtr mLogicComponent;
	public:
		//Constructor
		GameObjectLight(const std::string& name);
		//Destructor
		~GameObjectLight();
		/// Set logic component
		void setLogicComponent(LogicComponentPtr logicComponent);

		/// return logic component
		LogicComponentPtr getLogicComponent();

		/// Get light component
		/// @return light component
		RenderComponentLightPtr getRenderComponentLight() const;

		/// Set light component
		/// @param pRenderComponentLight the light component to set
		void setRenderComponentLight(RenderComponentLightPtr pRenderComponentLight);

		/// Set positional component
		/// @param pRenderComponentPositional the component containing the positional information
		void setRenderComponentPositional(RenderComponentPositionalPtr pRenderComponentPositional);

		/// Set initial component
		void setRenderComponentInitial(RenderComponentInitialPtr pRenderComponentInitial);

		/// Return positional component 
		/// @return positional component
		RenderComponentPositionalPtr getRenderComponentPositional() const;

		/// Return initial component 
		/// @return initial component
		RenderComponentInitialPtr getRenderComponentInitial() const;

		/// React to a world change to the one given as a parameter
		/// @param world world to change to
		void changeWorld(int world);

		/// Reset object
		virtual void reset();

		bool hasPositionalComponent() const;
		RenderComponentPositionalPtr getPositionalComponent() const;


		/// Process collision event
		/// @param gameObject which has collision with
		void processCollision(GameObjectPtr pGameObject);

		/// Process collision event
		/// @param gameObject which has collision with
		void processEnterTrigger(GameObjectPtr pGameObject);

		/// Process collision event
		/// @param gameObject which has collision with
		void processExitTrigger(GameObjectPtr pGameObject);

		// update logic component
		void updateLogic(double elapsedSeconds);

	};

	/// Transport object carrying around data from the level loader
	/// to the light object
	class TGameObjectLightParameters: public TGameObjectParameters
	{
	public:
		/// Default constructor
		TGameObjectLightParameters();
		/// Default destructor
		~TGameObjectLightParameters();

		/// Light-specific parameters
		TRenderComponentLightParameters tRenderComponentLightParameters;
		/// Positional parameters
		TRenderComponentPositionalParameters tRenderComponentPositionalParameters;


		///Logic parameters
		TLogicComponentParameters tLogicComponentParameters;
	};
}
#endif