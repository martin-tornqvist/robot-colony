didFirstMove = false

function display_info()
  x, y = get_rbt_pos()
  t = get_tick_nr()
  power = get_rbt_power()
  power_max = get_rbt_power_max()
  return
    "Pos: " .. x .. ", " .. y ..
    "   Power: " .. power .. "/" .. power_max ..
    "   Tick: " .. t
end

recharging = false

function act()
  build(50, 20)
  
  power         = get_rbt_power()
  power_percent = get_rbt_power_percent();
  
  recharge_x, recharge_y = get_pos_of_nearest_assembly();
  
  if(power_percent <= 50) then
    move_to(recharge_x, recharge_y)
    wait()
    recharging = true
  end
  
  if power_percent == 100 then
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