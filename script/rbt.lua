didFirstMove = false

function displayInfo()
  x, y = getRbtPos()
  return "Pos: " .. x .. ", " .. y
end

function act()
  x, y = getRbtPos()

  if didFirstMove == false then
    moveTo(70, 19)
    if x == 70 and y == 19 then didFirstMove = true  end
  else
    moveTo(1, 1)
  end
end