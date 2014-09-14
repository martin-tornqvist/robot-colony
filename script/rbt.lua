function display_info()
  x, y = get_rbt_pos()
  t = get_tick_nr()
  energy = get_rbt_energy()
  energy_max = get_rbt_energy_max()
  return
    "Pos: " .. x .. ", " .. y ..
    "   Energy: " .. energy .. "/" .. energy_max ..
    "   Tick: " .. t
end

recharging = false

function act()
  build("", 50, 20)
  
  energy         = get_rbt_energy()
  energy_percent = get_rbt_energy_percent();
  
  recharge_x, recharge_y = get_nearest_recharge_station();
  
  if energy_percent <= 50 then
    move_to(recharge_x, recharge_y)
    wait()
    recharging = true
  end
  
  if energy_percent == 100 then
    recharging = false
  end
  
  if recharging == false then
    build("", 40, 20)
    build_road(50, 20, 40, 20)
    build("", 50, 10)
    build("", 30, 20)
    build("", 50, 2)
    build("", 20, 20)
    build("", 5, 5)
  end
  
end