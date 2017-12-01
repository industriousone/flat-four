-- NinjaFork!
-- (c)2002 by Kane Karsteter


local PLANE_SIZE = 100       -- object sizes
local DUDE_SIZE  = 100
local EXPL_SIZE  = 128
local PART_SIZE  = 8
local BOMB_SIZE  = 32
local FORK_SIZE  = 32
local LIFE_SIZE  = 32
local SCORE_SIZE = 32

local FORK_X     = -45       -- initial pos of fork relative to plane
local FORK_Y     = -40

local X_MAX  =  320          -- logical screen measurements
local Y_MAX  =  240
local SKY    =  200
local GROUND = -190

local MIN_SPEED   = 70       -- slowest plane speed
local MAX_SPEED   = 130      -- fastest plane speed
local FORK_SPEED  = 220      -- fork speed
local NUM_DUDES   = 3        -- starting number of dudes
local DUDE_SLOW   = 40       -- speed of slowest dude
local DUDE_FAST   = 100      -- speed of fastest dude
local DUDE_EARLY  = 2        -- min time before new dude appears
local DUDE_LATE   = 5        -- max time before new dude appears
local BOMB_SLOW   = 40       -- speed of slowest bomb
local BOMB_FAST   = 60       -- speed of fastest bomb
local PROB_FLIP   = 0.003    -- probability a dude will change direction
local PROB_THROW  = 0.005    -- probability a dude will throw a bomb
local RESPAWN     = 4        -- time before new plane appears
local FADE_OUT    = 2        -- time to fade out explosions
local NUM_LIVES   = 3        -- starting number of lives
local NUM_PARTS   = 20       -- particles from explosion
local PART_SPEED  = 140      -- maximum particle speed
local GRAVITY     = 20       -- gravity force acting on particles

local FORK_POINTS = -1       -- points for each fork fired
local DUDE_POINTS = 50       -- points for blowing up a dude


local NewPlane = Service(Core.Event, "NewPlane")  -- custom game events
local NewDude  = Service(Core.Event, "NewDude")


local App = Service(Core.EventListener)



-------------------------------------------------------------------------
-- Support functions: manage the game entities
-------------------------------------------------------------------------

function App:createVisual(name, texture, size)

	local shr = Render.Shader()
	shr:enableAlphaBlend()
	shr.texture = texture

	local spr = Render.Sprite(size, size)

	self.visuals[name] = Render.Visual(spr, shr)

end

function App:loadVisuals()

	self.visuals = { }

	local tex = io:fromFile("Data/plane.png")
	self:createVisual("plane", tex, PLANE_SIZE)

	self:createVisual("life", tex, LIFE_SIZE)
		
	tex = io:fromFile("Data/dude.png")
	self:createVisual("dude", tex, DUDE_SIZE)
		
	tex = io:fromFile("Data/bomb.png")
	self:createVisual("bomb", tex, BOMB_SIZE)

	tex = io:fromFile("Data/fork.png")
	self:createVisual("fork", tex, FORK_SIZE)

	self:createVisual("flippedFork", tex, FORK_SIZE)
	self.visuals["flippedFork"].geom:flipX()

	tex = io:fromFile("Data/explosion.png")
	self:createVisual("explosion", tex, EXPL_SIZE)

	self:createVisual("particle", tex, PART_SIZE)

end

function App:createEntity(entityType)
	
	local e = { }
	e.visual = self.visuals[entityType]
	e.matrix = Matrix()
	e.shape  = Collider.BoundingSphere(e.visual.geom.width / 2)
	e.update = App[entityType]
	e.alive  = true

	if (not self.entities[entityType]) then
		self.entities[entityType] = { }
	end
	tinsert(self.entities[entityType], e)

	return e

end

