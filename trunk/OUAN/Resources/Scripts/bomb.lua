BOMB_STATE_OFF=0
BOMB_STATE_IDLE=1
BOMB_STATE_ACTIVATE=2
BOMB_STATE_EXPLOSION=3
BOMB_STATE_FOLLOW=4
BOMB_STATE_PUZZLE_START=5

BOMB_STATE_NAMES= {}
BOMB_STATE_NAMES[BOMB_STATE_OFF]="OFF"
BOMB_STATE_NAMES[BOMB_STATE_IDLE]="IDLE"
BOMB_STATE_NAMES[BOMB_STATE_ACTIVATE]="ACTIVATE"
BOMB_STATE_NAMES[BOMB_STATE_EXPLOSION]="EXPLOSION"
BOMB_STATE_NAMES[BOMB_STATE_FOLLOW]="FOLLOW"
BOMB_STATE_NAMES[BOMB_STATE_PUZZLE_START]="PUZZLE_START"


BOMB_EXPLOSION_TIME=1;
BOMB_ACTIVATION_TIME=1;

function bombLogic(pBomb,state)
	-- local vars defined for legibility and to invoke as few c++ calls as possible
	local myName=pBomb:getName()
	local playerDistance=getPlayerDistance(myName)
	local activated=pBomb:hasTakenHit()	
	local time=pBomb:getTimeSpent()	
	local newState=state
	
	if state==BOMB_STATE_OFF then
	elseif state==BOMB_STATE_IDLE and activated then
		log("BOMB transitioning to activate")
		newState=BOMB_STATE_ACTIVATE
	elseif state==BOMB_STATE_PUZZLE_START and activated then
		log("BOMB transitioning to idle")
		newState=BOMB_STATE_IDLE
	elseif state==BOMB_STATE_IDLE and playerDistance > 20 then
		log("BOMB transitioning to follow")
		newState=BOMB_STATE_FOLLOW
	elseif state==BOMB_STATE_FOLLOW and playerDistance <= 20 then
		log("BOMB transitioning to idle")
		newState=BOMB_STATE_IDLE
	elseif state==BOMB_STATE_ACTIVATE and time > BOMB_ACTIVATION_TIME then
		log("BOMB transitioning to explosion")	
		newState=BOMB_STATE_EXPLOSION
	elseif state==BOMB_STATE_EXPLOSION and time > BOMB_EXPLOSION_TIME then
		log("BOMB transitioning to puzzle start")	
		newState=BOMB_STATE_PUZZLE_START
	end
	return newState
end