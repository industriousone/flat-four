-- boxes.lua
-- A physics/collision demo.

-----------------------------------------------------------------------------

local App = Service(Core.EventListener)

local createBox
function createBox(x, y, z, h, p, r)

	local box = { }

	box.shape = Collider.BoundingBox(1, 1, 1)
	box.rbody = Physics.RigidBody()
	box.rbody:setPosition(x, y, z)
	box.rbody:setEulerRotation(h, p, r)

	return box

end

function App:initialize()

	system:setWindowTitle("F4/Boxes")

	-- create the box model
	local geom = StdLib.Cube(1, 1, 1)
	local shdr = Render.Shader()
	shdr.texture = io:fromFile("Data/moon.png")
	self.box = Render.Visual(geom, shdr)

	-- create the boxes
	self.boxes = { createBox( 0.6, 0, 0, 0, 0, 0),
				   createBox( 0.8, 1.2, 0, 0, 0, 0),
				   createBox( 0.4, 2.4, 0, 0, 0, 0),
				   createBox( 0.3, 4.5, -0.1, 45, 45, 0)
				 }

	-- create a ground plane
	self.ground = Collider.BoundingPlane(0, 1, 0, -2.5)

	-- create a view into the scene
	self.camera = Matrix()
	self.camera:setPosition(0, 0, -5)

	-- give a little gravity
	physics:setGravity(0, -5, 0)

	-- describe the physical response of a collision
	self.response = Physics.ContactPhysics()
	self.response.friction = self.response.INFINITY
	self.response.restitution = 0.2

	-- create a contact list to store the results of collision testing
	self.contacts = Collider.ContactList()

	self.time = system:getTimer()

end

function App:onTimeEvent(event)

	-- The simulation gets (very) unstable if the timestep gets too big
	-- so I run the physics at a fixed timestep of 100Hz.
	local currTime = system:getTimer()
	while (self.time < currTime) do
		physics:advance(0.01)

		-- Test each box against the ground
		for i = 1, getn(self.boxes) do
			self.boxes[i].shape:setMatrix(self.boxes[i].rbody:getMatrix())
			if (collider:test(self.ground, self.boxes[i].shape, self.contacts) > 0) then
				physics:resolveCollision(nil, self.boxes[i].rbody, self.contacts, self.response)
			end
		end

		-- Test each box against the other boxes
		for i = 1, getn(self.boxes)-1 do
			for j = i+1, getn(self.boxes) do
				if (collider:test(self.boxes[i].shape, self.boxes[j].shape, self.contacts) > 0) then
					physics:resolveCollision(self.boxes[i].rbody, self.boxes[j].rbody, self.contacts, self.response)
				end
			end
		end

		self.time = self.time + 0.01
	end

	-- Render the new state of the simulation
	render:start()
	render:setProjection3D(60, system:getDisplayAspect(), 0.1, 1000)
	render:setViewMatrix(self.camera)

	-- A note here: setWorldMatrix() stores a reference to the given matrix,
	-- so it is important that it stays in memory until render:finish() is
	-- called. RigidBody:getMatrix() returns a *copy* of it's internal data,
	-- which I must cache locally to prevent the matrix from getting lost.

	for i = 1, getn(self.boxes) do
		self.boxes[i].matrix = self.boxes[i].rbody:getMatrix()
		render:setWorldMatrix(self.boxes[i].matrix)
		render:queue(self.box)
	end

	render:finish()
	system:swapBuffers()

end

function App:onKeyEvent(event)

	if (event.code == event.ESCAPE) then
		core:quit()
	end

end

-------------------------------------------------------------------------
-- Starts here
-------------------------------------------------------------------------

io       = IO.IOService()
render   = Render.RenderService()
system   = System.SystemService()
collider = Collider.CollisionService()
physics  = Physics.PhysicsService()

StdIO.Register()

io:mountFilesystem("Data", "Data")
core:addEventListener( App() )


