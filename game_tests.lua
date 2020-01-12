load_scenario_script('test0')

game_units:assign_movement_cmp(0, MovementComponent.new(10))
game_units:assign_movement_cmp(1, MovementComponent.new(15))
game_units:assign_movement_cmp(2, MovementComponent.new(10))

game_units:assign_direct_fight_cmp(0, DirectFightComponent.new(2))
game_units:assign_direct_fight_cmp(1, DirectFightComponent.new(3))
game_units:assign_direct_fight_cmp(2, DirectFightComponent.new(3))

game_units:remove_position_cmp(0)
game_units:assign_position_cmp(0, PositionComponent.new(Coord.new(-2, 0), 4))
--game_units:assign_position_cmp(0, PositionComponent.new(Coord.new(-2, 4), 4))


game_units:remove_position_cmp(2)
game_units:assign_position_cmp(2, PositionComponent.new(Coord.new(-3, 1), 2))


set_local_player_index(0)
system:start()

next_phase_action()
next_phase_action()