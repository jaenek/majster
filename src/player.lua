local width = 400
local height = 300

Config = {
	TOP = keyboard.w,
	RIGHT = keyboard.d,
	BOTTOM = keyboard.s,
	LEFT = keyboard.a,
}

Player = {
	pos = { x = 400 - 25, y = 300 - 25 },
	vel = { x = 0.0, y = 0.0 },
}

function Player:move(direction)
	if direction == Config.TOP then

	end
end
