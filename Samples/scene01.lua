-- scene01.lua
-- Creates a very simple scene: a single, static box.

-------------------------------------------------------------------------
-- Define my application service
-------------------------------------------------------------------------

local App = Service(Core.EventListener)

function App:initialize()

	system:setWindowTitle("F4 Scene Manager Tutorial")

	-- Start by creating a visual to represent my entity. For this
	-- example I'll just use a simple textured cube
	local cube   = StdLib.Cube(1, 1, 1)

	local shader = Render.Shader()
	shader.texture = io:fromFile("Data/moon.png")

	local visual = Render.Visual(cube, shader)


	-- A Pose provides a position and orientation
	local pose = Scene.Pose()
	pose:setEulerRotation(45, 0, 0)


	-- Putting it into a Body allows me to attach it to the entity
	local body = Scene.Body()
	body:addVisual(visual)


	-- Finally, assemble the entity. Note that the Pose must appear before any
	-- attributes that it is to effect, so it usually should be first. Since
	-- the camera is after the pose, it will follow the entity around.
	self.entity = Scene.Entity("MyEntity")
	self.entity:addAttribute(pose)
	self.entity:addAttribute(body)


	-- A camera provides a viewpoint into the scene
	local camera = Scene.PerspectiveCamera()
	camera:setPosition(0, 0, 3)


	-- Create visitors for traversing the entity tree
	self.updatePass = Scene.UpdateVisitor()
	self.renderPass = Scene.RenderVisitor()


	-- Specify the active camera for rendering
	self.renderPass:setCamera(camera)

end

function App:onTimeEvent(event)

	self.updatePass:update(self.entity, event.elapsed)
	self.renderPass:render(self.entity)

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

StdIO.Register()

io:mountFilesystem("Data", "Data")
core:addEventListener( App() )



