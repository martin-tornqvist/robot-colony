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

function act()
  repeat step_towards(40, 10) until is_rbt_at(40, 10) 
  repeat step_towards(42, 10) until is_rbt_at(40, 10) 
end