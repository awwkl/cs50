function love.draw()
  love.graphics.print("Hello World!")
end

love.load()
love.update(dt)
love.draw()
love.graphics.printf(text, x, y, [width], [align])
love.window.setMode(width, height, params)