function App:update(event)

	for k,v in self.entities do
		for i = 1, getn(v) do
			if (v[i].update) then
				v[i].update(self, v[i], event)
				v[i].shape:setMatrix(v[i].matrix)
			end
		end
	end

	-- remove dead entities
	for k,v in self.entities do
		local alive = { }
		for i = 1, getn(v) do
			if (v[i].alive) then
				tinsert(alive, v[i])
			end
		end
		self.entities[k] = alive
	end

	-- look for collisions between bombs and plane
	local player = self.entities["plane"][1]
	local bombs  = self.entities["bomb"]
	if (player and bombs) then
		for i = 1, getn(bombs) do
			if (collider:test(player.shape, bombs[i].shape, nil) > 0) then
				self:killPlane(player, bombs[i])
			end
		end
	end

	-- look for collisions between forks and dudes
	local forks = self.entities["fork"]
	local dudes = self.entities["dude"]
	if (forks and dudes) then
		for i = 1, getn(dudes) do
			for j = 1, getn(forks) do
				if (collider:test(dudes[i].shape, forks[j].shape, nil) > 0) then
					self:killDude(dudes[i], forks[j])
					break
				end
			end
		end
	end

end

function App:draw()

	render:setBackgroundColor(0.5, 0.7, 1)

	if (self.gameOver) then
		render:queue(self.gameOver)
	end

	for k,v in self.entities do
		for i = 1, getn(v) do
			render:setWorldMatrix(v[i].matrix)
			render:queue(v[i].visual)
		end
	end

	render:setWorldMatrix(self.scoreboard.matrix)
	render:queue(self.scoreboard)

end





-------------------------------------------------------------------------
-- Game functions: handle changes in the game state
-------------------------------------------------------------------------

function App:mapInputs()

	-- Map action names to functions
	self.actions = 
	{ 
		["Left"]  = App.goLeft,
		["Right"] = App.goRight,
		["Up"]    = App.goUp,
		["Down"]  = App.goDown,
		["Fire"]  = App.fireWeapon,
	}

	-- Create the logical input objects for my controls
	self.mapper:createAxis("Left", 0.2, 0.2)
	self.mapper:createAxis("Right", 0.2, 0.2)
	self.mapper:createAxis("Up", 0.2, 0.2)
	self.mapper:createAxis("Down", 0.2, 0.2)
	self.mapper:createTrigger("Fire", 0.5, 0.3)

	-- Bind the logical objects to physical input objects
	self.mapper:bind("left",  "Left")
	self.mapper:bind("right", "Right")
	self.mapper:bind("up",    "Up")
	self.mapper:bind("down",  "Down")
	self.mapper:bind("space", "Fire")

	self.mapper:bind("mouse.btn0", "Fire")

	self.mapper:bind("ctrl0.-axis0", "Left")
	self.mapper:bind("ctrl0.+axis0", "Right")
	self.mapper:bind("ctrl0.+axis1", "Up")
	self.mapper:bind("ctrl0.-axis1", "Down")
	self.mapper:bind("ctrl0.btn0",   "Fire")

end

function App:start()

	self.leftRight  = -MIN_SPEED
	self.upDown     = 0
	self.dudeSlow   = DUDE_SLOW
	self.dudeFast   = DUDE_FAST
	self.bombSlow   = BOMB_SLOW
	self.bombFast   = BOMB_FAST
	self.probFlip   = PROB_FLIP
	self.probThrow  = PROB_THROW
	self.canThrow   = true
	self.livesLeft  = NUM_LIVES
	self.gameOver   = nil
	self.score      = 0
	self.scoreboard.text = 0

	self.entities = { }
	self:createEntity("plane")
	for i = 1, NUM_DUDES do
		self:createEntity("dude")
	end

	for i = NUM_LIVES, 1, -1 do
		local life = self:createEntity("life")
		life.matrix:setPosition(X_MAX - (LIFE_SIZE/2 + LIFE_SIZE * (i - 1)), Y_MAX - (LIFE_SIZE/2 + SCORE_SIZE), 0)
	end

	core:scheduleEvent(random(DUDE_EARLY, DUDE_LATE), self, NewDude())

end

function App:adjustScore(amount)

	self.score = self.score + amount
	self.scoreboard.text = self.score

