-- fontfactory.lua
--
-- Demonstrates how to use FontFactory to create a custom texture font
-- and OutputStream to save it to a file. You'll need to change the 
-- filename in the call to setTypeface() to refer to a valid TrueType 
-- or Type1 font file.
--

io = IO.IOService()
StdIO.Register()      -- contains the 4FNT ReaderWriter


local ff  = Text.FontFactory()
if (not ff) then error("unable to start FontFactory") end

io:mountFilesystem("fonts", "C:/Windows/Fonts")
local ok = ff:setTypeface("fonts/arial.ttf")
if (not ok) then error("unable to load source font") end

ff:setPixelSize(28)


-- Generate and save the font

local txf = ff:createTextureFont()
if (not txf) then error("unable to generate font") end

ok = io:toFile(txf, "Data/arial-28px.4fnt")
if (not ok) then error("unable to write file") end
