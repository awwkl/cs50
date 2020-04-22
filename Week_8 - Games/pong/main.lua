-- First to 10 points wins.

-- https://github.com/Ulydev/push
push = require 'push' -- draw game at virtual resolution; provide retro aesthetic

-- https://github.com/vrld/hump/blob/master/class.lua
Class = require 'class'

require 'Paddle'
require 'Ball'

-- size of our actual window
WINDOW_WIDTH = 1280
WINDOW_HEIGHT = 720

-- size we're trying to emulate with push library
VIRTUAL_WIDTH = 432
VIRTUAL_HEIGHT = 243

-- paddle movement speed; pixels
PADDLE_SPEED = 200

-- called once at beginning of game
function love.load()
		love.window.setTitle('Pong')
		love.graphics.setDefaultFilter('nearest', 'nearest')

		-- seed the RNG with the current time so it varies between games
		math.randomseed(os.time())

		-- create font objects, then set font
		smallFont = love.graphics.newFont('font.ttf', 8)
		largeFont = love.graphics.newFont('font.ttf', 16)
		scoreFont = love.graphics.newFont('font.ttf', 32)
		love.graphics.setFont(smallFont)
		
		-- push setup; initialize virtual resolution
		push:setupScreen(VIRTUAL_WIDTH, VIRTUAL_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT, {
				vsync = true,
				fullscreen = false,
				resizable = true
		})

		-- set up our sound effects into a table
		sounds = {
				['paddle_hit'] = love.audio.newSource('sounds/paddle_hit.wav', 'static'),
				['score'] = love.audio.newSource('sounds/score.wav', 'static'),
				['wall_hit'] = love.audio.newSource('sounds/wall_hit.wav', 'static')
		}

		player1 = Paddle(10, 30, 5, 20)
		player2 = Paddle(VIRTUAL_WIDTH - 10, VIRTUAL_HEIGHT - 30, 5, 20)

		-- place a ball in the middle of the screen
		ball = Ball(VIRTUAL_WIDTH / 2 - 2, VIRTUAL_HEIGHT / 2 - 2, 4, 4)

		player1Score = 0
		player2Score = 0

		-- either going to be 1 or 2, to indicate serving player
		servingPlayer = 1

		winningPlayer = 0

		-- 1. 'start' (before first serve)
		-- 2. 'serve' (waiting on a key press to serve the ball)
		-- 3. 'play' (the ball is in play)
		-- 4. 'done' (the game is over, with a victor, ready for restart)
		gameState = 'start'

		love.keyboard.keysPressed = {}
end

-- uses push:resize to handle the resizing
function love.resize(w, h)
		push:resize(w, h)
end

-- called every frame, passing in dt (deltaTime - time passed since last frame)
-- Multiplying this by any changes we wish to make in our game will allow our game to perform consistently across all hardware

function love.update(dt)
		if gameState == 'start' then
				-- pressing enter will begin the game
				if love.keyboard.wasPressed('enter') or love.keyboard.wasPressed('return') then
						gameState = 'serve'
				end
		elseif gameState == 'serve' then
				-- pressing enter will serve the ball
				if love.keyboard.wasPressed('enter') or love.keyboard.wasPressed('return') then
						gameState = 'play'

						-- before switching to play, initialize ball's velocity based
						-- on player who last scored
						ball.dy = math.random(-50, 50)
						if servingPlayer == 1 then
								ball.dx = math.random(140, 200)
						else
								ball.dx = -math.random(140, 200)
						end
				end
		elseif gameState == 'play' then
				-- detect ball collision with paddles
				-- reverse dx and slightly increase it
				if ball:collides(player1) then
						ball.dx = -ball.dx * 1.03
						ball.x = player1.x + 5

						-- if the ball hits the paddle above its midpoint, ball should go upwards
						-- else ball should go downwards
						if ball.y - ball.height < player1.y + player1.height / 2 then
								ball.dy = -math.random(50, 100) * (player1.y + 
										(player1.height / 2)) / ball.y
						else
								ball.dy = math.random(50, 100) * 
										(player1.y + player1.height) / ball.y
						end

						sounds['paddle_hit']:play()		-- play sound
				end
				if ball:collides(player2) then
						ball.dx = -ball.dx * 1.03
						ball.x = player2.x - 4

						if ball.y - ball.height < player2.y + player2.height / 2 then
								ball.dy = -math.random(50, 100) * (player2.y + 
										(player2.height / 2)) / ball.y
						else
								ball.dy = math.random(50, 100) * 
										(player2.y + player2.height) / ball.y
						end

						sounds['paddle_hit']:play()
				end

				-- detect upper and lower screen boundary collision
				if ball.y <= 0 then
						ball.y = 0
						ball.dy = -ball.dy
						sounds['wall_hit']:play()
				end

				if ball.y >= VIRTUAL_HEIGHT - 4 then
						ball.y = VIRTUAL_HEIGHT - 4
						ball.dy = -ball.dy
						sounds['wall_hit']:play()
				end

				-- if we reach the left or right edge of the screen, go back to serve
				-- and update the score and serving player
				if ball.x < 0 then
						servingPlayer = 1
						player2Score = player2Score + 1
						sounds['score']:play()

						-- if score reaches 10, set state to 'done'
						-- else set state to 'serve' and reset ball to center of screen
						if player2Score == 10 then
								winningPlayer = 2
								gameState = 'done'
						else
								gameState = 'serve'
								ball:reset()
						end
				end

				if ball.x > VIRTUAL_WIDTH then
						servingPlayer = 2
						player1Score = player1Score + 1
						sounds['score']:play()

						if player1Score == 10 then
								winningPlayer = 1
								gameState = 'done'
						else
								gameState = 'serve'
								ball:reset()
						end
				end

		elseif gameState == 'done' then
				-- game is simply in a restart phase here, but will set the serving
				-- player to the opponent of whomever won for fairness!
				if love.keyboard.wasPressed('enter') or love.keyboard.wasPressed('return') then
						gameState = 'serve'

						ball:reset()

						-- reset scores to 0
						player1Score = 0
						player2Score = 0

						-- decide serving player as the opposite of who won
						if winningPlayer == 1 then
								servingPlayer = 2
						else
								servingPlayer = 1
						end
				end
		end

		-- paddles can move no matter what state we're in
		if love.keyboard.isDown('w') then
				player1.dy = -PADDLE_SPEED
		elseif love.keyboard.isDown('s') then
				player1.dy = PADDLE_SPEED
		else
				player1.dy = 0
		end

		-- player 2 (if player 2 is a player; see below for player 2 as AI)
		-- if love.keyboard.isDown('up') then
		-- 		player2.dy = -PADDLE_SPEED
		-- elseif love.keyboard.isDown('down') then
		-- 		player2.dy = PADDLE_SPEED
		-- else
		-- 		player2.dy = 0
		-- end

		-- player2 as AI. paddle.y will simply track ball.y
		player2_new_y = ball.y - player2.height / 2 + ball.height / 2 		-- makes MIDDLE of paddle follow middle of ball
		player2_new_y = math.max(0, player2_new_y) 		-- ensures that paddle does not go above screen
		player2_new_y = math.min(VIRTUAL_HEIGHT - player2.height, player2_new_y)		-- ensures that paddle does not go below screen
		
		player2.y = player2_new_y

		player1:update(dt)
		-- player2:update(dt)

		-- update ball's position
		ball:update(dt)

		-- clear the table for keys pressed, as the frame has ended
		love.keyboard.keysPressed = {}
