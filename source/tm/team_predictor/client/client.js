const socket = new WebSocket('ws://' + window.location.hostname + ':46924');

let data = null;
socket.onmessage = function(event) {
	const parsed = JSON.parse(event.data);

	if (parsed.generations) {
		data = parsed.generations;
		populate_generation();
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

function add_element_to_dropdown(dropdown, element) {
	const option = document.createElement('option');
	option.value = element;
	option.textContent = element;
	dropdown.appendChild(option);
}

function add_elements_to_dropdown(dropdown, object) {
	for (const element of object) {
		add_element_to_dropdown(dropdown, element);
	}
}

function add_keys_to_dropdown(dropdown, object) {
	add_elements_to_dropdown(dropdown, Object.keys(object));
}

function populate_generation() {
	const dropdown = document.getElementById('generation');
	dropdown.innerHTML = '';
	add_keys_to_dropdown(dropdown, data);
	dropdown.value = "1";
}

function create_species_dropdown(pokemon_data) {
	const dropdown = document.createElement('select');
	dropdown.className = 'species';
	add_keys_to_dropdown(dropdown, pokemon_data);
	return dropdown;
}

function create_numeric_input(label, min, max) {
	const input = document.createElement('input');
	input.type = 'number';
	input.min = min;
	input.max = max;
	input.placeholder = label;
	return input;
}

function create_level_input() {
	input = create_numeric_input('level', 1, 100);
	input.value = 100;
	return input;
}

function create_item_input(items_data) {
	const dropdown = document.createElement('select');
	dropdown.className = 'item';
	add_elements_to_dropdown(dropdown, items_data);
	return dropdown;
}

function create_ability_input() {
	const dropdown = document.createElement('select');
	dropdown.className = 'ability';
	return dropdown;
}

function create_nature_input(natures_data) {
	const dropdown = document.createElement('select');
	dropdown.className = 'item';
	add_elements_to_dropdown(dropdown, natures_data);
	return dropdown;
}

const stats = ['HP', 'Atk', 'Def', 'SpA', 'SpD', 'Spe'];

function create_ev_inputs() {
	const evs = document.createElement('div');
	stats.forEach(stat => {
		evs.appendChild(create_numeric_input(stat, 0, 252));
	});
	return evs;
}

function create_move_input(move_data) {
	const dropdown = document.createElement('select');
	dropdown.className = 'move';
	add_element_to_dropdown(dropdown, 'Select move');
	add_elements_to_dropdown(dropdown, move_data);
	return dropdown;
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
		add_elements_to_dropdown(ability, pokemon_data.abilities);
	}
	const moves = pokemon.querySelector('.moves');
	moves.innerHTML = '';
	for (let n = 0; n != 4; ++n) {
		moves.appendChild(create_move_input(pokemon_data.moves));
	}
}

function create_pokemon_inputs(generation_data) {
	const pokemon = document.createElement('div');
	pokemon.className = 'pokemon';

	const species = create_species_dropdown(generation_data.pokemon);
	species.addEventListener('change', () => {
		update_moves_and_abilities(pokemon, generation_data.pokemon[species.value]);
	});
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
});

document.getElementById('generate').addEventListener('click', send_data);
document.addEventListener('keydown', (event) => {
	if (event.key === 'Enter') {
		// Without this, the data is submitted twice
		event.preventDefault();
		send_data();
	}
});

function send_data() {
	socket.send(JSON.stringify(team_to_json()));
}

function team_to_json() {
	const generation = document.getElementById('generation').value;
	const style = document.getElementById('style').value;
	const team = [];
	const pokemon_elements = document.querySelectorAll('.pokemon');

	pokemon_elements.forEach(pokemon => {
		const species = pokemon.querySelector('.species').value;
		const level = pokemon.querySelector('input[placeholder="level"]').value;

		const pokemon_data = {species, level};

		const item = pokemon.querySelector('.item');
		if (item) {
			pokemon_data.item = item.value;
		}

		const ability = pokemon.querySelector('.ability');
		if (ability) {
			pokemon_data.ability = ability.value;
		}

		const nature = pokemon.querySelector('.nature');
		if (nature) {
			pokemon_data.nature = nature.value;
			const evs = {};
			stats.forEach(stat => {
				evs[stat] = pokemon.querySelector(`input[placeholder="${stat}"]`).value;
			});
			pokemon_data.evs = evs;
		}

		pokemon_data.moves = Array.from(pokemon.querySelectorAll('.move')).map(move => move.value).filter(move => move != 'Select move');

		team.push(pokemon_data);
	});

	return { generation, style, team };
}

function display_predicted(predicted) {
	document.getElementById('predicted').textContent = predicted;
}