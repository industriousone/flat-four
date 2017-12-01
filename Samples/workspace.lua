-- workspace.lua
-- I try stuff here.

-------------------------------------------------------------------------
-- Define my application service
-------------------------------------------------------------------------

local App = Service(Core.EventListener)

-------------------------------------------------------------------------

function App:initialize()

	system:setWindowTitle("F4/Workspace")
	physics:setGravity(0, 0, 0)

	-- Make a box
	local geom = StdLib.Cube(1, 0.25, 0.5)
	local shader = Render.Shader()
	shader.texture = io:fromFile("Data/moon.png")
	local body = Scene.Body()
	body:addVisual(Render.Visual(geom, shader))

	-- Scene root
	self.root  = Scene.Entity("root")
	
	local space = Scene.SimpleSpace()
	self.root:addAttribute(space)

	-- Camera
	self.camera = Scene.PerspectiveCamera()
	self.camera:setPosition(0, 0, 4)

	-- Traversals
	self.uv = Scene.UpdateVisitor()
	self.rv = Scene.RenderVisitor()

	self.rv:setCamera(self.camera)

	-- First box
	local box = Scene.Entity("box1")
	space:addEntity(box)

	self.pose = Scene.Pose()
	self.pose:setPosition(-1.5, 0, 0)

	box:addAttribute(self.pose)
	box:addAttribute(body)

	-- Second box
	local box = Scene.Entity("box2")
	space:addEntity(box)

	self.physics = Scene.Physics()
	self.physics:setPosition(1.5, 0, 0)

	box:addAttribute(self.physics)
	box:addAttribute(body)

	self.angle = 0

end

function App:onTimeEvent(event)

	self.pose:setEulerRotation(self.angle, 0, 0)
	self.physics:setEulerRotation(self.angle, 0, 0)

	self.angle = self.angle + 0.1

	self.uv:update(self.root, event.elapsed)
	self.rv:render(self.root)

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