end

-- Does not account for keys that are held down, which is handled by a separate function (`love.keyboard.isDown`)
-- Useful for when we want things to happen right away, just once, like when we want to quit.
function love.keypressed(key)
		if key == 'escape' then
				love.event.quit()
		end 

		-- add to our table of keys pressed this frame
		love.keyboard.keysPressed[key] = true
end

--[[
		A custom function that will let us test for individual keystrokes outside
		of the default `love.keypressed` callback, since we can't call that logic
		elsewhere by default.
]]
function love.keyboard.wasPressed(key)
		if love.keyboard.keysPressed[key] then
				return true
		else
				return false
		end
end

-- called each frame after update
-- draws objects to screen
function love.draw()
		-- begin drawing with push, in our virtual resolution
		push:apply('start')

		love.graphics.clear(40/255, 45/255, 52/255, 255/255)
		
		-- render different things depending on which part of the game we're in
		if gameState == 'start' then
				-- UI messages
				love.graphics.setFont(smallFont)
				love.graphics.printf('Welcome to Pong!', 0, 10, VIRTUAL_WIDTH, 'center')
				love.graphics.printf('Press Enter to begin!', 0, 20, VIRTUAL_WIDTH, 'center')
		elseif gameState == 'serve' then
				-- UI messages
				love.graphics.setFont(smallFont)
				love.graphics.printf('Player ' .. tostring(servingPlayer) .. "'s serve!", 
						0, 10, VIRTUAL_WIDTH, 'center')
				love.graphics.printf('Press Enter to serve!', 0, 20, VIRTUAL_WIDTH, 'center')
		elseif gameState == 'play' then
				-- no UI messages to display in play
		elseif gameState == 'done' then
				-- UI messages
				love.graphics.setFont(largeFont)
				love.graphics.printf('Player ' .. tostring(winningPlayer) .. ' wins!',
						0, 10, VIRTUAL_WIDTH, 'center')
				love.graphics.setFont(smallFont)
				love.graphics.printf('Press Enter to restart!', 0, 30, VIRTUAL_WIDTH, 'center')
		end

		-- show the score before ball is rendered so it can move over the text
		displayScore()
		
		player1:render()
		player2:render()
		ball:render()

		-- display FPS for debugging; simply comment out to remove
		displayFPS()

		-- end our drawing to push
		push:apply('end')
end

function displayScore()
		love.graphics.setFont(scoreFont)
		love.graphics.print(tostring(player1Score), VIRTUAL_WIDTH / 2 - 50,
				VIRTUAL_HEIGHT / 3)
		love.graphics.print(tostring(player2Score), VIRTUAL_WIDTH / 2 + 30,
				VIRTUAL_HEIGHT / 3)
end

function displayFPS()
		-- simple FPS display across all states
		love.graphics.setFont(smallFont)
		love.graphics.setColor(0, 255, 0, 255)
		love.graphics.print('FPS: ' .. tostring(love.timer.getFPS()), 10, 10)
end