-- cutscenes.lua
function helloWorldSkip()
	--local any=getAny()
	--if any then
		--any:setOuternMovement(...)
	--end	
end

TRACK_TIME=2

SENTENCE1_TIME = 3
SENTENCE2_TIME = 3

-- NOTE: Check common.lua for some frequently used functions!!

function helloWorld(timer)
	setCameraTrajectory("LEVEL_START")	
	trajectoryCamWait()
	
	log ("Switching to a tracking camera!!")
	setAnyTrackingCamera()
	
	log ("narrative pause")
	busyWait(timer,TRACK_TIME)

	local any=getAny()	
	timedSay(any,"CUTSCENE_HELLOWORLD_INTRO",1,timer,SENTENCE1_TIME)	
	log ("Any message 1")
	timedSay(any,"CUTSCENE_HELLOWORLD_SECOND",1,timer,SENTENCE2_TIME)
	log ("Any message 2")	
	timedSay(any,"CUTSCENE_HELLOWORLD_LAST", 0.5,timer,SENTENCE1_TIME)
	log ("Any message 3")
	
	any:changeAnimation("die02")
	timedSay(any,"CUTSCENE_HELLOWORLD_DYING",0.5,timer,0.5)
	animationWait(any,"die02")

	
	--stage 3: PC MOVEMENT:
	-- any:walkToXYZ(), any:runToXYZ() -> make Any move to a given position
	-- any:walkToObject("poster#0",25), any:runToObject() -> make Any move to a short distance of a given gameObject
	-- any:walkPath("pathname"), any:runPath("pathname") -> make Any follow a predefined trajectory
	--while not any:hasArrivedToObject("poster#0") do
	--	coroutine.yield(0)
	--end
	any:changeAnimation("idle01")
	return COROUTINE_FINISHED
end
