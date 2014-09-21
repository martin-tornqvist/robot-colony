function display_info()
  local x, y = get_rbt_pos()
  local t = get_tick_nr()
  local energy = get_rbt_energy()
  local energy_max = get_rbt_energy_max()
  return
    "Pos: " .. x .. ", " .. y ..
    "   Energy: " .. energy .. "/" .. energy_max ..
    "   Tick: " .. t
end

function act()
  local x, y = get_rbt_pos()

  repeat build("", x - 1, y) until is_assembly_done_at("", x - 1, y)
  repeat go_towards(x - 1, y) until get_rbt_energy_percent() == 100
  
  
  repeat
    build("", 25, 10)
    if get_rbt_energy_percent() <= 50 then
      repeat go_towards(x + 1, y) until get_rbt_energy_percent() == 100
    end
  until is_assembly_done_at("", 25, 10)
  
  repeat go_towards(25, 10) until get_rbt_energy_percent() == 100
  
  repeat
    repeat go_towards(12, 10) until is_rbt_at(12, 10)
    repeat go_towards(25, 10) until get_rbt_energy_percent() == 100
  until false
    
end