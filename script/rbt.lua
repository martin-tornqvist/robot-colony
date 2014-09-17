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

  repeat build("", x + 1, y) until
    is_assembly_done_at("", x + 1, y)

  repeat step_towards(x + 1, y) until
    is_rbt_at(x + 1, y)
   
  repeat wait() until
    get_rbt_energy_percent() == 100
   
  repeat step_towards(1, 1) until
    get_rbt_energy_percent() < 60
   
  repeat step_towards(x + 1, y) until
    is_rbt_at(x + 1, y)
   
  repeat wait() until
    get_rbt_energy_percent() == 100
end