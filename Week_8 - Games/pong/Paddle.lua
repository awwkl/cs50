Paddle = Class{}

-- called just once, when the object is first created
-- x, y for positioning; width, height for size
-- self is a referecence to object being instantiated

function Paddle:init(x, y, width, height)
    self.x = x
    self.y = y
    self.width = width
    self.height = height
    self.dy = 0
end

function Paddle:update(dt)
    if self.dy < 0 then
        self.y = math.max(0, self.y + self.dy * dt)
    else
        self.y = math.min(VIRTUAL_HEIGHT - self.height, self.y + self.dy * dt)
    end
end

-- called in love.draw() in main.lua
function Paddle:render()
    love.graphics.rectangle('fill', self.x, self.y, self.width, self.height)
end