didFirstMove = false

function displayInfo()
  x, y = getRbtPos()
  t = getTickNr()
  pwrPct = getRbtPwrPct()
  return "Pos: " .. x .. ", " .. y .. "   Pwr: " .. pwrPct .. "%   Tick: " .. t
end

recharging = false

function act()
  build(50, 20)
  
  pwr     = getRbtPwr()
  pwrPct  = getRbtPwrPct();
  
  if(pwrPct <= 50) then
    moveTo(50, 20)
    wait()
    recharging = true
  end
  
  if pwrPct == 100 then
    recharging = false
  end
  
  if recharging == false then
    build(40, 20)
    build(50, 10)
    build(30, 20)
    build(50, 2)
    build(20, 20)
    build(5, 5)
  end
  
end