end

function App:goLeft(speed)
	self.leftRight = -(speed * (MAX_SPEED-MIN_SPEED) + MIN_SPEED)
end

function App:goRight(speed)
	self.leftRight = speed * (MAX_SPEED-MIN_SPEED) + MIN_SPEED
end

function App:goUp(speed)
	self.upDown = speed * MAX_SPEED
end

function App:goDown(speed)
	self.upDown = -speed * MAX_SPEED
end

function App:fireWeapon()

	local plane = self.entities["plane"][1]
	if (not plane) then return end
	local fork  = self:createEntity("fork")

	local pos = Vector(FORK_X, FORK_Y)
	if (self.leftRight > 0) then pos.x = -pos.x end
	fork.matrix:setPosition(plane.matrix:getPosition() + pos)

	fork.velocity = Vector(self.leftRight/MAX_SPEED, -0.40, 0):normalize() * FORK_SPEED

	self:adjustScore(FORK_POINTS)

end

function App:createExplosion(pos)

	local x = self:createEntity("explosion")
	x.matrix:setPosition(pos)
	x.fadeOut = FADE_OUT

	for i = 1, NUM_PARTS do
		x = self:createEntity("particle")
		x.matrix:setPosition(pos)
		x.velocity = Vector(random(-PART_SPEED, PART_SPEED), random(-PART_SPEED, PART_SPEED), 0)
		x.fadeOut = random(FADE_OUT - 1, FADE_OUT)
	end

end

function App:killPlane(plane, bomb)

	self:createExplosion(plane.matrix:getPosition())

	plane.alive = nil
	bomb.alive  = nil
	
	self.canThrow = false

	self.livesLeft = self.livesLeft - 1
	self.entities["life"][1].alive = nil
	if (self.livesLeft > 0) then
		core:scheduleEvent(3, self, NewPlane())
	else
		self.gameOver = self.font:createVisual()
		self.gameOver:setAlignment(self.gameOver.CENTER, self.gameOver.CENTER)
		self.gameOver.text = "GAME OVER"
		self.gameOver.shader:setColor(1, 0, 0)
	end

end

function App:killDude(dude, fork)

	self:createExplosion(dude.matrix:getPosition())

	dude.alive = nil
	fork.alive = nil

	self:adjustScore(DUDE_POINTS)

end

function App:onNewPlane()

	for i = 1, getn(self.entities["bomb"]) do
		self.entities["bomb"][i].alive = nil
	end

	self:createEntity("plane")
	self.canThrow = true

end

function App:onNewDude(event)

	self:createEntity("dude")
	if (not self.gameOver) then
		core:scheduleEvent(random(DUDE_EARLY, DUDE_LATE), self, event)
	end

end



-------------------------------------------------------------------------
-- Controller functions: for each type of entity
-------------------------------------------------------------------------

function App:plane(entity, timeEvent)

	local flipped = entity.visual.geom:isXFlipped()
	if ((self.leftRight > 0 and not flipped) or (self.leftRight < 0 and flipped)) then
		entity.visual.geom:flipX()
	end

	local vel = Vector(self.leftRight, self.upDown, 0) 

	local pos = entity.matrix:getPosition() + (vel * timeEvent.elapsed)
	if (pos.x < -X_MAX)  then pos.x =  X_MAX end
	if (pos.x >  X_MAX)  then pos.x = -X_MAX end
	if (pos.y < GROUND)  then pos.y = GROUND end
	if (pos.y > SKY)     then pos.y = SKY    end
	entity.matrix:setPosition(pos)

end

