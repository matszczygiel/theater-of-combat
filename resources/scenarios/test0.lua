
function init()
    print('Initializing scenario')
    load_map_json('test')
    load_units_json('test1')
    
    game_scenario.teams['team 0'] = {0}
    game_scenario.teams['team 1'] = {1}

    game_scenario.player_teams[1] = {'team 0'}
    game_scenario.player_teams[2] = {'team 1'}

end

function day(day_no)
    if day_no == 1 then
        print('day 1')
    elseif day_no == 2 then
        print('day 2')
    end
end

graphics_config = {
    units = {
        file = 'units.png',
        token_size = 90,
        specific_tokens = {
            [0] = {x = 1, y = 0},
            [1] = {x = 1, y = 1}
        }
    },
    map = {
        tiles_file = 'tiles.png',
        tile_size = 256,
        hex_tiles = {
            [HexType.field] = {x = 0, y = 0},
            [HexType.forest] = {x = 1, y = 0}
        }
    }
}

    

