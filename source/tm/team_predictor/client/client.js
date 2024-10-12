const socket = new WebSocket('ws://' + window.location.hostname + ':46924');

let data = null;
socket.onmessage = function(event) {
	const parsed = JSON.parse(event.data);

	if (parsed.generations) {
		data = parsed.generations;
		populate_generation();
		send_most_likely_data();
	}
	if (parsed.predicted) {
		display_predicted(parsed.predicted);
	}
};

const max_pokemon = 6;
const add_pokemon_button = document.getElementById('add-pokemon');

function get_team() {
	return document.getElementById('team');
}

function clear_team() {
	get_team().innerHTML = '';
	add_pokemon_button.style.display = 'inline-block';
}

document.getElementById('generation').addEventListener('change', clear_team);

function add_elements_to_select(select, object) {
	for (const element of object) {
		const option = document.createElement('option');
		option.value = element;
		option.textContent = element;
		select.appendChild(option);
	}
}

function add_keys_to_select(select, object) {
	add_elements_to_select(select, Object.keys(object));
}

function populate_generation() {
	const generation = document.getElementById('generation');
	generation.innerHTML = '';
	add_keys_to_select(generation, data);
	generation.value = "1";
	generate_most_likely_on_change(generation);
}

function create_species_input(pokemon, pokemon_data) {
	const species = document.createElement('select');
	species.className = 'species';
	add_keys_to_select(species, pokemon_data);
	species.addEventListener('change', () => {
		update_moves_and_abilities(pokemon, pokemon_data[species.value]);
		send_most_likely_data();
	});
	return species;
}

function create_numeric_input(label, min, max, initial) {
	const input = document.createElement('input');
	input.type = 'number';
	input.min = min;
	input.max = max;
	input.placeholder = label;
	input.value = initial;
	generate_most_likely_on_change(input);
	return input;
}

function create_level_input() {
	return create_numeric_input('level', 1, 100, 100);
}

function create_select(data, name) {
	const select = document.createElement('select');
	select.className = name;
	add_elements_to_select(select, data);
	generate_most_likely_on_change(select);
	return select;
}

function create_item_input(items_data) {
	return create_select(['Select item', ...items_data], 'item');
}

function create_ability_input() {
	const ability = document.createElement('select');
	ability.className = 'ability';
	generate_most_likely_on_change(ability);
	return ability;
}

function create_nature_input(natures_data) {
	const nature = create_select(natures_data, 'nature');
	nature.value = 'Hardy';
	return nature;
}

const stats = ['HP', 'Atk', 'Def', 'SpA', 'SpD', 'Spe'];

function create_ev_inputs() {
	const evs = document.createElement('div');
	stats.forEach(stat => {
		evs.appendChild(create_numeric_input(stat, 0, 252, 0));
	});
	return evs;
}

function create_move_input(move_data) {
	return create_select(['Select move', ...move_data], 'move');
}

function create_move_inputs() {
	const moves = document.createElement('div');
	moves.className = 'moves';
	return moves;
}

function update_moves_and_abilities(pokemon, pokemon_data) {
	if (pokemon_data.abilities) {
		const ability = pokemon.querySelector('.ability');
		ability.innerHTML = '';
		add_elements_to_select(ability, pokemon_data.abilities);
	}
	const moves = pokemon.querySelector('.moves');
	moves.innerHTML = '';
	for (let n = 0; n !== 4; ++n) {
		moves.appendChild(create_move_input(pokemon_data.moves));
	}
}

function create_pokemon_inputs(generation_data) {
	const pokemon = document.createElement('div');
	pokemon.className = 'pokemon';

	const species = create_species_input(pokemon, generation_data.pokemon);
	pokemon.appendChild(species);
	pokemon.appendChild(create_level_input());
	if (generation_data.items) {
		pokemon.appendChild(create_item_input(generation_data.items));
	}
	if (generation_data.natures) {
		pokemon.appendChild(create_ability_input());
		pokemon.appendChild(create_nature_input(generation_data.natures));
		pokemon.appendChild(create_ev_inputs());
	}
	pokemon.appendChild(create_move_inputs());

	update_moves_and_abilities(pokemon, generation_data.pokemon[species.value]);
	return pokemon;
}

add_pokemon_button.addEventListener('click', () => {
	const generation = document.getElementById('generation').value;
	const generation_data = data[generation];
	
	if (get_team().childElementCount < max_pokemon) {
		get_team().appendChild(create_pokemon_inputs(generation_data));
	}
	
	// Hide the button if we have reached the max number of Pokémon
	if (get_team().childElementCount >= max_pokemon) {
		add_pokemon_button.style.display = 'none';
	}

	send_most_likely_data();
});

function generate_most_likely_on_change(element) {
	element.addEventListener('change', send_most_likely_data);
}

document.getElementById('generate-random').addEventListener('click', send_random_data);
document.addEventListener('keydown', (event) => {
	if (event.key === 'Enter') {
		// Without this, the data is submitted twice
		event.preventDefault();
		send_random_data();
	}
});
document.getElementById('generate-most-likely').addEventListener('click', send_most_likely_data);

function send_data(style) {
	socket.send(JSON.stringify(team_to_json(style)));
}

function send_random_data() {
	send_data('random');
}

function send_most_likely_data() {
	send_data('most likely');
}

function team_to_json(style) {
	const generation = document.getElementById('generation').value;
	const team = [];
	const pokemon_elements = document.querySelectorAll('.pokemon');

	pokemon_elements.forEach(pokemon => {
		const species = pokemon.querySelector('.species').value;
		const level = pokemon.querySelector('input[placeholder="level"]').value;

		const pokemon_data = {species, level};

		const item = pokemon.querySelector('.item');
		if (item && item.value !== 'Select item') {
			pokemon_data.item = item.value;
		}

		const ability = pokemon.querySelector('.ability');
		if (ability) {
			pokemon_data.ability = ability.value;
		}

		const nature = pokemon.querySelector('.nature');
		if (nature) {
			if (nature.value !== 'Select nature') {
				pokemon_data.nature = nature.value;
			}
			const evs = {};
			stats.forEach(stat => {
				evs[stat] = pokemon.querySelector(`input[placeholder="${stat}"]`).value;
			});
			pokemon_data.evs = evs;
		}

		pokemon_data.moves = Array.from(pokemon.querySelectorAll('.move')).map(move => move.value).filter(move => move !== 'Select move');

		team.push(pokemon_data);
	});

	return { generation, style, team };
}

function display_predicted(predicted) {
	document.getElementById('predicted').textContent = predicted;
}