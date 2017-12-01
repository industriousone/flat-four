-- scene02.lua
-- Demonstrates the use of rigid body physics and collision detection.

-------------------------------------------------------------------------
-- Define my application service
-------------------------------------------------------------------------

local App = Service(Core.EventListener)

local createGround = function()

	local surface = Scene.Surface()
	surface:addBoundingShape(Collider.BoundingPlane(0, 1, 0, 0))

	local entity = Scene.Entity("ground")
	entity:addAttribute(surface)

	return entity

end

local createBox = function(x, y, z)

	-- Create and cache objects that are shared between all of the boxes
	if (sharedVisual == nil) then
		
		local cube = StdLib.Cube(1, 1, 1)
		local shdr = Render.Shader()
		shdr.texture = io:fromFile("Data/moon.png")
		sharedVisual = Render.Visual(cube, shdr)

		sharedPhysics = Physics.ContactPhysics()
		sharedPhysics.friction = sharedPhysics.INFINITY
		sharedPhysics.restitution = 0.5

	end

	local pose = Scene.Physics()
	pose:setPosition(x, y, z)

	local body = Scene.Body()
	body:addVisual(sharedVisual)

	local surface = Scene.Surface()
	surface:addBoundingShape(Collider.BoundingBox(1, 1, 1))
	surface:setContactPhysics(sharedPhysics)

	local entity = Scene.Entity("box")
	entity:addAttribute(pose)
	entity:addAttribute(body)
	entity:addAttribute(surface)

	return entity

end

function App:initialize()

	system:setWindowTitle("F4 Scene Manager Tutorial")
	physics:setGravity(0, -4, 0)

	self.updatePass  = Scene.UpdateVisitor()
	self.renderPass  = Scene.RenderVisitor()
	self.collidePass = Scene.CollisionVisitor()

	local camera = Scene.PerspectiveCamera()
	camera:setPosition(-2, 2, 7)
	self.renderPass:setCamera(camera)

	-- Set up a root entity for the scene. Add a space attribute in
	-- which to place all of the other scene objects.

	self.root = Scene.Entity("root")

	local space = Scene.SimpleSpace()
	self.root:addAttribute(space)

	-- Add a ground plane
	space:addEntity(createGround())

	-- Add the boxes
	for i = 1,3 do
		local box = createBox(-i * 0.6, i * 1.5, 0)
		space:addEntity(box)
	end

end

function App:onTimeEvent(event)

	self.updatePass:update(self.root, event.elapsed)
	self.renderPass:render(self.root)
	self.collidePass:collide(self.root)

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