function App:dude(entity, timeEvent)

	local pos = entity.matrix:getPosition()
	if (not entity.speed) then 
		pos.x = random(-X_MAX, X_MAX)
		pos.y = GROUND
		
		entity.speed = random(self.dudeSlow, self.dudeFast) 
		if (random() < 0.5) then 
			entity.speed = -entity.speed 
		end
	end

	if (random() < self.probFlip) then
		entity.speed = -entity.speed
	end

	pos.x = pos.x + (entity.speed * timeEvent.elapsed)
	if (pos.x < -X_MAX) then pos.x =  X_MAX end
	if (pos.x >  X_MAX) then pos.x = -X_MAX end

	entity.matrix:setPosition(pos)

	if (self.canThrow and random() < self.probThrow) then
		local b = self:createEntity("bomb")
		b.matrix:setPosition(pos)
	end

end

function App:bomb(entity, timeEvent)

	if (not entity.vel) then
		local speed = random(self.bombSlow, self.bombFast)
		entity.vel = Vector(random() - 0.5, 1, 0):normalize() * speed
	end

	local pos = entity.matrix:getPosition() + (entity.vel * timeEvent.elapsed)
	if (pos.x < -X_MAX) then pos.x =  X_MAX end
	if (pos.x >  X_MAX) then pos.x = -X_MAX end
	if (pos.y >  Y_MAX) then entity.alive = nil end
	entity.matrix:setPosition(pos)

end

function App:fork(entity, timeEvent)

	if (entity.velocity.x > 0) then
		entity.visual = self.visuals["flippedFork"]
	end

	local pos = entity.matrix:getPosition() + (entity.velocity * timeEvent.elapsed)
	if (pos.x < -X_MAX) then pos.x =  X_MAX end
	if (pos.x >  X_MAX) then pos.x = -X_MAX end
	if (pos.y < -Y_MAX) then entity.alive = nil end
	entity.matrix:setPosition(pos)

end

function App:explosion(entity, timeEvent)

	entity.fadeOut = entity.fadeOut - timeEvent.elapsed
	if (entity.fadeOut <= 0) then
		entity.alive = nil
	end
	entity.visual.shader:setAlpha(entity.fadeOut / FADE_OUT)

end

function App:particle(entity, timeEvent)

	entity.fadeOut = entity.fadeOut - timeEvent.elapsed
	if (entity.fadeOut <= 0) then
		entity.alive = nil
	end
	entity.visual.shader:setAlpha(entity.fadeOut / FADE_OUT)

	entity.matrix:setPosition(entity.matrix:getPosition() + (entity.velocity * timeEvent.elapsed))
	entity.velocity.y = entity.velocity.y - (GRAVITY * timeEvent.elapsed)

end



-------------------------------------------------------------------------
-- System interface: these functions are called by F4 
-------------------------------------------------------------------------

function App:initialize()

	system:setWindowTitle("Attack of the Ninja Forks!")

	self.font = io:fromFile("Data/arial-28px.4fnt")
	if (not self.font) then error("Unable to load font!") end

	self.mapper = StdLib.ActionMapper()
	self:mapInputs()

	self.scoreboard = self.font:createVisual()
	self.scoreboard:setAlignment(self.scoreboard.RIGHT, self.scoreboard.TOP)
	self.scoreboard.shader:setColor(0, 0, 0)
	self.scoreboard.matrix = Matrix()
	self.scoreboard.matrix:setPosition(X_MAX, Y_MAX + 3, 0)

	self:loadVisuals()
	self:start()
			
end

function App:onAction(event)

	self.actions[event.name](self, event.value)

end

function App:onKeyEvent(event)

	if (event.code == event.ESCAPE) then
		core:quit()
	end

	if (self.gameOver) then
		self:start()
	end

end
	
function App:onTimeEvent(event)

	self:update(event)

	render:start()
	render:setProjection2D(-X_MAX, Y_MAX, X_MAX, -Y_MAX)
	self:draw()
	render:finish()
	system:swapBuffers()

	return true

end



-------------------------------------------------------------------------
-- Game startup 
-------------------------------------------------------------------------

io       = IO.IOService()         -- start important subsystems
system   = System.SystemService()
render   = Render.RenderService()
collider = Collider.CollisionService()

StdIO.Register()            -- load support for common file formats

io:mountFilesystem("Data", "Data")
core:addEventListener( App() )
