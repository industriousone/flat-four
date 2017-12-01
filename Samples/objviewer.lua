-- objviewer.lua
--
-- A simple object viewer with trackball style view manipulation.
-- To manipulate the image, click and drag with the mouse.
--

-----------------------------------------------------------------------------

local App = Service(Core.EventListener)

function App:initialize()

	system:setWindowTitle("F4/Object Viewer")

	-- Load the object to view
	local shader = Render.Shader()
	shader.texture = io:fromFile("Data/moon.png")
	if (not shader.texture) then error("unable to load texture") end
	local geom = StdLib.Cube(80, 10, 40)
	self.visual = Render.Visual(geom, shader)
	if (not self.visual) then error("unable to load object") end


	-- Find the center and radius of the object's bounding sphere
	local min = Vector()
	local max = Vector()
	self.visual.geom:getExtents(min, max)

	local center = (min + max) * 0.5
	min = min - center
	max = max - center

	if (-min.x > max.x) then max.x = -min.x end
	if (-min.y > max.y) then max.y = -min.y end
	if (-min.z > max.z) then max.z = -min.z end

	-- Create a view matrix to push the camera back
	self.camera = Matrix()
	self.camera:setPosition(0, 0, -max.length * 2)

	-- Create a world matrix for the object
	self.matrix = Matrix()
	self.matrix:setPosition(-center)

	self.screenRadius = 1

end

function App:onTimeEvent(event)

	render:start()
	render:setProjection3D(60, system:getDisplayWidth()/system:getDisplayHeight(), 0.1, 1000)

	render:setViewMatrix(self.camera)
	render:setWorldMatrix(self.matrix)
	
	render:queue(self.visual)

	render:finish()
	system:swapBuffers()

end

function App:onKeyEvent(event)

	if (event.code == event.ESCAPE) then
		core:quit()
	end

end

function App:onClickEvent(event)

	if (event.pressed) then
		self.lastPos = self:projectToSphere(event.x, event.y)
	end

end

function App:onMotionEvent(event)

	if (event:getButtonState(0)) then
	
		local pos = self:projectToSphere(event.x, event.y)

		local r = self:calcRotation(self.lastPos, pos)
		self.matrix = self.matrix * r

		self.lastPos = pos

	end

end

function App:projectToSphere(x, y)

	local dx = system:getDisplayWidth()
	local dy = system:getDisplayHeight()

	x = (2 * x - dx) / dx
	y = (dy - 2 * y) / dy

	local radius2 = self.screenRadius * self.screenRadius
	local dist2 = x*x + y*y

	local z	
	if 	(dist2 < radius2 * 0.5) then
		z = sqrt(radius2 - dist2)
	else
		z = radius2 / (2 * sqrt(dist2))
	end

	return Vector(x, y, z)

end

function App:calcRotation(lastPos, currPos)

	local axis = currPos:cross(lastPos)
	axis:normalize()

	local delta = lastPos - currPos
	local t = delta.length / (2 * self.screenRadius)
	if (t < -1.0) then t = -1.0 end
	if (t >  1.0) then t =  1.0 end
	local angle = 2 * asin(t)

	local m = Matrix()
	m:setRotation(-angle, axis)
	return m

end



-------------------------------------------------------------------------
-- Starts here
-------------------------------------------------------------------------

io = IO.IOService()
render = Render.RenderService()
system = System.SystemService()

StdIO.Register()

io:mountFilesystem("Data", "Data")
core:addEventListener( App() )


